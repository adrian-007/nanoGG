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

#ifndef CONTACT_H
#define CONTACT_H

#include <QObject>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QList>

#include "chattab.h"

class Contact : public QObject, public QStandardItem
{
    Q_OBJECT
public:
    enum
    {
        FLAG_PAGE_VISIBLE   = 0x0001,
        FLAG_IS_GROUP       = 0x0002,
        FLAG_HIDDEN         = 0x0004,
        FLAG_UNREAD_MESSAGE = 0x0008,
        FLAG_TYPING         = 0x0010,
        FLAG_REFRESH_AVATAR = 0x0020
    };

    void setFlag(int flag)
    {
        flags |= flag;
    }

    void unsetFlag(int flag)
    {
        flags &= ~flag;
    }

    bool isFlagSet(int flag) const
    {
        return (flags & flag) == flag;
    }

    Contact(const QString& name, const QString& groupId);
    Contact(unsigned int uin, const QString& groupId = QString());
    ~Contact();

    void add();
    void hide();
    void show();

    inline bool isGroup()
    {
        return isFlagSet(FLAG_IS_GROUP);
    }

    static QList<Contact*>& getContacts()
    {
        return contacts;
    }

    static Contact* getDefaultGroup();

    void createPage(bool show = false);
    void deletePage();
    bool hasPage() const
    {
        return widget != 0;
    }

    ChatTab* getPage()
    {
        return widget;
    }

    QString getStatusImagePath();
    void refreshPresence();

    static QString getStatusName(unsigned int s);
    static QString getStatusImagePath(unsigned int s);

    void setName(const QString& name);
    void setDescription(const QString& description);
    void setAvatar(const QPixmap& avatar);
    void setStatus(unsigned int status);

    QString getName() const;
    QString getDescription() const;
    QPixmap getAvatar() const;

    QString name;
    QString desc;
    QString group;

    QString uin;
    unsigned int status;

public slots:
    void onPresence(const QString& uin, unsigned int status, const QString& desc);
    void onMessage(const QString& uin, unsigned int time, const QString& msg, bool& handled, bool ownMessage);
    void onTypingNotify(const QString& uin, unsigned short count);

signals:
    void statusUpdate(Contact* c, const QPixmap&);
    void newMessage(Contact* c, bool forceShow);

private:
    void setStatusIcon(const QPixmap& icon);

    ChatTab* widget;
    int flags;

    void init();
    bool operator < (const QStandardItem& other) const;
    static QList<Contact*> contacts;
    static Contact* defaultGroup;
};

#endif // CONTACT_H
