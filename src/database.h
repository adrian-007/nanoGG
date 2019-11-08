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

#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

class Contact;

class Database : public QObject {
    Q_OBJECT
public:
    explicit Database(QObject *parent = 0);
    ~Database();

    static void newInstance() {
        if(__instance)
            delete __instance;
        __instance = new Database;
    }

    static void deleteInstance() {
        if(__instance) {
            delete __instance;
            __instance = 0;
        }
    }

    static Database* getInstance() {
        return __instance;
    }

    bool open() {
        bool ret = db.open();
        if(ret) {
            createConfigTable();
            createContactsTable();
        }
        return ret;
    }
    void close() { db.close(); }
    bool isOpen() { return db.isOpen(); }
    void resetToDefaults();

    QString getGGNumber();
    QString getGGPassword();
    bool getFriendsOnly();
    QString getLastDescription();
    bool getGroupExpanded(const QString& groupId);

    void setGGNumber(const QString& number);
    void setGGPassword(const QString& password);
    void setFriendsOnly(bool value);
    void setLastDescription(const QString& description);
    void setGroupExpanded(const QString& groupId, bool expanded);

    void loacContacts(QObject* sigReceiver);
    bool addContact(Contact* c);
    void updateContacts();
    void clearContacts();

signals:
    void reloadData();

public slots:

private:
    static Database* __instance;

    void createConfigTable();
    void createContactsTable();

    QSqlDatabase db;
};

#endif // DATABASE_H
