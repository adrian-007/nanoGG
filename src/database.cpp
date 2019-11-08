/*
 * Copyright (C) 2010 adrian_007, adrian-007 on o2 point pl
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#include "database.h"
#include "contact.h"
#include "contactlist.h"
#include "contactgroup.h"
#include "debug.h"

#include <QVariant>
#include <QFile>
#include <QDir>

Database* Database::__instance = 0;

Database::Database(QObject *parent) : QObject(parent)
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("nanoGG.sqlite");

    open();
}

Database::~Database()
{
    if(isOpen())
    {
        close();
    }
}

void Database::createConfigTable()
{
    QSqlQuery q(db);

    if(q.exec(
                "create table config_strings(" \
                "key text unique not null," \
                "value text not null" \
                ");"
                ))
    {
        q.exec("insert into config_strings values('ggPassword', '');");
        q.exec("insert into config_strings values('lastDescription', '');");
        q.exec("insert into config_strings values('ggNumber', '');");
    }

    if(q.exec(
                "create table config_ints(" \
                "key text unique not null," \
                "value integer not null" \
                ");"
                ))
    {
        q.exec("insert into config_ints values('friendsOnly', 1);");
    }

    if(q.exec("select ggNumber, ggPassword, lastDescription, friendsOnly from config;"))
    {
        debug() << "got old table...";

        q.next();

        QSqlQuery qq(db);

        qq.prepare("update config_ints set value = :number where key like 'ggNumber';");
        qq.bindValue(":number", q.value(0).toInt());
        qq.exec();

        qq.prepare("update config_ints set value = :friendsOnly where key like 'friendsOnly';");
        qq.bindValue(":friendsOnly", q.value(3).toInt());
        qq.exec();

        qq.prepare("update config_strings set value = :pass where key like 'ggPassword';");
        qq.bindValue(":pass", q.value(1).toString());
        qq.exec();

        qq.prepare("update config_strings set value = :desc where key like 'lastDescription' = :desc;");
        qq.bindValue(":desc", q.value(2).toString());
        qq.exec();

        q.exec("drop table config;");
    }
}

void Database::createContactsTable()
{
    QSqlQuery q(db);

    q.exec(
                "create table groups(" \
                "id varchar(128) unique not null," \
                "name varchar2(128)," \
                "isExpanded integer default 1"
                ");"
                );

    q.exec(
                "create table contacts(" \
                "login varchar2(128) unique not null," \
                "name varchar2(128) not null," \
                "groupId varchar2(128) not null"
                ");"
                );
}

void Database::resetToDefaults()
{
    QSqlQuery q(db);

    q.exec("drop table contacts");
    q.exec("drop table groups");
    q.exec("drop table config_strings");
    q.exec("drop table config_ints");

    createConfigTable();
    createContactsTable();

    emit reloadData();
}

QString Database::getGGNumber()
{
    QSqlQuery q(db);

    if(q.exec("select value from config_strings where key like 'ggNumber';"))
    {
        q.next();
        return q.value(0).toString();
    }
    return 0;
}

QString Database::getGGPassword()
{
    QSqlQuery q(db);

    if(q.exec("select value from config_strings where key like 'ggPassword';"))
    {
        q.next();
        return q.value(0).toString();
    }
    return "";
}

bool Database::getFriendsOnly()
{
    QSqlQuery q(db);

    if(q.exec("select value from config_ints where key like 'friendsOnly';"))
    {
        q.next();
        return q.value(0).toInt() != 0;
    }
    return false;
}

QString Database::getLastDescription()
{
    QSqlQuery q(db);

    if(q.exec("select value from config_strings where key like 'lastDescription';"))
    {
        q.next();
        return q.value(0).toString();
    }
    return "";
}

bool Database::getGroupExpanded(const QString& groupId)
{
    QSqlQuery q(db);

    q.prepare("select isExpanded from groups where id = :groupId;");
    q.bindValue(":groupId", groupId);

    if(q.exec())
    {
        q.next();
        return q.value(0).toInt() != 0;
    }
    return true;
}

void Database::setGGNumber(const QString& login)
{
    QSqlQuery q(db);

    q.prepare("update config_strings set value = :number where key like 'ggNumber';");
    q.bindValue(":number", login);
    q.exec();
}

void Database::setGGPassword(const QString& password)
{
    QSqlQuery q(db);

    q.prepare("update config_strings set value = :password where key like 'ggPassword';");
    q.bindValue(":password", password);
    q.exec();
}

void Database::setFriendsOnly(bool value)
{
    QSqlQuery q(db);

    q.prepare("update config_ints set value = :fo where key like 'friendsOnly';");
    q.bindValue(":fo", value ? 1 : 0);
    q.exec();
}

void Database::setLastDescription(const QString& description)
{
    QSqlQuery q(db);

    q.prepare("update config_strings set value = :description where key like 'lastDescription';");
    q.bindValue(":description", description);
    q.exec();
}

void Database::setGroupExpanded(const QString &groupId, bool expanded)
{
    QSqlQuery q(db);

    q.prepare("update groups set isExpanded = :expanded where id = :groupId");
    q.bindValue(":expanded", expanded ? 1 : 0);
    q.bindValue(":groupId", groupId);
    q.exec();
}

void Database::loacContacts(QObject* sigReceiver)
{
    Contact* c;
    {
        QSqlQuery q(db);
        q.prepare("select id, name, isExpanded from groups");

        if(q.exec())
        {
            while(q.next())
            {
                c = new Contact(q.value(1).toString(), q.value(0).toString());

                if(q.value(2).toInt() == 0)
                {
                    c->setFlag(Contact::FLAG_HIDDEN);
                }
                else
                {
                    c->unsetFlag(Contact::FLAG_HIDDEN);
                }

                c->add();
            }
        }
    }

    {
        QSqlQuery q(db);
        q.prepare("select uin, name, groupId from contacts");

        if(q.exec())
        {
            while(q.next())
            {
               c = new Contact(q.value(0).toInt(), q.value(2).toString());
               c->setName(q.value(1).toString());
               c->add();

               connect(c, SIGNAL(statusUpdate(Contact*,QPixmap)), sigReceiver, SLOT(onStatusUpdate(Contact*,QPixmap)));
               connect(c, SIGNAL(newMessage(Contact*,bool)), sigReceiver, SLOT(onNewMessage(Contact*,bool)));
            }
        }
    }


    foreach(Contact* g, Contact::getContacts())
    {
        if(g->isGroup())
        {
            if(g->isFlagSet(Contact::FLAG_HIDDEN))
            {
                g->hide();
            }
        }
    }

    ContactList::getInstance()->sort();
}

void Database::updateContacts()
{
    QSqlQuery q(db);

    foreach(Contact* g, Contact::getContacts())
    {
        if(g->isGroup())
        {
            q.prepare("select name, isExpanded from groups where id = :groupId");
            q.bindValue(":groupId", g->group);

            if(q.exec())
            {
                q.next();
                g->setName(q.value(0).toString());

                if(q.value(1).toInt() == 0)
                {
                    g->unsetFlag(Contact::FLAG_HIDDEN);
                }
                else
                {
                    g->setFlag(Contact::FLAG_HIDDEN);
                    g->hide();
                }
            }
        }
        else
        {
            q.prepare("select name, groupId from contacts where uin = :uin");
            q.bindValue(":uin", g->uin);

            if(q.exec())
            {
                q.next();
                g->setName(q.value(0).toString());
                g->group = q.value(1).toString();
            }
        }
    }

    ContactList::getInstance()->sort();
}

void Database::clearContacts()
{
    QSqlQuery q(db);
    q.exec("drop table groups;");
    q.exec("drop table contacts;");

    createContactsTable();
}

bool Database::addContact(Contact* c)
{
    QSqlQuery q(db);

    bool ret;

    if(c->isGroup())
    {
        q.prepare("insert into groups values(:groupId, :name, :isExpanded)");
        q.bindValue(":groupId", c->group);
        q.bindValue(":name", c->getName());
        q.bindValue(":isExpanded", c->isFlagSet(Contact::FLAG_HIDDEN) ? 0 : 1);
        ret = q.exec();

        if(ret == false)
        {
            q.prepare("update groups set name = :name, isExpanded = :isExpanded where id like :groupId;");
            q.bindValue(":groupId", c->group);
            q.bindValue(":name", c->getName());
            q.bindValue(":isExpanded", c->isFlagSet(Contact::FLAG_HIDDEN) ? 0 : 1);
            q.exec();
        }
    }
    else
    {
        q.prepare("insert into contacts values(:uin, :name, :group)");
        q.bindValue(":uin", c->uin);
        q.bindValue(":name", c->getName());
        q.bindValue(":group", c->group);
        ret = q.exec();

        if(ret == false)
        {
            q.prepare("update contacts set name = :name, groupId = :group where uin = :uin;");
            q.bindValue(":uin", c->uin);
            q.bindValue(":name", c->getName());
            q.bindValue(":group", c->group);
            q.exec();
        }
    }

    return ret;
}
