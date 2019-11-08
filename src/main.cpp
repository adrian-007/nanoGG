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

#include "mainwindow.h"
#include "debug.h"

#include <QtGui/QApplication>
#include <QWindowsStyle>
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QTextCodec>

#if defined(Q_OS_SYMBIAN)
void loadStyleSheet(QApplication& app) {
    QFile data(":/res/default.css");
    QString style;

    if(data.open(QFile::ReadOnly)) {
        QTextStream styleIn(&data);
        style = styleIn.readAll();
        data.close();
        app.setStyleSheet(style);
    }
}
#endif

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

#if defined(Q_OS_SYMBIAN)
    app.setStyle(new QWindowsStyle);
    app.setFont(QFont("Tahoma", 5));
    loadStyleSheet(app);
#endif

    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));

    MainWindow mainWindow;
    mainWindow.setOrientation(MainWindow::ScreenOrientationAuto);
    mainWindow.showExpanded();

    return app.exec();
}
