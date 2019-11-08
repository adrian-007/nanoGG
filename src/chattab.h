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

#ifndef CHATTAB_H
#define CHATTAB_H

#include <QWidget>
#include <QTime>

class Contact;

namespace Ui {
class ChatTab;
}

class ChatTab : public QWidget
{
    Q_OBJECT
    
public:
    explicit ChatTab(Contact* c, QWidget *parent = 0);
    ~ChatTab();
    
    static void cleanup();

    inline void ownMessage(const QString& msg) {
        message("", msg, QTime::currentTime());
    }

    void message(const QString& nick, const QString& msg, const QTime& time);
    void status(const QString& msg);

    void setHeader(const QString& text);
    void pageActive();

    const Contact* getContact() const { return contact; }

private slots:
    void onTimer();
    void onWebViewContentSizeChanged(const QSize& s);
    void on_send_clicked();

private:
    void appendView(QString str, bool scroll);

    Ui::ChatTab *ui;

    Contact* contact;

    int lastTypingLen;
    static QTimer* getTimer();
    static QTimer* __timer;
};

#endif // CHATTAB_H
