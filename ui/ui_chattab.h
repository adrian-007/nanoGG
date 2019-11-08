/********************************************************************************
** Form generated from reading UI file 'chattab.ui'
**
** Created: Fri 16. Mar 21:28:51 2012
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CHATTAB_H
#define UI_CHATTAB_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPlainTextEdit>
#include <QtGui/QPushButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include <QtWebKit/QWebView>

QT_BEGIN_NAMESPACE

class Ui_ChatTab
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_2;
    QLabel *avatar;
    QLabel *description;
    QWebView *webView;
    QHBoxLayout *horizontalLayout;
    QPlainTextEdit *input;
    QPushButton *send;

    void setupUi(QWidget *ChatTab)
    {
        if (ChatTab->objectName().isEmpty())
            ChatTab->setObjectName(QString::fromUtf8("ChatTab"));
        ChatTab->resize(641, 497);
        verticalLayout = new QVBoxLayout(ChatTab);
        verticalLayout->setSpacing(7);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 7, 0, 3);
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setSizeConstraint(QLayout::SetMinAndMaxSize);
        avatar = new QLabel(ChatTab);
        avatar->setObjectName(QString::fromUtf8("avatar"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(avatar->sizePolicy().hasHeightForWidth());
        avatar->setSizePolicy(sizePolicy);
        avatar->setMinimumSize(QSize(32, 32));
        avatar->setMaximumSize(QSize(32, 32));
        avatar->setPixmap(QPixmap(QString::fromUtf8(":/res/avatar.png")));
        avatar->setScaledContents(true);

        horizontalLayout_2->addWidget(avatar);

        description = new QLabel(ChatTab);
        description->setObjectName(QString::fromUtf8("description"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(description->sizePolicy().hasHeightForWidth());
        description->setSizePolicy(sizePolicy1);
        description->setMaximumSize(QSize(16777215, 48));
        description->setContextMenuPolicy(Qt::NoContextMenu);
        description->setAutoFillBackground(true);
        description->setWordWrap(true);

        horizontalLayout_2->addWidget(description);


        verticalLayout->addLayout(horizontalLayout_2);

        webView = new QWebView(ChatTab);
        webView->setObjectName(QString::fromUtf8("webView"));
        sizePolicy1.setHeightForWidth(webView->sizePolicy().hasHeightForWidth());
        webView->setSizePolicy(sizePolicy1);
        webView->setUrl(QUrl("about:blank"));

        verticalLayout->addWidget(webView);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(5);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setSizeConstraint(QLayout::SetMinAndMaxSize);
        horizontalLayout->setContentsMargins(0, 0, 5, 0);
        input = new QPlainTextEdit(ChatTab);
        input->setObjectName(QString::fromUtf8("input"));
        QSizePolicy sizePolicy2(QSizePolicy::MinimumExpanding, QSizePolicy::Expanding);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(input->sizePolicy().hasHeightForWidth());
        input->setSizePolicy(sizePolicy2);
        input->setMinimumSize(QSize(0, 48));
        input->setMaximumSize(QSize(16777215, 48));
        input->setUndoRedoEnabled(false);

        horizontalLayout->addWidget(input);

        send = new QPushButton(ChatTab);
        send->setObjectName(QString::fromUtf8("send"));
        sizePolicy.setHeightForWidth(send->sizePolicy().hasHeightForWidth());
        send->setSizePolicy(sizePolicy);
        send->setMinimumSize(QSize(48, 48));
        send->setMaximumSize(QSize(48, 48));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/res/send.png"), QSize(), QIcon::Normal, QIcon::Off);
        send->setIcon(icon);

        horizontalLayout->addWidget(send);

        horizontalLayout->setStretch(1, 1);

        verticalLayout->addLayout(horizontalLayout);

        verticalLayout->setStretch(1, 1);

        retranslateUi(ChatTab);

        QMetaObject::connectSlotsByName(ChatTab);
    } // setupUi

    void retranslateUi(QWidget *ChatTab)
    {
        ChatTab->setWindowTitle(QApplication::translate("ChatTab", "Form", 0, QApplication::UnicodeUTF8));
        avatar->setText(QString());
        description->setText(QString());
        input->setPlainText(QString());
        send->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class ChatTab: public Ui_ChatTab {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CHATTAB_H
