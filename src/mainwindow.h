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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QModelIndex>

#include "version.h"

namespace Ui {
    class MainWindow;
}

class Contact;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    enum ScreenOrientation {
        ScreenOrientationLockPortrait,
        ScreenOrientationLockLandscape,
        ScreenOrientationAuto
    };

    explicit MainWindow(QWidget *parent = 0);
    virtual ~MainWindow();

    void setOrientation(ScreenOrientation orientation);
    void showExpanded();

    void readXmlList();

    void getConfig();
    void setConfig();

private slots:
    void onNewContactList();
    void onNewContact(const QString& uin);
    void onStatusUpdate(Contact* c, const QPixmap& icon);
    void onNewMessage(Contact* c, bool forceShow);
    void onReloadData();
    void onImportFinished();
    void onOwnPresence(unsigned int status, const QString& desc);

    void on_tabs_currentChanged(int index);
    void on_pushButtonUstawOpis_clicked();
    void on_actionZamknijKarte_triggered();
    void on_actionTylko_dla_znajomych_triggered();
    void on_actionDostepny_triggered();
    void on_actionPorozmawiajmy_triggered();
    void on_actionJestem_zajety_triggered();
    void on_actionZaraz_wracam_triggered();
    void on_actionNiewidoczny_triggered();
    void on_actionNiedostepny_triggered();
    void on_actionO_programie_triggered();
    void on_actionKontakty_triggered();
    void on_actionImport_listy_triggered();
    void on_actionUstawienia_domyslne_triggered();
    void on_actionPobierz_avatary_triggered();

private:
    void setStatus(unsigned int status);

    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
