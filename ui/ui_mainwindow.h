/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created: Fri 16. Mar 21:28:51 2012
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QFormLayout>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QPlainTextEdit>
#include <QtGui/QPushButton>
#include <QtGui/QTabWidget>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionZakoncz;
    QAction *actionDostepny;
    QAction *actionPorozmawiajmy;
    QAction *actionJestem_zajety;
    QAction *actionZaraz_wracam;
    QAction *actionNiewidoczny;
    QAction *actionNiedostepny;
    QAction *actionTylko_dla_znajomych;
    QAction *actionZamknijKarte;
    QAction *actionImport_listy;
    QAction *actionO_programie;
    QAction *actionKontakty;
    QAction *actionUstawienia_domyslne;
    QAction *actionPobierz_avatary;
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QTabWidget *tabs;
    QWidget *tabMain;
    QFormLayout *formLayout;
    QLineEdit *ggNumber;
    QLabel *labelHaslo;
    QLineEdit *ggPassword;
    QLabel *labelOpis;
    QPlainTextEdit *ggDescription;
    QPushButton *pushButtonUstawOpis;
    QLabel *labelGGNumber;
    QMenuBar *menuBar;
    QMenu *menuNanoGG;
    QMenu *menuOkna;
    QMenu *menuStatus;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(598, 456);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/res/available.png"), QSize(), QIcon::Normal, QIcon::Off);
        MainWindow->setWindowIcon(icon);
        MainWindow->setDocumentMode(true);
        MainWindow->setTabShape(QTabWidget::Rounded);
        actionZakoncz = new QAction(MainWindow);
        actionZakoncz->setObjectName(QString::fromUtf8("actionZakoncz"));
        actionDostepny = new QAction(MainWindow);
        actionDostepny->setObjectName(QString::fromUtf8("actionDostepny"));
        actionDostepny->setCheckable(true);
        actionDostepny->setIcon(icon);
        actionPorozmawiajmy = new QAction(MainWindow);
        actionPorozmawiajmy->setObjectName(QString::fromUtf8("actionPorozmawiajmy"));
        actionPorozmawiajmy->setCheckable(true);
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/res/chat.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionPorozmawiajmy->setIcon(icon1);
        actionJestem_zajety = new QAction(MainWindow);
        actionJestem_zajety->setObjectName(QString::fromUtf8("actionJestem_zajety"));
        actionJestem_zajety->setCheckable(true);
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/res/dnd.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionJestem_zajety->setIcon(icon2);
        actionZaraz_wracam = new QAction(MainWindow);
        actionZaraz_wracam->setObjectName(QString::fromUtf8("actionZaraz_wracam"));
        actionZaraz_wracam->setCheckable(true);
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/res/away.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionZaraz_wracam->setIcon(icon3);
        actionNiewidoczny = new QAction(MainWindow);
        actionNiewidoczny->setObjectName(QString::fromUtf8("actionNiewidoczny"));
        actionNiewidoczny->setCheckable(true);
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/res/invisible.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionNiewidoczny->setIcon(icon4);
        actionNiedostepny = new QAction(MainWindow);
        actionNiedostepny->setObjectName(QString::fromUtf8("actionNiedostepny"));
        actionNiedostepny->setCheckable(true);
        actionNiedostepny->setChecked(true);
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/res/unavailable.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionNiedostepny->setIcon(icon5);
        actionTylko_dla_znajomych = new QAction(MainWindow);
        actionTylko_dla_znajomych->setObjectName(QString::fromUtf8("actionTylko_dla_znajomych"));
        actionTylko_dla_znajomych->setCheckable(true);
        actionTylko_dla_znajomych->setEnabled(true);
        actionTylko_dla_znajomych->setShortcutContext(Qt::WidgetShortcut);
        actionTylko_dla_znajomych->setAutoRepeat(true);
        actionTylko_dla_znajomych->setIconVisibleInMenu(true);
        actionZamknijKarte = new QAction(MainWindow);
        actionZamknijKarte->setObjectName(QString::fromUtf8("actionZamknijKarte"));
        actionZamknijKarte->setVisible(false);
        actionImport_listy = new QAction(MainWindow);
        actionImport_listy->setObjectName(QString::fromUtf8("actionImport_listy"));
        actionO_programie = new QAction(MainWindow);
        actionO_programie->setObjectName(QString::fromUtf8("actionO_programie"));
        actionO_programie->setIcon(icon);
        actionKontakty = new QAction(MainWindow);
        actionKontakty->setObjectName(QString::fromUtf8("actionKontakty"));
        actionUstawienia_domyslne = new QAction(MainWindow);
        actionUstawienia_domyslne->setObjectName(QString::fromUtf8("actionUstawienia_domyslne"));
        actionPobierz_avatary = new QAction(MainWindow);
        actionPobierz_avatary->setObjectName(QString::fromUtf8("actionPobierz_avatary"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(centralWidget->sizePolicy().hasHeightForWidth());
        centralWidget->setSizePolicy(sizePolicy1);
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(2);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setSizeConstraint(QLayout::SetMaximumSize);
        gridLayout->setContentsMargins(1, 3, 1, 1);
        tabs = new QTabWidget(centralWidget);
        tabs->setObjectName(QString::fromUtf8("tabs"));
        sizePolicy1.setHeightForWidth(tabs->sizePolicy().hasHeightForWidth());
        tabs->setSizePolicy(sizePolicy1);
        tabs->setContextMenuPolicy(Qt::NoContextMenu);
        tabs->setAutoFillBackground(true);
        tabs->setTabPosition(QTabWidget::North);
        tabs->setTabShape(QTabWidget::Rounded);
        tabs->setElideMode(Qt::ElideNone);
        tabs->setUsesScrollButtons(true);
        tabs->setDocumentMode(true);
        tabs->setTabsClosable(false);
        tabMain = new QWidget();
        tabMain->setObjectName(QString::fromUtf8("tabMain"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(1);
        sizePolicy2.setHeightForWidth(tabMain->sizePolicy().hasHeightForWidth());
        tabMain->setSizePolicy(sizePolicy2);
        tabMain->setMaximumSize(QSize(16777215, 16777215));
        tabMain->setSizeIncrement(QSize(0, 0));
        formLayout = new QFormLayout(tabMain);
        formLayout->setSpacing(6);
        formLayout->setContentsMargins(11, 11, 11, 11);
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        formLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
        formLayout->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);
        formLayout->setRowWrapPolicy(QFormLayout::DontWrapRows);
        formLayout->setLabelAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        formLayout->setFormAlignment(Qt::AlignCenter);
        formLayout->setHorizontalSpacing(5);
        formLayout->setVerticalSpacing(5);
        formLayout->setContentsMargins(3, 7, 3, 3);
        ggNumber = new QLineEdit(tabMain);
        ggNumber->setObjectName(QString::fromUtf8("ggNumber"));
        QSizePolicy sizePolicy3(QSizePolicy::MinimumExpanding, QSizePolicy::Expanding);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(ggNumber->sizePolicy().hasHeightForWidth());
        ggNumber->setSizePolicy(sizePolicy3);
        ggNumber->setMinimumSize(QSize(0, 32));
        ggNumber->setContextMenuPolicy(Qt::NoContextMenu);
        ggNumber->setAutoFillBackground(false);
        ggNumber->setInputMethodHints(Qt::ImhDigitsOnly|Qt::ImhPreferNumbers);
        ggNumber->setMaxLength(32767);

        formLayout->setWidget(0, QFormLayout::FieldRole, ggNumber);

        labelHaslo = new QLabel(tabMain);
        labelHaslo->setObjectName(QString::fromUtf8("labelHaslo"));

        formLayout->setWidget(2, QFormLayout::LabelRole, labelHaslo);

        ggPassword = new QLineEdit(tabMain);
        ggPassword->setObjectName(QString::fromUtf8("ggPassword"));
        sizePolicy3.setHeightForWidth(ggPassword->sizePolicy().hasHeightForWidth());
        ggPassword->setSizePolicy(sizePolicy3);
        ggPassword->setMinimumSize(QSize(0, 32));
        ggPassword->setEchoMode(QLineEdit::Password);

        formLayout->setWidget(2, QFormLayout::FieldRole, ggPassword);

        labelOpis = new QLabel(tabMain);
        labelOpis->setObjectName(QString::fromUtf8("labelOpis"));

        formLayout->setWidget(3, QFormLayout::LabelRole, labelOpis);

        ggDescription = new QPlainTextEdit(tabMain);
        ggDescription->setObjectName(QString::fromUtf8("ggDescription"));
        sizePolicy2.setHeightForWidth(ggDescription->sizePolicy().hasHeightForWidth());
        ggDescription->setSizePolicy(sizePolicy2);

        formLayout->setWidget(3, QFormLayout::FieldRole, ggDescription);

        pushButtonUstawOpis = new QPushButton(tabMain);
        pushButtonUstawOpis->setObjectName(QString::fromUtf8("pushButtonUstawOpis"));
        sizePolicy1.setHeightForWidth(pushButtonUstawOpis->sizePolicy().hasHeightForWidth());
        pushButtonUstawOpis->setSizePolicy(sizePolicy1);
        pushButtonUstawOpis->setMinimumSize(QSize(0, 48));

        formLayout->setWidget(4, QFormLayout::FieldRole, pushButtonUstawOpis);

        labelGGNumber = new QLabel(tabMain);
        labelGGNumber->setObjectName(QString::fromUtf8("labelGGNumber"));

        formLayout->setWidget(0, QFormLayout::LabelRole, labelGGNumber);

        tabs->addTab(tabMain, icon5, QString());

        gridLayout->addWidget(tabs, 0, 1, 1, 1);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 598, 21));
        menuBar->setFocusPolicy(Qt::ClickFocus);
        menuBar->setContextMenuPolicy(Qt::NoContextMenu);
        menuBar->setAcceptDrops(true);
        menuBar->setDefaultUp(false);
        menuBar->setNativeMenuBar(true);
        menuNanoGG = new QMenu(menuBar);
        menuNanoGG->setObjectName(QString::fromUtf8("menuNanoGG"));
        menuNanoGG->setAcceptDrops(false);
        menuOkna = new QMenu(menuBar);
        menuOkna->setObjectName(QString::fromUtf8("menuOkna"));
        menuStatus = new QMenu(menuBar);
        menuStatus->setObjectName(QString::fromUtf8("menuStatus"));
        MainWindow->setMenuBar(menuBar);

        menuBar->addAction(menuNanoGG->menuAction());
        menuBar->addAction(menuOkna->menuAction());
        menuBar->addAction(menuStatus->menuAction());
        menuNanoGG->addAction(actionZakoncz);
        menuNanoGG->addAction(actionImport_listy);
        menuNanoGG->addAction(actionPobierz_avatary);
        menuNanoGG->addSeparator();
        menuNanoGG->addAction(actionUstawienia_domyslne);
        menuNanoGG->addSeparator();
        menuNanoGG->addAction(actionO_programie);
        menuOkna->addAction(actionKontakty);
        menuOkna->addAction(actionZamknijKarte);
        menuStatus->addAction(actionPorozmawiajmy);
        menuStatus->addAction(actionDostepny);
        menuStatus->addAction(actionJestem_zajety);
        menuStatus->addAction(actionZaraz_wracam);
        menuStatus->addAction(actionNiewidoczny);
        menuStatus->addAction(actionNiedostepny);
        menuStatus->addSeparator();
        menuStatus->addAction(actionTylko_dla_znajomych);

        retranslateUi(MainWindow);
        QObject::connect(actionZakoncz, SIGNAL(triggered()), MainWindow, SLOT(close()));

        tabs->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "nanoGG", 0, QApplication::UnicodeUTF8));
        actionZakoncz->setText(QApplication::translate("MainWindow", "Zako\305\204cz nanoGG", 0, QApplication::UnicodeUTF8));
        actionDostepny->setText(QApplication::translate("MainWindow", "Dost\304\231pny", 0, QApplication::UnicodeUTF8));
        actionPorozmawiajmy->setText(QApplication::translate("MainWindow", "Porozmawiajmy", 0, QApplication::UnicodeUTF8));
        actionJestem_zajety->setText(QApplication::translate("MainWindow", "Jestem zaj\304\231ty", 0, QApplication::UnicodeUTF8));
        actionZaraz_wracam->setText(QApplication::translate("MainWindow", "Zaraz wracam", 0, QApplication::UnicodeUTF8));
        actionNiewidoczny->setText(QApplication::translate("MainWindow", "Niewidoczny", 0, QApplication::UnicodeUTF8));
        actionNiedostepny->setText(QApplication::translate("MainWindow", "Niedost\304\231pny", 0, QApplication::UnicodeUTF8));
        actionTylko_dla_znajomych->setText(QApplication::translate("MainWindow", "Tylko dla znajomych", 0, QApplication::UnicodeUTF8));
        actionTylko_dla_znajomych->setIconText(QApplication::translate("MainWindow", "Tylko dla znajomych", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionTylko_dla_znajomych->setToolTip(QApplication::translate("MainWindow", "Tylko dla znajomych", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionZamknijKarte->setText(QApplication::translate("MainWindow", "Zamknij kart\304\231", 0, QApplication::UnicodeUTF8));
        actionImport_listy->setText(QApplication::translate("MainWindow", "Import Listy", 0, QApplication::UnicodeUTF8));
        actionO_programie->setText(QApplication::translate("MainWindow", "O programie", 0, QApplication::UnicodeUTF8));
        actionKontakty->setText(QApplication::translate("MainWindow", "Kontakty", 0, QApplication::UnicodeUTF8));
        actionUstawienia_domyslne->setText(QApplication::translate("MainWindow", "Ustawienia domy\305\233lne", 0, QApplication::UnicodeUTF8));
        actionPobierz_avatary->setText(QApplication::translate("MainWindow", "Pobierz avatary", 0, QApplication::UnicodeUTF8));
        ggNumber->setInputMask(QString());
        ggNumber->setText(QString());
        labelHaslo->setText(QApplication::translate("MainWindow", "Has\305\202o:", 0, QApplication::UnicodeUTF8));
        labelOpis->setText(QApplication::translate("MainWindow", "Opis:", 0, QApplication::UnicodeUTF8));
        pushButtonUstawOpis->setText(QApplication::translate("MainWindow", "Ustaw", 0, QApplication::UnicodeUTF8));
        labelGGNumber->setText(QApplication::translate("MainWindow", "#GG:", 0, QApplication::UnicodeUTF8));
        tabs->setTabText(tabs->indexOf(tabMain), QApplication::translate("MainWindow", "nanoGG", 0, QApplication::UnicodeUTF8));
        menuNanoGG->setTitle(QApplication::translate("MainWindow", "nanoGG", 0, QApplication::UnicodeUTF8));
        menuOkna->setTitle(QApplication::translate("MainWindow", "Okna", 0, QApplication::UnicodeUTF8));
        menuStatus->setTitle(QApplication::translate("MainWindow", "Status", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
