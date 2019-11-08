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

#include "conversationpage.h"
#include "contact.h"
#include "ggprotocol.h"
#include "debug.h"

#include <QTimer>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QScrollBar>

QTimer* ConversationPage::__timer = 0;

QTimer* ConversationPage::getTimer() {
    if(__timer == 0) {
        __timer = new QTimer;
        __timer->start(1500);
    }
    return __timer;
}

void ConversationPage::cleanup() {
    if(__timer) {
        delete __timer;
        __timer = 0;
    }
}

ConversationPage::ConversationPage(Contact* parent) : QWidget(0), contact(parent), lastTypingLen(0) {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    QHBoxLayout* row;

    content = new QTextEdit(this);
    line = new QLineEdit(this);
    header = new QLabel(this);
    avatar = new QLabel(this);
    sendButton = new QPushButton(QPixmap(":/res/send.png"), "Wyślij", this);
    clrButton = new QPushButton(QPixmap(":/res/clear.png"), "Wyczyść", this);

    connect(line, SIGNAL(returnPressed()), SLOT(onEnter()));
    connect(content->verticalScrollBar(), SIGNAL(valueChanged(int)), SLOT(onScrollChange(int)));
    connect(content->document(), SIGNAL(cursorPositionChanged(QTextCursor)), SLOT(onCursorChange(QTextCursor)));
    connect(getTimer(), SIGNAL(timeout()), SLOT(onTimer()));
    connect(sendButton, SIGNAL(clicked()), SLOT(onEnter()));
    connect(clrButton, SIGNAL(clicked()), content, SLOT(clear()));

    content->setReadOnly(true);

    header->setMaximumHeight(32);
    header->setWordWrap(true);
    header->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    avatar->setAlignment(Qt::AlignRight);

    avatar->setFixedSize(32, 32);
    avatar->setScaledContents(true);
    avatar->setAlignment(Qt::AlignLeft);

    sendButton->setFixedHeight(20);
    clrButton->setFixedHeight(20);
    line->setFixedHeight(20);

    QPixmap img = contact->getAvatar();
    if(img.isNull() == false) {
        avatar->setPixmap(img);
    } else {
        avatar->setVisible(false);
    }

    row = new QHBoxLayout(this);
    row->setSizeConstraint(QLayout::SetMaximumSize);
    row->addWidget(avatar, 0, Qt::AlignLeft);
    row->addWidget(header, 1, Qt::AlignRight);
    mainLayout->addLayout(row);

    mainLayout->addWidget(content);

    row = new QHBoxLayout(this);

    mainLayout->addWidget(line);

    row->addWidget(sendButton);
    row->addWidget(clrButton);

    mainLayout->addLayout(row);

    this->setLayout(mainLayout);
}

ConversationPage::~ConversationPage()
{
}

void ConversationPage::setVisible(bool visible) {
    if(visible) {
        line->setFocus();
    } else {
        line->clearFocus();
    }

    QWidget::setVisible(visible);
}

void ConversationPage::onEnter() {
    if(line->text().isEmpty() == false) {
        const QString& msg = line->text();
        ownMessage(msg);
        line->clear();

        GGProtocol::getInstance()->sendMessage(contact->uin, msg);
    }
}

void ConversationPage::onTimer() {
    int len = line->text().length();

    if(lastTypingLen == len)
        return;

    if(len > 0 && lastTypingLen == 0) {
        len = 1;
    }

    GGProtocol::getInstance()->sendTypingNotify(contact->uin, (unsigned short)len);

    lastTypingLen = len;
}

void ConversationPage::onScrollChange(int value) {
    if(value == content->verticalScrollBar()->maximum() || value == 0) {
        line->setFocus();
    }
}

void ConversationPage::onCursorChange(const QTextCursor& /*c*/) {
}

void ConversationPage::setHeader(const QString &text) {
    if(text.isEmpty()) {
        if(avatar->pixmap() == 0 || avatar->pixmap()->isNull())
            header->setVisible(false);
        else
            header->setText("");
    } else {
        QString s = text;
        s.replace("\n", " ");

        if(s.length() > 80) {
            s = s.left(80) + "...";
        }

        header->setText(s);
        header->setVisible(true);
    }
}

void ConversationPage::message(const QString &nick, const QString &msg, const QTime &time) {
    QString str = QString("<span style=\"color: #606060;\">%1</span> <b>%2</b>: %3")
            .arg(time.toString("[hh:mm:ss]"))
            .arg(nick.isEmpty() ? "ja" : Qt::escape(nick))
            .arg(msg);

    content->append(str);

    QTextCursor c =  content->textCursor();
    c.movePosition(QTextCursor::End);
    content->setTextCursor(c);
    content->ensureCursorVisible();
}

void ConversationPage::status(const QString &msg) {
    QString str = QString("<span style=\"color: #606060;\">%1</span> <i><b>%2</b> %3</i>")
            .arg(QTime::currentTime().toString("[hh:mm:ss]"))
            .arg(Contact::getStatusName(contact->status))
            .arg(msg);

    content->append(str);

    QTextCursor c =  content->textCursor();
    c.movePosition(QTextCursor::End);
    content->setTextCursor(c);
    content->ensureCursorVisible();
}

void ConversationPage::pageActive() {
    contact->unsetFlag(Contact::FLAG_UNREAD_MESSAGE);
    contact->refreshPresence();
}
