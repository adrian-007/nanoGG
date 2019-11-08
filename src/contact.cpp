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

#include "contact.h"
#include "ggprotocol.h"
#include "contactgroup.h"
#include "contactlist.h"
#include "database.h"
#include "notify.h"
#include "debug.h"

#include <QApplication>
#include <QDir>
#include <QDateTime>

QList<Contact*> Contact::contacts;
Contact* Contact::defaultGroup = 0;

Contact* Contact::getDefaultGroup()
{
    if(defaultGroup == 0)
    {
        defaultGroup = new Contact("Spoza listy", "{Z-NONE}");
        defaultGroup->add();
    }

    return defaultGroup;
}

Contact::Contact(const QString &name, const QString &groupId) : QObject(), flags(0), widget(0)
{
    this->setFlag(FLAG_IS_GROUP);

    if(name.isEmpty())
    {
        setName("< Brak nazwy >");
    }
    else
    {
        setName(name);
    }

    this->group = groupId;
    this->uin = "";

    this->setIcon(QPixmap(":/res/expanded.png"));
    this->setEditable(false);
}

Contact::Contact(unsigned int uin, const QString& groupId) : QObject(), flags(0), widget(0)
{
    this->uin = uin;
    this->group = groupId;
    this->setFlag(FLAG_REFRESH_AVATAR);

    status = (unsigned int)GGProtocol::STATUS_UNAVAILABLE;

    this->setEditable(false);

    connect(GGProtocol::getInstance(), SIGNAL(presence(uint,uint,QString)), SLOT(onPresence(uint,uint,QString)));
    connect(GGProtocol::getInstance(), SIGNAL(message(uint,uint,QString,bool&,bool)), SLOT(onMessage(uint,uint,QString,bool&,bool)));
    connect(GGProtocol::getInstance(), SIGNAL(typingNotify(uint,unsigned short)), SLOT(onTypingNotify(uint,unsigned short)));
}

Contact::~Contact()
{
    if(widget)
    {
        delete widget;
        widget = 0;
    }

    if(this->isGroup())
    {
        Database::getInstance()->setGroupExpanded(group, isFlagSet(FLAG_HIDDEN) == false);
    }

    debug() << "contact" << this->uin << "deleted";
}

void Contact::setName(const QString &name)
{
    this->setData(name, Qt::DisplayRole);
}

void Contact::setDescription(const QString &description)
{
    this->setData(description, Qt::UserRole + 1);

    if(widget)
    {
        widget->setHeader(description);
        widget->status(description);
        Notify::presence();
    }
}

void Contact::setStatusIcon(const QPixmap &icon)
{
    this->setData(icon, Qt::UserRole + 2);
}

void Contact::setAvatar(const QPixmap &avatar)
{
    this->setData(avatar, Qt::UserRole + 3);
}

void Contact::setStatus(unsigned int status)
{
    this->status = status;
    setStatusIcon(QPixmap(getStatusImagePath()));
}

QString Contact::getName() const
{
    return this->data(Qt::DisplayRole).value<QString>();
}

QString Contact::getDescription() const
{
    return this->data(Qt::UserRole + 1).value<QString>();
}

QPixmap Contact::getAvatar() const
{
    return this->data(Qt::UserRole + 3).value<QPixmap>();
}

void Contact::add()
{
    contacts.append(this);

    if(this->isFlagSet(FLAG_IS_GROUP) == false)
    {
        QString path = QString("%1/avatars/%2.avatar").arg(QDir::currentPath()).arg(uin);
        QPixmap av(path);

        if(av.isNull() == false)
        {
            setAvatar(av);
            this->unsetFlag(FLAG_REFRESH_AVATAR);
        }
        else
        {
            this->setFlag(FLAG_REFRESH_AVATAR);
        }

        setStatusIcon(QPixmap(getStatusImagePath()));
        show();
    }
    else
    {
        this->setSelectable(false);
        ContactList::getInstance()->getModel().appendRow(this);
    }
}

void Contact::hide()
{
    if(this->isGroup())
    {
        this->setFlag(FLAG_HIDDEN);
        this->setIcon(QPixmap(":/res/collapsed.png"));

        foreach(Contact* c, getContacts())
        {
            if(c->isGroup())
                continue;

            if(c->group == this->group)
            {
                c->hide();
            }
        }

        ContactList::getInstance()->sort();
        Database::getInstance()->setGroupExpanded(group, false);
    }
    else
    {
        ContactList::getInstance()->getModel().takeRow(this->index().row());
    }
}

void Contact::show()
{
    if(this->isGroup())
    {
        this->unsetFlag(FLAG_HIDDEN);
        this->setIcon(QPixmap(":/res/expanded.png"));

        foreach(Contact* c, getContacts())
        {
            if(c->isGroup())
                continue;

            if(c->group == this->group)
            {
                c->show();
            }
        }

        ContactList::getInstance()->sort();
        Database::getInstance()->setGroupExpanded(group, true);
    }
    else
    {
        ContactList::getInstance()->getModel().appendRow(this);
    }
}

void Contact::refreshPresence()
{
    QPixmap img(this->getStatusImagePath());
    setStatusIcon(img);

    emit statusUpdate(this, img);
}

void Contact::onPresence(const QString& uin, unsigned int status, const QString &desc)
{
    if(this->uin != uin)
        return;

    this->status = status;
    setDescription(desc);

    QPixmap img(getStatusImagePath());

    setStatusIcon(img);

    emit statusUpdate(this, img);
}

void Contact::onMessage(const QString& uin, unsigned int time, const QString &msg, bool& handled, bool ownMessage)
{
    if(this->uin != uin)
        return;

    this->setFlag(FLAG_UNREAD_MESSAGE);
    this->unsetFlag(FLAG_TYPING);
    this->refreshPresence();

    createPage();

    QString message = msg;
    message.replace("MS Shell Dlg 2", "Tahoma");
    message.remove(QRegExp("font-size: ?9pt;?"));

    QDateTime dt;
    dt.setTime_t(time);

    if(ownMessage)
    {
        widget->message("", message, dt.time());
    }
    else
    {
        Notify::message();
        widget->message(this->getName(), message, dt.time());
    }

    handled = true;
}

void Contact::onTypingNotify(const QString& uin, unsigned short count)
{
    if(this->uin != uin)
        return;

    if(count > 0)
        this->setFlag(FLAG_TYPING);
    else
        this->unsetFlag(FLAG_TYPING);

    refreshPresence();
}

QString Contact::getStatusImagePath()
{
    if(this->isFlagSet(FLAG_TYPING))
    {
        return ":/res/typing.png";
    }
    else if(this->isFlagSet(FLAG_UNREAD_MESSAGE))
    {
        return ":/res/message.png";
    }

    return getStatusImagePath(this->status);
}

QString Contact::getStatusImagePath(unsigned int s)
{
    QString img;
    switch(s)
    {
    case GGProtocol::STATUS_AVAILABLE:
    {
        img = ":/res/available.png";
        break;
    }
    case GGProtocol::STATUS_BUSY:
    {
        img = ":/res/away.png";
        break;
    }
    case GGProtocol::STATUS_DND:
    {
        img = ":/res/dnd.png";
        break;
    }
    case GGProtocol::STATUS_FFC:
    {
        img = ":/res/chat.png";
        break;
    }
    case GGProtocol::STATUS_INVISIBLE:
    {
        img = ":/res/invisible.png";
        break;
    }
    case GGProtocol::STATUS_BLOCKED:
    {
        img = ":/res/blocked.png";
        break;
    }
    default:
    {
        img = ":/res/unavailable.png";
    }
    }
    return img;
}

QString Contact::getStatusName(unsigned int s)
{
    QString img;
    switch(s)
    {
    case GGProtocol::STATUS_AVAILABLE:
    {
        img = "Dostępny";
        break;
    }
    case GGProtocol::STATUS_BUSY:
    {
        img = "Zaraz wracam";
        break;
    }
    case GGProtocol::STATUS_DND:
    {
        img = "Jestem zajęty";
        break;
    }
    case GGProtocol::STATUS_FFC:
    {
        img = "Porozmawiajmy";
        break;
    }
    case GGProtocol::STATUS_INVISIBLE:
    {
        img = "Niewidoczny";
        break;
    }
    case GGProtocol::STATUS_BLOCKED:
    {
        img = "Zablokowany";
        break;
    }
    default:
    {
        img = "Niedostępny";
    }
    }
    return img;
}

void Contact::createPage(bool show /*= false*/)
{
    QString d = getDescription();

    QPixmap img;

    if(widget == 0)
    {
        widget = new ChatTab(this);
        widget->setHeader(d);
        widget->status(d);
    }
    else
    {
    }

    emit newMessage(this, show);
    emit statusUpdate(this, QPixmap(this->getStatusImagePath()));
}

void Contact::deletePage()
{
}

bool Contact::operator < (const QStandardItem& other) const
{
    const Contact* rhs = dynamic_cast<const Contact*>(&other);

    if (rhs)
    {
        if (this->group == rhs->group)
        {
            if (rhs->isFlagSet(FLAG_IS_GROUP) == false && this->isFlagSet(FLAG_IS_GROUP) == false)
            {
                if (this->status == rhs->status)
                {
                    return QStandardItem::operator <(other);
                }
                else
                {
                    return this->status < rhs->status;
                }
            }
            else
            {
                return false;
            }
        }
        else
        {
            return this->group < rhs->group;
        }
    }

    return false;
}
