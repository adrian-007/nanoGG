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

#include "ggprotocol.h"
#include "database.h"
#include "ggstream.h"
#include "contact.h"
#include "contactlist.h"
#include "version.h"
#include "debug.h"
#include "notify.h"

#include <QtNetwork/QNetworkReply>
#include <QStringList>
#include <QCryptographicHash>
#include <QTimer>
#include <QTextCodec>
#include <QTextDocument>
#include <QXmlStreamReader>
#include <QFile>
#include <QDir>

#include <ctime>
#include <zlib/zlib.h>

#define GG11

#ifndef GG11
// przychodzace
#define GG_WELCOME                          0x00000001
#define GG_LOGIN_OK                         0x00000035
#define GG_LOGIN_FAILED                     0x00000043
#define GG_DISCONNECTING                    0x0000000B
#define GG_DISCONNECTING_ACK                0x0000000D
#define GG_USERLIST_REPLY                   0x00000030
#define GG_STATUS_CHANGE                    0x00000036
#define GG_NOTIFY_REPLY                     0x00000037
#define GG_RECV_MSG                         0x0000002E
#define GG_PONG                             0x00000007
#define GG_CONTACTS_LIST_REPLY              0x00000030
#define GG_OWN_MESSAGE                      0x0000005A
#define GG_USERLIST_VERSION                 0x0000005C
#define GG_XML_EVENT                        0x00000027
#define GG_SEND_MSG_ACK                     0x00000005

// wychodzace
#define GG_LOGIN                            0x00000031
#define GG_USERLIST_REQUEST                 0x0000002F
#define GG_NEW_STATUS                       0x00000038
#define GG_OUTGOING_MSG                     0x0000002D
#define GG_USERLIST_EMPTY                   0x00000012
#define GG_NOTIFY_FIRST                     0x0000000F
#define GG_NOTIFY_LAST                      0x00000010
#define GG_PING                             0x00000008
#define GG_RECV_MSG_ACK                     0x00000046
#define GG_ADD_NOTIFY                       0x0000000D
#define GG_SEND_MESSAGE                     0x0000002D
#define GG_CONTACTS_LIST_REQUEST            0x0000002F

// two way
#define GG_TYPING_NOTIFY                    0x00000059

// flagi i takie tam
#define GG_STATUS_FLAGS                     (0x00100000 | 0x00800000)
#define GG_PROTO_FEATURES                   (0x00000001 | 0x00000002 | 0x00000004 | 0x00000010 |/* 0x00000020 |*/ 0x00000040 | 0x00000100 | 0x00000400 | 0x00002000 | 0x00004000)
#define GG_MAX_MESSAGE_LEN                  1500
#define GG_CONTACTS_LIST_GET                0x02
#define GG_CONTACTS_LIST_REPLY_GET          0x06
#define GG_CONTACTS_LIST_REPLY_GET_MORE     0x04

// statusy
#define GG_STATUS_NOT_AVAIL                 0x0001
#define GG_STATUS_NOT_AVAIL_DESCR           0x0015
#define GG_STATUS_FFC                       0x0017
#define GG_STATUS_FFC_DESCR                 0x0018
#define GG_STATUS_AVAIL                     0x0002
#define GG_STATUS_AVAIL_DESCR               0x0004
#define GG_STATUS_BUSY                      0x0003
#define GG_STATUS_BUSY_DESCR                0x0005
#define GG_STATUS_DND                       0x0021
#define GG_STATUS_DND_DESCR                 0x0022
#define GG_STATUS_INVISIBLE                 0x0014
#define GG_STATUS_INVISIBLE_DESCR           0x0016
#define GG_STATUS_BLOCKED                   0x0006

#else
// przychodzace
#define GG_WELCOME                          0x00000001
#define GG_LOGIN_OK                         0x00000035
#define GG_LOGIN_FAILED                     0x00000043
#define GG_DISCONNECTING                    0x0000000B
#define GG_DISCONNECTING_ACK                0x0000000D
#define GG_USERLIST_REPLY                   0x00000030
#define GG_STATUS_CHANGE                    0x00000036
#define GG_NOTIFY_REPLY                     0x00000037
#define GG_RECV_MSG                         0x0000007E
#define GG_PONG                             0x000000A1
#define GG_CONTACTS_LIST_REPLY              0x00000030
#define GG_OWN_MESSAGE                      0x0000005A
#define GG_USERLIST_VERSION                 0x0000005C
#define GG_XML_EVENT                        0x00000027
#define GG_SEND_MSG_ACK                     0x00000005

// wychodzace
#define GG_LOGIN                            0x00000083
#define GG_USERLIST_REQUEST                 0x0000002F
#define GG_NEW_STATUS                       0x00000038
#define GG_USERLIST_EMPTY                   0x00000012
#define GG_NOTIFY_FIRST                     0x0000000F
#define GG_NOTIFY_LAST                      0x00000078
#define GG_PING                             0x00000008
#define GG_RECV_MSG_ACK                     0x00000086
#define GG_ADD_NOTIFY                       0x0000000D
#define GG_SEND_MESSAGE                     0x0000007D
#define GG_CONTACTS_LIST_REQUEST            0x0000002F

// two way
#define GG_TYPING_NOTIFY                    0x00000059

// flagi i takie tam
#define GG_STATUS_FLAGS                     (0x00100000 | 0x00800000)
#define GG_PROTO_FEATURES                   (0x00000001 | 0x00000002 | 0x00000004 | 0x00000010 |/* 0x00000020 |*/ 0x00000040 | 0x00000100 | 0x00000400 | 0x00002000 | 0x00004000)
#define GG_MAX_MESSAGE_LEN                  1500
#define GG_CONTACTS_LIST_GET                0x02
#define GG_CONTACTS_LIST_REPLY_GET          0x06
#define GG_CONTACTS_LIST_REPLY_GET_MORE     0x04

// statusy
#define GG_STATUS_NOT_AVAIL                 0x0001
#define GG_STATUS_NOT_AVAIL_DESCR           0x0015
#define GG_STATUS_FFC                       0x0017
#define GG_STATUS_FFC_DESCR                 0x0018
#define GG_STATUS_AVAIL                     0x0002
#define GG_STATUS_AVAIL_DESCR               0x0004
#define GG_STATUS_BUSY                      0x0003
#define GG_STATUS_BUSY_DESCR                0x0005
#define GG_STATUS_DND                       0x0021
#define GG_STATUS_DND_DESCR                 0x0022
#define GG_STATUS_INVISIBLE                 0x0014
#define GG_STATUS_INVISIBLE_DESCR           0x0016
#define GG_STATUS_BLOCKED                   0x0006

#undef NANOGG_DEBUG
#define NANOGG_DEBUG 1

#endif

#if NANOGG_DEBUG

#include <string>

std::string print_hex(const void* buf, int len)
{
    char hex[32];
    std::string str;
    std::string header = "---|-";

    for(int i = 1; i <= 16; ++i)
    {
        sprintf(hex, "%02d-", i);
        header += hex;
    }

    header += "|" + std::string(18, '-');
    str += header;
    str += "\n   |" + std::string(49, ' ') + "|";
    str += "\n001| ";

    const unsigned char* uBuf = (const unsigned char*)buf;

    for(int i = 0; i < len; ++i)
    {
        sprintf(hex, "%02X ", (unsigned int)uBuf[i]);
        str += hex;

        if(((i+1) % 16) == 0)
        {
            str += "| ";

            for(int j = i-15; j < i+1; ++j)
            {
                if(::isprint((unsigned int)uBuf[j]))
                {
                    str += uBuf[j];
                }
                else
                {
                    str += ".";
                }
            }

            if((i+1) != len)
            {
                sprintf(hex, "\n%03d| ", ((i / 16) + 2));
                str += hex;
            }
        }
    }

    int padding = len % 16;

    if(padding != 0)
    {
        str += std::string((16 - padding)*3 - 1, ' ');
        str += " | ";

        for(int j = 0; j < padding; ++j)
        {
            if(::isprint((unsigned int)uBuf[len-padding+j]))
            {
                str += uBuf[len-padding+j];
            }
            else
            {
                str += ".";
            }
        }
    }

    str += "\n   |" + std::string(49, ' ') + "|\n";
    str += header;

    return str;
}

void GGProtocol::printPacket(const QByteArray& tmp, bool in /*= true*/, int type /*= -1*/, int len /*= -1*/)
{
    if(type == -1)
    {
        type = packetType;
    }

    if(len == -1)
    {
        len = packetLength;
    }

    QString str = QString((in ? "[IN] " : "[OUT]")) + QString(" Packet: type=0x") + QByteArray::number((unsigned)type, 16).toUpper() + QString(" len=") + QByteArray::number((unsigned)len) + QString(" (") + QByteArray::number(tmp.length()) + QString(")");

    str += QString("\n");
    str += print_hex(tmp.constData(), tmp.length()).c_str();
    str += "\n\n";

    debug(str.toAscii());
}

#else

void GGProtocol::printPacket(const QByteArray&, bool)
{
}

#endif

GGProtocol* GGProtocol::__instance = 0;
unsigned int GGProtocol::message_count = 0;

GGProtocol::GGProtocol(QObject *parent) : QObject(parent)
{
    http = 0;
    socket = 0;
    curAvatarReceiver = 0;

    packetLength = packetType = sequenceNumber = 0;
    currentStatus = STATUS_UNAVAILABLE;

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), SLOT(onTimer()));

    avatars = 0;
}

GGProtocol::~GGProtocol()
{
    if(isConnected())
    {
        setStatus(STATUS_UNAVAILABLE);
    }

    if(timer)
    {
        timer->stop();
    }
}

void GGProtocol::getNextAvatarReceiver()
{
    if(avatars == 0)
    {
        avatars = new QNetworkAccessManager(this);
        connect(avatars, SIGNAL(finished(QNetworkReply*)), SLOT(onAvatarFinished(QNetworkReply*)));

        QDir().mkdir(QDir::currentPath() + "/avatars");
    }

    curAvatarReceiver = 0;
    foreach(Contact* c, Contact::getContacts())
    {
        if(c->isGroup())
        {
            continue;
        }

        if(c->isFlagSet(Contact::FLAG_REFRESH_AVATAR) && c != curAvatarReceiver)
        {
            curAvatarReceiver = c;
            break;
        }
    }

    if(curAvatarReceiver != 0)
    {
        debug() << QString("getting avatar for %1").arg(curAvatarReceiver->uin);
        curAvatarReceiver->unsetFlag(Contact::FLAG_REFRESH_AVATAR);
        avatars->get(QNetworkRequest(QUrl(QString("http://avatars.gg.pl/%1").arg(curAvatarReceiver->uin))));
    }
    else
    {
#ifndef Q_OS_SYMBIAN
        avatars->deleteLater();
        avatars = 0;
#endif
        debug("avatar fetch finished");
    }
}

void GGProtocol::establishConnection()
{
    if(http == 0)
    {
        http = new QNetworkAccessManager(this);
        connect(http, SIGNAL(finished(QNetworkReply*)), SLOT(onHttpFinished(QNetworkReply*)));
    }

    http->get(QNetworkRequest(QUrl(QString("http://appmsg.gadu-gadu.pl/appsvc/appmsg_ver11.asp?fmnumber=%1&ssl=1").arg(Database::getInstance()->getGGNumber()))));
    message_count = 0;
}

void GGProtocol::onHttpFinished(QNetworkReply* reply)
{
    if(reply->error() == QNetworkReply::NoError)
    {
        QString line = reply->readAll();

        if(line[0] == '0')
        {
            QStringList ipport = line.split(' ')[2].split(':');
            debug() << "got server data, ip: " << ipport[0] << " port:" << ipport[1];

            if(socket == 0)
            {
                socket = new QSslSocket(this);
                connect(socket, SIGNAL(readyRead()), SLOT(onIncomingData()));
                connect(socket, SIGNAL(bytesWritten(qint64)), SLOT(onPendingData(qint64)));
                connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), SLOT(onSocketError(QAbstractSocket::SocketError)));
                connect(socket, SIGNAL(disconnected()), SLOT(onDisconnected()));
            }

            socket->connectToHostEncrypted(ipport[0], ipport[1].toInt());
        }
    }
    else
    {
        debug() << "failed to establish connection: " << qPrintable(reply->errorString());
        emit ownPresence(STATUS_UNAVAILABLE, Database::getInstance()->getLastDescription());
    }

    reply->deleteLater();
}

void GGProtocol::onAvatarFinished(QNetworkReply* reply)
{
    if(reply->error() == QNetworkReply::NoError)
    {
        QByteArray b = reply->readAll();
        if(b.length() > 0 && curAvatarReceiver != 0)
        {
            QString path = QString("%1/avatars/%2.avatar").arg(QDir::currentPath()).arg(curAvatarReceiver->uin);
            debug() << path;
            QFile file(path);

            if(file.exists())
            {
                file.remove();
            }

            if(file.open(QIODevice::WriteOnly))
            {
                file.write(b);
                file.close();

                curAvatarReceiver->setAvatar(QPixmap(path));
            }
        }
        else
        {
            QString redirectUrl = reply->attribute(QNetworkRequest::RedirectionTargetAttribute).value<QString>();

            if(redirectUrl.isEmpty() == false)
            {
                avatars->get(QNetworkRequest(redirectUrl));
                return;
            }
        }
    }
    else
    {
        debug(qPrintable(reply->errorString()));
    }

    reply->deleteLater();
    getNextAvatarReceiver();
}

void GGProtocol::setStatus(unsigned int status)
{
    if(currentStatus == STATUS_UNAVAILABLE && status == STATUS_UNAVAILABLE)
    {
        return;
    }

    if(currentStatus == STATUS_UNAVAILABLE)
    {
        currentStatus = status;
        establishConnection();
    }
    else
    {
        QString desc = Database::getInstance()->getLastDescription();

        if(desc.length() > 255)
        {
            desc = desc.left(255);
        }

        unsigned int st = toStatus(status, false);

        if(desc.length() > 0)
        {
            st |= 0x4000;
        }

        if(Database::getInstance()->getFriendsOnly())
        {
            //st |= 0x8000;
        }

        QByteArray packet;
        GGStream stream(&packet, QIODevice::WriteOnly);

        stream << st;
        //stream << (unsigned int)GG_STATUS_FLAGS;
        stream << (unsigned int)0x00000014;
        stream.appendString(desc, true);
        stream.appendAndRepeat(0x00, 4);

        sendPacket(GG_NEW_STATUS, packet);

        currentStatus = status;

        // After setting presence to offline, server will terminate the connection - that may serve as a kind of ack.
        //if(status == STATUS_UNAVAILABLE) {
        //    socket->close();
        //}
    }
}

void GGProtocol::sendMessage(const QString& uin, const QString &message)
{
    if(message.length() > GG_MAX_MESSAGE_LEN)
    {
        QString msg = message;
        QString tmp;

        while(msg.isEmpty() == false)
        {
            tmp = msg.left(msg.length() > GG_MAX_MESSAGE_LEN ? GG_MAX_MESSAGE_LEN : msg.length());
            sendMessage(uin, tmp);
            msg.remove(0, tmp.length());
        }
        return;
    }

    QTextCodec* codec = QTextCodec::codecForName("cp1250");

    QByteArray plain = codec->fromUnicode(message);
    QByteArray html;
    html.append("<span style=\"font-family:'arial';\">");
    html.append(Qt::escape(message).replace("\n", "\n<br/>"));
    html.append("</span>");

    QByteArray packet;
    GGStream stream(&packet, QIODevice::WriteOnly);

#ifndef GG11
    stream << (unsigned int)uin;
    stream << (unsigned int)time(0);
    stream << (unsigned int)0x0008;
    stream << (unsigned int)(offset + html.size());
    stream << (unsigned int)(offset + html.size() + plain.size());
    stream.appendByteArray(html);
    stream.appendByteArray(plain);

    stream << (unsigned char)0x02;
    stream << (unsigned char)0x06;
    stream << (unsigned char)0x00;
    stream << (unsigned char)0x00;
    stream << (unsigned char)0x00;
    stream << (unsigned char)0x08;
    stream << (unsigned char)0x00;
    stream << (unsigned char)0x00;
    stream << (unsigned char)0x00;
#else
    QString to = uin;

    stream << (unsigned char)0x0A;
    stream.appendVariableValue(to.length() + 2);
    stream << (unsigned char)0x00;
    stream.appendVariableValue(to.length());
    stream.appendString(to);

    stream << (unsigned char)0x10;
    stream << (unsigned char)0x08;
    stream << (unsigned char)0x18;
    stream.appendVariableValue(++message_count);

    stream << (unsigned char)0x2A;
    stream.appendVariableValue(plain.length());
    stream.appendString(plain);

    stream << (unsigned char)0x32;
    stream.appendVariableValue(html.length());
    stream.appendString(html);
#endif

    sendPacket(GG_SEND_MESSAGE, packet);
}

void GGProtocol::importContacts()
{
    if(buffer.size() > 0)
    {
        return;
    }

    QByteArray packet;
    GGStream stream(&packet, QIODevice::WriteOnly);
    stream << (unsigned char)GG_CONTACTS_LIST_GET;

    sendPacket(GG_CONTACTS_LIST_REQUEST, packet);
}

void GGProtocol::sendTypingNotify(const QString& uin, unsigned short count)
{
    QByteArray packet;
    GGStream stream(&packet, QIODevice::WriteOnly);

    stream << (unsigned short)count;
    stream << (unsigned int)uin.toInt();
    sendPacket(GG_TYPING_NOTIFY, packet);
}

unsigned int GGProtocol::toStatus(unsigned int status, bool desc)
{
    unsigned int ret = 0;

    if(desc)
    {
        ret |= 0x4000;
    }

    switch(status)
    {
    case STATUS_AVAILABLE:
    {
        ret |= desc ? GG_STATUS_AVAIL_DESCR : GG_STATUS_AVAIL;
        break;
    }
    case STATUS_FFC:
    {
        ret |= desc ? GG_STATUS_FFC_DESCR : GG_STATUS_FFC;
        break;
    }
    case STATUS_DND:
    {
        ret |= desc ? GG_STATUS_DND_DESCR : GG_STATUS_DND;
        break;
    }
    case STATUS_BUSY:
    {
        ret |= desc ? GG_STATUS_BUSY_DESCR : GG_STATUS_BUSY;
        break;
    }
    case STATUS_INVISIBLE:
    {
        ret |= desc ? GG_STATUS_INVISIBLE_DESCR : GG_STATUS_INVISIBLE;
        break;
    }
    case STATUS_UNAVAILABLE:
    {
        ret |= desc ? GG_STATUS_NOT_AVAIL_DESCR : GG_STATUS_NOT_AVAIL;
        break;
    }
    case STATUS_BLOCKED:
    {
        ret |= GG_STATUS_BLOCKED;
        break;
    }
    }

    return ret;
}

unsigned int GGProtocol::fromStatus(unsigned int status)
{
    status &= 0x00FF;

    switch(status)
    {
    case GG_STATUS_NOT_AVAIL:
    case GG_STATUS_NOT_AVAIL_DESCR:
    {
        return STATUS_UNAVAILABLE;
    }
    case GG_STATUS_FFC:
    case GG_STATUS_FFC_DESCR:
    {
        return STATUS_FFC;
    }
    case GG_STATUS_AVAIL:
    case GG_STATUS_AVAIL_DESCR:
    {
        return STATUS_AVAILABLE;
    }
    case GG_STATUS_BUSY:
    case GG_STATUS_BUSY_DESCR:
    {
        return STATUS_BUSY;
    }
    case GG_STATUS_DND:
    case GG_STATUS_DND_DESCR:
    {
        return STATUS_DND;
    }
    case GG_STATUS_INVISIBLE:
    case GG_STATUS_INVISIBLE_DESCR:
    {
        return STATUS_INVISIBLE;
    }
    case GG_STATUS_BLOCKED:
    {
        return STATUS_BLOCKED;
    }
    }
    return 0xFFFF;
}

void GGProtocol::onIncomingData()
{
    tmpData += socket->readAll();

    if(tmpData.length() == 0)
    {
        return;
    }

start_processing:

    if(tmpData.size() >= 8)
    {
        {
            GGStream stream(&tmpData, QIODevice::ReadOnly);
            unsigned int type, length;
            stream >> type;
            stream >> length;

            packetType = type;
            packetLength = length;
        }

        if(tmpData.size() >= (int)(packetLength + 8))
        {
            data = tmpData.mid(8, packetLength);
            dispatch();
            tmpData.remove(0, packetLength + 8);

            data.clear();
            packetType = packetLength = 0;
            goto start_processing;
        }
    }
}

void GGProtocol::onPendingData(qint64 /*written*/)
{
}

void GGProtocol::onDisconnected()
{
    Notify::setEnabled(false);

    timer->stop();
    emit connection(true);

    foreach(Contact* c, Contact::getContacts())
    {
        if(c->isGroup())
        {
            continue;
        }

        c->setStatus(STATUS_UNAVAILABLE);
        c->refreshPresence();
        c->setDescription("");
    }

    ContactList::getInstance()->sort();

    currentStatus = STATUS_UNAVAILABLE;
    emit ownPresence(STATUS_UNAVAILABLE, Database::getInstance()->getLastDescription());
    debug("socket disconnected");
}

void GGProtocol::onSocketError(QAbstractSocket::SocketError /*error*/)
{
    debug(qPrintable(socket->errorString()));
    socket->disconnectFromHost();
    socket->abort();
    currentStatus = STATUS_UNAVAILABLE;
}

void GGProtocol::dispatch()
{
    emit incomingPacket(packetType, data);

    GGStream stream(&data, QIODevice::ReadOnly);
    printPacket(data, true);

    switch(packetType)
    {
    case GG_WELCOME:
    {
        debug("GG_WELCOME");
        welcome(stream);
        break;
    }
    //case GG_LOGIN_OK:
    case 0x9D:
    {
        debug("GG_LOGIN_OK");
        timer->start(1000 * 60 * 3); // Runs every 3 minutes
        loginOk(stream);
        break;
    }
    case GG_LOGIN_FAILED:
    {
        debug("GG_LOGIN_FAILED");
        loginFailed(stream);
        socket->close();
        break;
    }
    case GG_NOTIFY_REPLY:
    case GG_STATUS_CHANGE:
    {
        debug("GG_STATUS/GG_NOTIFY");
        notify(stream);
        break;
    }
    case GG_RECV_MSG:
    {
        debug("GG_RECV_MSG");
        recvMsg(stream, false);
        break;
    }
    case GG_SEND_MSG_ACK:
    {
        debug("GG_SEND_MSG_ACK");
        break;
    }
    case GG_OWN_MESSAGE:
    {
        debug("GG_OWN_MESSAGE");
        recvMsg(stream, true);
        break;
    }
    case GG_TYPING_NOTIFY:
    {
        debug("GG_TYPING_NOTIFY");
        typingNotify(stream);
        break;
    }
    case GG_PONG:
    {
        debug("GG_PONG");
        break;
    }
    case GG_CONTACTS_LIST_REPLY:
    {
        debug("GG_CONTACTS_LIST_REPLY");
        contactListReply(stream);
        break;
    }
    case GG_USERLIST_VERSION:
    {
        debug("GG_USERLIST_VERSION");
        newList(stream);
        break;
    }
    case GG_XML_EVENT:
    {
        debug("GG_XML_EVENT");
        break;
    }
    case GG_DISCONNECTING:
    {
        debug("GG_DISCONNECTING");
        break;
    }
    case GG_DISCONNECTING_ACK:
    {
        debug("GG_DISCONNECTING_ACK");
        break;
    }
    default:
    {
        debug("Unknown packet:") << "0x" + QString::number(packetType, 16).toUpper();
    }
    }
}

void GGProtocol::parseXmlList(const QByteArray &list)
{
    Database::getInstance()->clearContacts();
    emit newContactList();

    Contact* c = 0;

    QXmlStreamReader xml(list);
    while(xml.atEnd() == false && xml.hasError() == false)
    {
        QXmlStreamReader::TokenType token = xml.readNext();

        if(token == QXmlStreamReader::StartDocument)
        {
            continue;
        }

        if(token == QXmlStreamReader::StartElement)
        {
            if(xml.name() == "Group")
            {
                QString name;
                QString id;
                bool expanded = false;

                xml.readNext();

                while(!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == "Group"))
                {
                    if(xml.tokenType() == QXmlStreamReader::StartElement)
                    {
                        if(xml.name() == "Id")
                        {
                            xml.readNext();
                            id = xml.text().toString();
                            continue;
                        }

                        if(xml.name() == "Name")
                        {
                            xml.readNext();
                            name = xml.text().toString();
                            continue;
                        }

                        if(xml.name() == "IsExpanded")
                        {
                            xml.readNext();

                            if(xml.text().toString() == "true")
                            {
                                expanded = true;
                            }
                            continue;
                        }
                    }
                    xml.readNext();
                }

                c = new Contact(name, id);
                if(expanded == false)
                {
                    c->setFlag(Contact::FLAG_HIDDEN);
                }

                if(Database::getInstance()->addContact(c))
                {
                    c->add();
                }
                else
                {
                    delete c;
                }
            }
            else if(xml.name() == "Contact")
            {
                QString name;
                QString groupId;
                unsigned int uin;

                xml.readNext();

                while(!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == "Contact"))
                {
                    if(xml.tokenType() == QXmlStreamReader::StartElement)
                    {
                        if(xml.name() == "Groups")
                        {
                            xml.readNext();

                            while(!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == "Groups"))
                            {
                                if(xml.tokenType() == QXmlStreamReader::StartElement)
                                {
                                    if(xml.name() == "GroupId")
                                    {
                                        xml.readNext();
                                        groupId = xml.text().toString();
                                        continue;
                                    }
                                }
                                xml.readNext();
                            }
                            continue;
                        }

                        if(xml.name() == "ShowName")
                        {
                            xml.readNext();
                            name = xml.text().toString();
                            continue;
                        }

                        if(xml.name() == "GGNumber")
                        {
                            xml.readNext();
                            uin = xml.text().toString().toInt();
                            continue;
                        }
                    }
                    xml.readNext();
                }

                c = new Contact(uin, groupId);
                c->setName(name);

                if(Database::getInstance()->addContact(c))
                {
                    c->add();
                }
                else
                {
                    delete c;
                }
            }
        }
    }

    foreach(Contact* g, Contact::getContacts())
    {
        if(g->isGroup())
        {
            if(g->isFlagSet(Contact::FLAG_HIDDEN))
                g->hide();
        }
    }

    sendNotifyInfo();
    Database::getInstance()->updateContacts();

    emit importFinished();
}

void GGProtocol::welcome(GGStream& s)
{
    QByteArray packet;
    GGStream stream(&packet);

    QByteArray pass;
    {
        unsigned char seed[4] = { 0 };
        QCryptographicHash sha1(QCryptographicHash::Sha1);

        s.readRawData((char*)seed, 4);
        pass += Database::getInstance()->getGGPassword();
        pass.append((const char*)seed, 4);

        sha1.addData(pass);
        pass = sha1.result();
    }

    ownUin = Database::getInstance()->getGGNumber();

#ifndef GG11
    const QString& desc = Database::getInstance()->getLastDescription();
    unsigned int st = toStatus(currentStatus, false);

    if(desc.length() > 0)
    {
        st |= 0x4000;
    }

    if(Database::getInstance()->getFriendsOnly())
    {
        st |= 0x8000;
    }

    stream << (unsigned int)ownUin;
    stream.appendString("pl");
    stream << (unsigned char)0x02; // sha1
    stream.appendByteArray(pass);
    stream.appendAndRepeat(0x00, 64 - pass.length()); // pad with zeros
    stream << (unsigned int)st;
    stream << (unsigned int)GG_STATUS_FLAGS;
    stream << (unsigned int)GG_PROTO_FEATURES;
    stream << (unsigned int)0x00;
    stream << (unsigned short)0x00;
    stream << (unsigned int)0x00;
    stream << (unsigned short)0x00;
    stream << (unsigned char)0x00;
    stream << (unsigned char)0x64;
    stream.appendString(FULLNAME " (" OS_NAME ")", true);
    stream.appendString(desc, true);

#else

    QString uinStr = ownUin;
    QString caps = "StatusComments,gg_account_sdp,edisc,bot,fanpage,pubdir,botCaps";

    // language field marker
    stream << (unsigned char)0x0A;
    stream << (unsigned char)0x02;  // dunno
    stream.appendString("pl");      // 2-byte code
    // end language

    // id marker
    stream << (unsigned char)0x12;
    stream << (unsigned char)(uinStr.length() + 2);
    stream << (unsigned char)0x01; // uin/login

    stream.appendVariableValue(uinStr.length());
    stream.appendString(uinStr);
    // end id

    // password marker
    stream << (unsigned char)0x1A;
    // hash length
    stream.appendVariableValue(pass.length());
    stream.appendByteArray(pass);
    // end password

    // hash type marker
    stream << (unsigned char)0x20;
    stream << (unsigned char)0x02;
    // end hash type

    // flags marker
    stream << (unsigned char)0x2d;
    // flags, same as in presence in wtw
    stream << (unsigned char)0x77;
    stream << (unsigned char)0xff;
    stream << (unsigned char)0xae;
    stream << (unsigned char)0x01;
    // end flags

    // dunno marker
    stream << (unsigned char)0x35;
    stream << (unsigned char)0x14;
    stream << (unsigned char)0x00;
    stream << (unsigned char)0x03;
    stream << (unsigned char)0x00;
    // end

    // version marker
    stream << (unsigned char)0x3a;
    stream.appendVariableValue(6);
    stream.appendString("nanoGG");
    // end version

    // image size field marker
    stream << (unsigned char)0x60;
    stream << (unsigned char)0xFF;
    stream << (unsigned char)0x01;
    // end image size

    // init status marker
    stream << (unsigned char)0x45;
    // init status
    stream << (unsigned char)0x23;
    stream << (unsigned char)0x20;
    stream << (unsigned char)0x00;
    stream << (unsigned char)0x00;
    // end init status

    // init desc marker
    stream << (unsigned char)0x4a;
    stream << (unsigned char)0x00;
    // end init desc

    // capas marker
    stream << (unsigned char)0x5a;
    stream.appendVariableValue(caps.length());
    stream.appendString(caps);
    // caps
#endif

    sendPacket(GG_LOGIN, packet);
}

void GGProtocol::sendNotifyInfo()
{
    int cnt = 0;
    foreach(Contact* c, Contact::getContacts())
    {
        if(c->isGroup())
            continue;
        ++cnt;
    }

    if(cnt == 0)
    {
        sendPacket(GG_USERLIST_EMPTY, QByteArray());
    }
    else
    {
        unsigned int type = cnt > 400 ? GG_NOTIFY_FIRST : GG_NOTIFY_FIRST;
        int i = 0;

        QByteArray packet;
        GGStream stream(&packet, QIODevice::WriteOnly);

        foreach(Contact* c, Contact::getContacts())
        {
            if(c->isGroup())
            {
                continue;
            }

            if(cnt < 400)
            {
                type = GG_NOTIFY_LAST;
            }

            if(++i == 400)
            {
                sendPacket(type, packet);
                packet.clear();
                stream.device()->seek(0);
            }

            QString uin = c->uin;
            stream << (unsigned char)0x00;
            stream.appendVariableValue(uin.length());
            stream.appendString(uin);
            stream << (unsigned char)0x03;

            --cnt;
        }

        sendPacket(type, packet);
    }
}

void GGProtocol::loginOk(GGStream& /*s*/)
{
    emit connection(false);
    sendNotifyInfo();
}

void GGProtocol::loginFailed(GGStream& /*s*/)
{
    emit ownPresence(STATUS_UNAVAILABLE, Database::getInstance()->getLastDescription());
}

void GGProtocol::notify(GGStream& s)
{
    unsigned int uin;
    unsigned int status;
    unsigned int descLen;
    QString desc;

    while(!s.atEnd())
    {
        s >> uin;
        s >> status;
        s.foward(16); // Skip unused data
        s >> descLen;
        desc = s.readString(descLen);

        emit presence(QString::number(uin), fromStatus(status), desc);
    }

    ContactList::getInstance()->sort();
}

void GGProtocol::recvMsg(GGStream &s, bool own)
{
    unsigned char marker;

    QString uin;
    QString msg;
    unsigned int time = 0;

    while(!s.atEnd())
    {
        s >> marker;

        switch(marker)
        {
        case 0x0A:
        {
            unsigned char len;
            s >> len;
            s.foward(1);
            unsigned int uinLen = s.readVariableValue();

            uin = s.readString(uinLen);
            break;
        }
        case 0x10:
        {
            s.foward(2);

            unsigned int cnt = s.readVariableValue();

            QByteArray buf;
            GGStream stream(&buf, QIODevice::WriteOnly);

            stream << (unsigned char)0x08;
            stream << (unsigned char)0x01;

            stream << (unsigned char)0x10;
            stream.appendVariableValue(cnt);

            stream << (unsigned char)0x18;
            stream << (unsigned char)0x01;

            sendPacket(GG_RECV_MSG_ACK, buf);
            break;
        }
        case 0x25:
        {
            s >> time;
            break;
        }
        case 0x2A:
        {
            unsigned int len = s.readVariableValue();
            s.foward(len);
            break;
        }
        case 0x32:
        {
            unsigned int len = s.readVariableValue();
            msg = s.readString(len);
            break;
        }
        case 0x3A:
        {
            unsigned int len = s.readVariableValue();
            s.foward(len);
            break;
        }
        case 0x49:
        {
            s.foward(8);
            break;
        }
        case 0x59:
        {
            s.foward(8);
            break;
        }
        }
    }

    debug() << uin << msg << time;

    bool handled = false;
    emit message(uin, time, msg, handled, own);

    if(!handled && own == false)
    {
        debug("message from unknown contact, adding to list");
        /*emit newContact(uin);
        QByteArray buf;
        GGStream stream(&buf, QIODevice::WriteOnly);

        stream << uin;
        stream << (unsigned char)0x03;

        sendPacket(GG_ADD_NOTIFY, buf);

        emit message(uin, time, msg, handled, false);*/
    }
    else
    {
        debug() << "message from" << uin;
    }
}

void GGProtocol::contactListReply(GGStream &s)
{
    unsigned char type;
    s >> type;

    buffer += s.read(data.length() - 1);

    if(type == GG_CONTACTS_LIST_REPLY_GET)
    {
        QByteArray list;
        list.resize(5 * buffer.size());

        unsigned long destLen = list.length();
        int ret = Z_ERRNO;

        do
        {
            ret = ::uncompress((Bytef*)list.data(), &destLen, (const Bytef*)buffer.constData(), buffer.size());

            if(ret == Z_BUF_ERROR)
            {
                list.resize(list.size() * 2);
                destLen = list.length();
            }
            else
            {
                break;
            }
        }
        while(true);

        if(ret == Z_OK)
        {
            list.resize(destLen);
            parseXmlList(list);
        }
        else
        {
            debug() << "corrupted contacts list on server...";
        }
        buffer.clear();
    }
}

void GGProtocol::typingNotify(GGStream &s)
{
    unsigned short count;
    unsigned int uin;

    s >> count;
    s >> uin;

    emit typingNotify(QString::number(uin), count);
}

void GGProtocol::newList(GGStream &/*s*/)
{
    importContacts();
}

void GGProtocol::sendPacket(unsigned int type, const QByteArray& packet)
{
    if(!socket || currentStatus == STATUS_UNAVAILABLE)
    {
        return;
    }

    QByteArray buf;
    GGStream s(&buf, QIODevice::WriteOnly);
    s << type;
    s << (unsigned int)packet.length();

    buf += packet;

    emit outgoingPacket(type, packet);
    {
        printPacket(packet, false, type, packet.length());
    }
    socket->write(buf);
}

void GGProtocol::onTimer()
{
    sendPacket(GG_PING, QByteArray());
}
