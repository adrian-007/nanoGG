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

#ifndef CONTACTLIST_H
#define CONTACTLIST_H

#include <QWidget>
#include <QGridLayout>

#include <QListView>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QStyledItemDelegate>

#include "flickcharm.h"

class ContactList : public QWidget {
    Q_OBJECT
public:
    explicit ContactList(QWidget *parent = 0);

    static void newInstance(QWidget* parent) {
        if(__instance)
            delete __instance;
        __instance = new ContactList(parent);
    }

    static void deleteInstance() {
        if(__instance) {
            delete __instance;
            __instance = 0;
        }
    }

    static ContactList* getInstance() {
        return __instance;
    }

    inline void sort() {
        model.sort(0);
    }

    QStandardItemModel& getModel() { return model; }

    void setVisible(bool visible);

signals:

public slots:
    void onActivated(const QModelIndex& index);
    void onClicked(const QModelIndex& index);

private:
    class PaintDelegate : public QStyledItemDelegate {
    public:
        PaintDelegate(QWidget* parent = 0);
        void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
        QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
    };

    QGridLayout mainLayout;
    QListView list;
    QStandardItemModel model;

    static ContactList* __instance;
};

#endif // CONTACTLIST_H
