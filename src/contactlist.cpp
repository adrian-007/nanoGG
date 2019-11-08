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

#include "contactlist.h"
#include "contact.h"
#include "debug.h"
#include "FlickCharmGlobal.h"

#include <QPainter>
#include <QFontMetrics>
#include <QAction>

ContactList* ContactList::__instance = 0;

ContactList::PaintDelegate::PaintDelegate(QWidget* parent /*= 0*/) : QStyledItemDelegate(parent) {
}

void ContactList::PaintDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    QVariant tmp = index.data(Qt::UserRole + 2);
    if(tmp.isNull() || tmp.isValid() == false) {
        QStyledItemDelegate::paint(painter, option, index);
        return;
    }

    QString desc = index.data(Qt::UserRole + 1).value<QString>();
    QRect rc = option.rect;

    QBrush bgBrush;
    QBrush textBrush;

    if (option.state & QStyle::State_Selected) {
        bgBrush = option.palette.highlight();
        textBrush = option.palette.highlightedText();
    } else {
        bgBrush = option.palette.background();
        textBrush = option.palette.text();
    }

    painter->save();
    painter->fillRect(rc, bgBrush);
    painter->setPen(textBrush.color());

    rc.adjust(1, 1, -1, -1);

    painter->drawPixmap(rc.x(), rc.y(), 16, 16, index.data(Qt::UserRole + 2).value<QPixmap>());

    bool hasAvatar = index.data(Qt::UserRole + 3).isValid();
    if(hasAvatar) {
        painter->drawPixmap(rc.x() + rc.width() - 32, rc.y(), 32, 32, index.data(Qt::UserRole + 3).value<QPixmap>());
    }

    rc.adjust(20, 0, (hasAvatar ? -40 : -8), 0);

    QFont font(option.font);
    font.setPointSize(font.pointSize() - 1);

    font.setBold(true);
    painter->setFont(font);
    painter->drawText(rc, Qt::TextSingleLine, index.data(Qt::DisplayRole).value<QString>());

    if(desc.isEmpty() == false) {
        rc.adjust(0, 16, 0, 0);
        font.setBold(false);

        painter->setFont(font);
        painter->drawText(rc, Qt::TextWrapAnywhere | Qt::AlignJustify, desc);
    }

    painter->restore();
}

QSize ContactList::PaintDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const {
    QVariant tmp = index.data(Qt::UserRole + 2);
    if(tmp.isNull() || tmp.isValid() == false) {
        QSize hint = QStyledItemDelegate::sizeHint(option, index);
        hint.setHeight(32);
        return hint;
    }

    QRect rc;
    QSize s;

    QString desc = index.data(Qt::UserRole + 1).value<QString>();

    bool hasAvatar = index.data(Qt::UserRole + 3).isValid();

    if(desc.isEmpty() == false) {
        rc = QRect(20 + 1, 16 + 1, option.rect.width() - 20 - (hasAvatar ? 40 : 8), option.rect.height());
        QFont font(option.font);
        font.setPointSize(font.pointSize() - 1);

        QFontMetrics fm(font);
        rc = fm.boundingRect(rc, Qt::TextWrapAnywhere | Qt::AlignJustify, desc);
        rc.adjust(-20, -16, (hasAvatar ? 32 : 0), 3);
    } else {
        s = QStyledItemDelegate::sizeHint(option, index);
        rc.adjust(1, 1, s.width() - 40, s.height());
    }

    s = rc.size();

    if(s.height() <= 32) {
        s.setHeight(32);
    }

    s.setHeight(s.height() + 2);
    return s;
}

ContactList::ContactList(QWidget *parent) : QWidget(parent), mainLayout(this), list(this) {
    mainLayout.setContentsMargins(0, 7, 0, 0);

    this->setLayout(&mainLayout);
    this->mainLayout.addWidget(&list);

    this->list.setModel(&this->model);
    this->list.setItemDelegate(new PaintDelegate(this));
    this->list.setWordWrap(true);
    this->list.setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    this->list.setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);

    connect(&this->list, SIGNAL(activated(QModelIndex)), SLOT(onActivated(QModelIndex)));

    FlickCharmGlobal::getInstance()->activateOn(&list);

/*#if defined(Q_OS_SYMBIAN)
    QAction *closeTabAction = new QAction(QString("Kontakty"), this);
    closeTabAction->setSoftKeyRole(QAction::NegativeSoftKey);

    connect(closeTabAction, SIGNAL(triggered()), SLOT(onCloseTabActionTriggered()));
    addAction(closeTabAction);

#endif*/

}

void ContactList::setVisible(bool visible) {
    if(visible) {
        list.setFocus();
        list.activateWindow();
    } else {
        list.clearFocus();
    }
    QWidget::setVisible(visible);
}

void ContactList::onActivated(const QModelIndex &index) {
    QStandardItem* item = this->model.itemFromIndex(index);
    if(item) {
        Contact* cnt = dynamic_cast<Contact*>(item);
        if(cnt) {
            if(cnt->isGroup()) {
                if(cnt->isFlagSet(Contact::FLAG_HIDDEN)) {
                    cnt->show();
                } else {
                    cnt->hide();
                }
            } else {
                cnt->createPage(true);
            }
        }
    }
}

void ContactList::onClicked(const QModelIndex &index) {
    QStandardItem* item = this->model.itemFromIndex(index);
    if(item) {
        Contact* cnt = dynamic_cast<Contact*>(item);
        if(cnt) {
            if(cnt->isGroup()) {
                if(cnt->isFlagSet(Contact::FLAG_HIDDEN)) {
                    cnt->show();
                } else {
                    cnt->hide();
                }
            }
        }
    }
}
