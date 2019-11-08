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
#include "ui_mainwindow.h"
#include "contact.h"
#include "conversationpage.h"
#include "database.h"
#include "ggprotocol.h"
#include "contactlist.h"
#include "notify.h"
#include "version.h"
#include "debug.h"
#include "FlickCharmGlobal.h"

#include <QtCore/QCoreApplication>
#include <QActionGroup>
#include <QStandardItemModel>
#include <QtXml/QXmlStreamReader>
#include <QStatusBar>
#include <QFile>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    Notify::init();
    FlickCharmGlobal::newInstance();

    Database::newInstance();
    GGProtocol::newInstance();

    ui->setupUi(this);
    this->setWindowTitle(FULLNAME);

#if defined(Q_OS_SYMBIAN)
    QAction *closeTabAction = new QAction(QString("Zamknij kartę"), this);
    closeTabAction->setSoftKeyRole(QAction::NegativeSoftKey);

    connect(closeTabAction, SIGNAL(triggered()), SLOT(on_actionZamknijKarte_triggered()));
    addAction(closeTabAction);
#endif

    //QRegExpValidator* v = new QRegExpValidator();
    //v->setRegExp(QRegExp("[0-9]{,48}"));
    //ui->ggNumber->setValidator(v);

    QActionGroup* g = new QActionGroup(ui->menuStatus);
    g->setExclusive(true);
    ui->actionDostepny->setActionGroup(g);
    ui->actionPorozmawiajmy->setActionGroup(g);
    ui->actionJestem_zajety->setActionGroup(g);
    ui->actionZaraz_wracam->setActionGroup(g);
    ui->actionNiewidoczny->setActionGroup(g);
    ui->actionNiedostepny->setActionGroup(g);

    connect(GGProtocol::getInstance(), SIGNAL(newContact(QString)), SLOT(onNewContact(QString)));
    connect(GGProtocol::getInstance(), SIGNAL(importFinished()), SLOT(onImportFinished()));
    connect(GGProtocol::getInstance(), SIGNAL(ownPresence(uint,QString)), SLOT(onOwnPresence(uint,QString)));
    connect(GGProtocol::getInstance(), SIGNAL(newContactList()), SLOT(onNewContactList()));

    ContactList::newInstance(this);
    ui->tabs->insertTab(1, ContactList::getInstance(), QPixmap(":/res/contacts.png"), "Kontakty");

    connect(Database::getInstance(), SIGNAL(reloadData()), SLOT(onReloadData()));

    getConfig();
}

MainWindow::~MainWindow()
{
    setStatus(GGProtocol::STATUS_UNAVAILABLE);

    ContactList::deleteInstance();
    GGProtocol::deleteInstance();

    setConfig();

    Database::deleteInstance();
    ConversationPage::cleanup();

    Notify::cleanup();
    FlickCharmGlobal::deleteInstance();

    delete ui;
}

void MainWindow::setOrientation(ScreenOrientation orientation)
{
#if defined(Q_OS_SYMBIAN)
    // If the version of Qt on the device is < 4.7.2, that attribute won't work
    if (orientation != ScreenOrientationAuto)
    {
        const QStringList v = QString::fromAscii(qVersion()).split(QLatin1Char('.'));
        if (v.count() == 3 && (v.at(0).toInt() << 16 | v.at(1).toInt() << 8 | v.at(2).toInt()) < 0x040702)
        {
            qWarning("Screen orientation locking only supported with Qt 4.7.2 and above");
            return;
        }
    }
#endif // Q_OS_SYMBIAN

    Qt::WidgetAttribute attribute;
    switch (orientation)
    {
#if QT_VERSION < 0x040702
    // Qt < 4.7.2 does not yet have the Qt::WA_*Orientation attributes
    case ScreenOrientationLockPortrait:
        attribute = static_cast<Qt::WidgetAttribute>(128);
        break;
    case ScreenOrientationLockLandscape:
        attribute = static_cast<Qt::WidgetAttribute>(129);
        break;
    default:
    case ScreenOrientationAuto:
        attribute = static_cast<Qt::WidgetAttribute>(130);
        break;
#else // QT_VERSION < 0x040702
    case ScreenOrientationLockPortrait:
        attribute = Qt::WA_LockPortraitOrientation;
        break;
    case ScreenOrientationLockLandscape:
        attribute = Qt::WA_LockLandscapeOrientation;
        break;
    default:
    case ScreenOrientationAuto:
        attribute = Qt::WA_AutoOrientation;
        break;
#endif // QT_VERSION < 0x040702
    };
    setAttribute(attribute, true);
}

void MainWindow::showExpanded()
{
#if defined(Q_WS_MAEMO_5) || defined(Q_WS_MAEMO_6) || defined(Q_OS_SYMBIAN)
    showMaximized();
#else
    show();
#endif
    Database::getInstance()->loacContacts(this);
}

void MainWindow::getConfig()
{
    ui->ggNumber->setText(Database::getInstance()->getGGNumber());
    ui->ggPassword->setText(Database::getInstance()->getGGPassword());
    ui->ggDescription->setPlainText(Database::getInstance()->getLastDescription());
    ui->actionTylko_dla_znajomych->setChecked(Database::getInstance()->getFriendsOnly());
}

void MainWindow::setConfig()
{
    Database::getInstance()->setGGNumber(ui->ggNumber->text());
    Database::getInstance()->setGGPassword(ui->ggPassword->text());
    Database::getInstance()->setLastDescription(ui->ggDescription->toPlainText());
    Database::getInstance()->setFriendsOnly(ui->actionTylko_dla_znajomych->isChecked());
}

void MainWindow::onReloadData()
{
    getConfig();
}

void MainWindow::onOwnPresence(unsigned int status, const QString &desc)
{
    ui->ggDescription->setPlainText(desc);

    switch(status) {
    case GGProtocol::STATUS_AVAILABLE:
    {
        ui->actionDostepny->setChecked(true);
        break;
    }
    case GGProtocol::STATUS_BUSY:
    {
        ui->actionZaraz_wracam->setChecked(true);
        break;
    }
    case GGProtocol::STATUS_DND:
    {
        ui->actionJestem_zajety->setChecked(true);
        break;
    }
    case GGProtocol::STATUS_FFC:
    {
        ui->actionPorozmawiajmy->setChecked(true);
        break;
    }
    case GGProtocol::STATUS_INVISIBLE:
    {
        ui->actionNiewidoczny->setChecked(true);
        break;
    }
    default:
    {
        ui->actionNiedostepny->setChecked(true);
    }
    }

    QString path = Contact::getStatusImagePath(status);
    QPixmap img(path);

    ui->tabs->setTabIcon(0, img);
    this->setWindowIcon(img);
}

void MainWindow::onStatusUpdate(Contact* c, const QPixmap& icon)
{
    if(c->hasPage() == false)
    {
        return;
    }

    int index = ui->tabs->indexOf(c->getPage());

    if(index > 0)
    {
        ui->tabs->setTabIcon(index, icon);
    }
}

void MainWindow::onNewMessage(Contact* c, bool forceShow)
{
    int index = ui->tabs->indexOf(c->getPage());

    if(index == -1)
    {
        index = ui->tabs->addTab(c->getPage(), c->getName());
        c->setFlag(Contact::FLAG_PAGE_VISIBLE);
    }
    else
    {
        if(ui->tabs->currentWidget() == (QWidget*)c->getPage())
        {
            c->getPage()->pageActive();
        }
    }

    if(forceShow)
    {
        ui->tabs->setCurrentIndex(index);
    }
}

void MainWindow::onNewContact(const QString& uin)
{
    Contact* c = new Contact(uin, QString(""));
    c->setName(uin);
    c->group = Contact::getDefaultGroup()->group;

    connect(c, SIGNAL(statusUpdate(Contact*,QPixmap)), SLOT(onStatusUpdate(Contact*,QPixmap)));
    connect(c, SIGNAL(newMessage(Contact*,bool)), SLOT(onNewMessage(Contact*,bool)));

    c->add();
}

void MainWindow::onImportFinished()
{
    foreach(Contact* c, Contact::getContacts())
    {
        if(c->isGroup())
        {
            continue;
        }

        connect(c, SIGNAL(statusUpdate(Contact*,QPixmap)), SLOT(onStatusUpdate(Contact*,QPixmap)));
        connect(c, SIGNAL(newMessage(Contact*,bool)), SLOT(onNewMessage(Contact*,bool)));
    }
}

void MainWindow::on_tabs_currentChanged(int index)
{
    if(index > 0)
    {
        ui->actionZamknijKarte->setVisible(true);

        QWidget* w = ui->tabs->currentWidget();

        ConversationPage* page = dynamic_cast<ConversationPage*>(w);

        if(page)
        {
            page->pageActive();
        }
    }
    else
    {
        ui->actionZamknijKarte->setVisible(false);
    }
}

void MainWindow::on_actionZamknijKarte_triggered()
{
    if(ui->tabs->currentIndex() < 1)
    {
        return;
    }

    QWidget* w = ui->tabs->currentWidget();

    foreach(Contact* c, Contact::getContacts())
    {
        if(c->getPage() == w)
        {
            c->unsetFlag(Contact::FLAG_PAGE_VISIBLE);
            break;
        }
    }

    ui->tabs->removeTab(ui->tabs->currentIndex());
}

void MainWindow::on_actionKontakty_triggered()
{
    int index = ui->tabs->indexOf(ContactList::getInstance());

    if(index < 0)
    {
        ui->tabs->insertTab(1, ContactList::getInstance(), QPixmap(":/res/contacts.png"), "Kontakty");
        index = 1;
    }

    ui->tabs->setCurrentIndex(index);
}

void MainWindow::on_actionTylko_dla_znajomych_triggered()
{
    Database::getInstance()->setFriendsOnly(ui->actionTylko_dla_znajomych->isChecked());
}

void MainWindow::on_pushButtonUstawOpis_clicked()
{
    if(GGProtocol::getInstance()->isConnected() == false)
    {
        setConfig();
    }
    else
    {
        Database::getInstance()->setLastDescription(ui->ggDescription->toPlainText());
    }

    GGProtocol::getInstance()->updateStatus();
}

void MainWindow::on_actionDostepny_triggered()
{
    setStatus(GGProtocol::STATUS_AVAILABLE);
}

void MainWindow::on_actionPorozmawiajmy_triggered()
{
    setStatus(GGProtocol::STATUS_FFC);
}

void MainWindow::on_actionJestem_zajety_triggered()
{
    setStatus(GGProtocol::STATUS_DND);
}

void MainWindow::on_actionZaraz_wracam_triggered()
{
    setStatus(GGProtocol::STATUS_BUSY);
}

void MainWindow::on_actionNiewidoczny_triggered()
{
    setStatus(GGProtocol::STATUS_INVISIBLE);
}

void MainWindow::on_actionNiedostepny_triggered()
{
    setStatus(GGProtocol::STATUS_UNAVAILABLE);
}

void MainWindow::setStatus(unsigned int status)
{
    if(GGProtocol::getInstance()->isConnected() == false)
    {
        setConfig();
        Notify::setEnabled(true);
    }
    else
    {
        if(status == GGProtocol::STATUS_UNAVAILABLE)
        {
            Notify::setEnabled(false);
        }
    }

    GGProtocol::getInstance()->setStatus(status);
}

void MainWindow::on_actionO_programie_triggered()
{
    QString title = FULLNAME " (2011-2012)";
    QString msg;
    msg += APPNAME "\tv" VERSIONSTRING "\n\n";
    msg += "Autor\tAdrian Guzowski (adrian_007)\n";
    msg += "Ikony\tPavel Kiselev (pk)\n";

    QMessageBox::about(this, title, msg);
}

void MainWindow::on_actionImport_listy_triggered()
{
    if(GGProtocol::getInstance()->isConnected() == false)
    {
        QMessageBox::warning(this, "Import listy", "Musisz połączyć się z serwerem GG aby importować listę!");
        return;
    }

    GGProtocol::getInstance()->importContacts();
}

void MainWindow::onNewContactList()
{
    foreach(Contact* c, Contact::getContacts())
    {
        if(c->hasPage())
        {
            if(c->isFlagSet(Contact::FLAG_PAGE_VISIBLE))
            {
                ui->tabs->removeTab(ui->tabs->indexOf(c->getPage()));
            }
        }
    }

    ContactList::getInstance()->getModel().clear();
    Contact::getContacts().clear();
}

void MainWindow::on_actionUstawienia_domyslne_triggered()
{
    if(GGProtocol::getInstance()->isConnected())
    {
        QMessageBox::warning(this, "Ustawienia domyślne", "Aby przywrócic ustawienia 'fabryczne' wypadałoby zakończyć istniejące połączenie...");
        return;
    }

    onNewContactList();
    Database::getInstance()->resetToDefaults();
}

void MainWindow::on_actionPobierz_avatary_triggered()
{
    GGProtocol::getInstance()->getNextAvatarReceiver();
}
