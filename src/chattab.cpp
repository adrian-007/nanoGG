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

#include "chattab.h"
#include "ui_chattab.h"
#include "contact.h"
#include "ggprotocol.h"
#include "FlickCharmGlobal.h"

#include <QtWebKit/QWebFrame>
#include <QTimer>

QTimer* ChatTab::__timer = 0;

QTimer* ChatTab::getTimer() {
    if(__timer == 0) {
        __timer = new QTimer;
        __timer->start(1500);
    }
    return __timer;
}

void ChatTab::cleanup() {
    if(__timer) {
        delete __timer;
        __timer = 0;
    }
}

ChatTab::ChatTab(Contact* c, QWidget *parent) : QWidget(parent), contact(c), lastTypingLen(0), ui(new Ui::ChatTab)
{
    ui->setupUi(this);

    ui->webView->setHtml("<html><head><link rel=\"stylesheet\" href=\"qrc:/res/chat.css\"></head><body></body></html>");
    connect(getTimer(), SIGNAL(timeout()), SLOT(onTimer()));
    connect(ui->webView->page()->mainFrame(), SIGNAL(contentsSizeChanged(QSize)), SLOT(onWebViewContentSizeChanged(QSize)));

    QPixmap img = contact->getAvatar();
    if(img.isNull() == false)
    {
        ui->avatar->setPixmap(img);
    }

    FlickCharmGlobal::getInstance()->activateOn(ui->webView);
}

ChatTab::~ChatTab()
{
    delete ui;
}

void ChatTab::onTimer()
{
    int len = ui->input->toPlainText().length();

    if(lastTypingLen == len)
        return;

    if(len > 0 && lastTypingLen == 0)
    {
        len = 1;
    }

    GGProtocol::getInstance()->sendTypingNotify(contact->uin, (unsigned short)len);

    lastTypingLen = len;
}

void ChatTab::setHeader(const QString &text)
{
    if(text.isEmpty())
    {
        ui->description->clear();
    }
    else
    {
        QString s = text;
        s.replace("\n", " ");

        if(s.length() > 80) {
            s = s.left(80) + "...";
        }

        ui->description->setText(s);
    }
}

void ChatTab::message(const QString &nick, const QString &msg, const QTime &time)
{
    const bool own = nick.isEmpty();

    QString str = QString("<div class=\"%1\"><span class=\"timestamp\">%2</span> <span class=\"%3\">%4:</span> %5</div>")
            .arg(own ? "msgOut" : "msgIn", time.toString("[hh:mm:ss]"), own ? "myNick" : "userNick", own ? "ja" : Qt::escape(nick), QString(msg).remove(QRegExp("<[^>]{3,}>")));

    qDebug() << qPrintable(str);

    appendView(str, true);
}

void ChatTab::status(const QString &msg)
{
    QString str = QString("<div class=\"status\"><span class=\"timestamp\">%1</span> <i>%2</i>: %3</div>")
            .arg(QTime::currentTime().toString("[hh:mm:ss]"), Contact::getStatusName(contact->status), QString(msg).remove(QRegExp("<[^>]*>")));

    appendView(str, true);
}

void ChatTab::pageActive()
{
    contact->unsetFlag(Contact::FLAG_UNREAD_MESSAGE);
    contact->refreshPresence();
}

void ChatTab::on_send_clicked()
{
    const QString& msg = ui->input->toPlainText();
    if(msg.isEmpty() == false)
    {
        ownMessage(msg);
        ui->input->clear();

        GGProtocol::getInstance()->sendMessage(contact->uin, msg);
    }
}

void ChatTab::onWebViewContentSizeChanged(const QSize &s)
{
    ui->webView->page()->mainFrame()->scroll(s.width(), s.height());
}

void ChatTab::appendView(QString str, bool /*scroll*/)
{
    ui->webView->page()->mainFrame()->evaluateJavaScript(QString("document.body.innerHTML += '%1';").arg(str.replace("\n", "<br/>")));
}
