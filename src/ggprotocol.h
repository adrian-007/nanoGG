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

#ifndef GGPROTOCOL_H
#define GGPROTOCOL_H

#include <QObject>
#include <QtNetwork/QNetworkAccessManager>
#include <QSslSocket>
#include <QByteArray>

class GGStream;
class QTimer;
class Contact;

class GGProtocol : public QObject
{
    Q_OBJECT
public:
    enum
    {
        STATUS_FFC = 0,
        STATUS_AVAILABLE,
        STATUS_DND,
        STATUS_BUSY,
        STATUS_INVISIBLE,
        STATUS_BLOCKED,
        STATUS_UNAVAILABLE
    };

    explicit GGProtocol(QObject *parent = 0);
    ~GGProtocol();

    static void newInstance()
    {
        if(__instance)
        {
            delete __instance;
        }
        __instance = new GGProtocol;
    }

    static void deleteInstance()
    {
        if(__instance)
        {
            delete __instance;
            __instance = 0;
        }
    }

    static GGProtocol* getInstance()
    {
        return __instance;
    }

    inline bool isConnected(bool skipStatus = false)
    {
        return (currentStatus != STATUS_UNAVAILABLE || skipStatus) && socket && socket->isOpen();
    }

    void setStatus(unsigned int status);
    void sendMessage(const QString& uin, const QString& message);
    void sendTypingNotify(const QString& uin, unsigned short count);
    void importContacts();
    void getNextAvatarReceiver();

    inline void updateStatus()
    {
        setStatus(currentStatus);
    }

signals:
    void incomingPacket(unsigned int type, const QByteArray& data);
    void outgoingPacket(unsigned int type, const QByteArray& data);
    void connection(bool disconnected);
    void ownPresence(unsigned int status, const QString& desc);
    void importFinished();

    void presence(const QString& uin, unsigned int status, const QString& desc);
    void message(const QString& uin, unsigned int time, const QString& message, bool& handled, bool ownMessage);
    void typingNotify(const QString& uin, unsigned short count);
    void newContact(const QString& uin);
    void newContactList();

public slots:
    void onHttpFinished(QNetworkReply*);
    void onAvatarFinished(QNetworkReply*);

    void onIncomingData();
    void onPendingData(qint64);
    void onSocketError(QAbstractSocket::SocketError);
    void onDisconnected();
    void onTimer();

private:
    static GGProtocol* __instance;
    static unsigned int message_count;

    unsigned int toStatus(unsigned int status, bool desc);
    unsigned int fromStatus(unsigned int status);

    QNetworkAccessManager* http;
    QSslSocket* socket;
    QTimer* timer;

    QByteArray tmpData;
    QByteArray data;
    QByteArray buffer;

    volatile unsigned int packetLength;
    volatile unsigned int packetType;
    volatile unsigned int currentStatus;
    volatile unsigned int sequenceNumber;
    QString ownUin;

    void establishConnection();
    void dispatch();
    void sendPacket(unsigned int type, const QByteArray& packet);
    void parseXmlList(const QByteArray& list);
    void sendNotifyInfo();

    void welcome(GGStream& s);
    void loginOk(GGStream& s);
    void loginFailed(GGStream& s);
    void notify(GGStream& s);
    void recvMsg(GGStream& s, bool own);
    void contactListReply(GGStream& s);
    void typingNotify(GGStream& s);
    void newList(GGStream& s);

    QNetworkAccessManager* avatars;
    Contact* curAvatarReceiver;

    void printPacket(const QByteArray& tmp, bool in = true, int type = -1, int len = -1);
};

#endif // GGPROTOCOL_H
