/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QFormLayout *formLayout;
    QComboBox *priorityComboBox;
    QPushButton *addButton;
    QTextEdit *textEdit;
    QPushButton *sendButton;
    QComboBox *configComboBox;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(444, 528);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        formLayout = new QFormLayout(centralwidget);
        formLayout->setObjectName("formLayout");
        priorityComboBox = new QComboBox(centralwidget);
        priorityComboBox->addItem(QString());
        priorityComboBox->addItem(QString());
        priorityComboBox->addItem(QString());
        priorityComboBox->addItem(QString());
        priorityComboBox->addItem(QString());
        priorityComboBox->addItem(QString());
        priorityComboBox->addItem(QString());
        priorityComboBox->addItem(QString());
        priorityComboBox->addItem(QString());
        priorityComboBox->setObjectName("priorityComboBox");

        formLayout->setWidget(1, QFormLayout::LabelRole, priorityComboBox);

        addButton = new QPushButton(centralwidget);
        addButton->setObjectName("addButton");
        addButton->setCheckable(false);

        formLayout->setWidget(1, QFormLayout::FieldRole, addButton);

        textEdit = new QTextEdit(centralwidget);
        textEdit->setObjectName("textEdit");

        formLayout->setWidget(2, QFormLayout::SpanningRole, textEdit);

        sendButton = new QPushButton(centralwidget);
        sendButton->setObjectName("sendButton");
        sendButton->setCheckable(false);

        formLayout->setWidget(3, QFormLayout::SpanningRole, sendButton);

        configComboBox = new QComboBox(centralwidget);
        configComboBox->addItem(QString());
        configComboBox->addItem(QString());
        configComboBox->addItem(QString());
        configComboBox->addItem(QString());
        configComboBox->addItem(QString());
        configComboBox->addItem(QString());
        configComboBox->addItem(QString());
        configComboBox->addItem(QString());
        configComboBox->addItem(QString());
        configComboBox->addItem(QString());
        configComboBox->addItem(QString());
        configComboBox->addItem(QString());
        configComboBox->addItem(QString());
        configComboBox->addItem(QString());
        configComboBox->addItem(QString());
        configComboBox->addItem(QString());
        configComboBox->setObjectName("configComboBox");
        configComboBox->setBaseSize(QSize(0, 0));
        configComboBox->setEditable(false);

        formLayout->setWidget(0, QFormLayout::SpanningRole, configComboBox);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 444, 22));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        priorityComboBox->setItemText(0, QCoreApplication::translate("MainWindow", "1", nullptr));
        priorityComboBox->setItemText(1, QCoreApplication::translate("MainWindow", "2", nullptr));
        priorityComboBox->setItemText(2, QCoreApplication::translate("MainWindow", "3", nullptr));
        priorityComboBox->setItemText(3, QCoreApplication::translate("MainWindow", "4", nullptr));
        priorityComboBox->setItemText(4, QCoreApplication::translate("MainWindow", "5", nullptr));
        priorityComboBox->setItemText(5, QCoreApplication::translate("MainWindow", "6", nullptr));
        priorityComboBox->setItemText(6, QCoreApplication::translate("MainWindow", "7", nullptr));
        priorityComboBox->setItemText(7, QCoreApplication::translate("MainWindow", "8", nullptr));
        priorityComboBox->setItemText(8, QCoreApplication::translate("MainWindow", "\320\235\320\276\320\262\321\213\320\271 \321\215\320\273\320\265\320\274\320\265\320\275\321\202", nullptr));

        addButton->setText(QCoreApplication::translate("MainWindow", "Add", nullptr));
        sendButton->setText(QCoreApplication::translate("MainWindow", "Send", nullptr));
        configComboBox->setItemText(0, QCoreApplication::translate("MainWindow", "\320\276\320\264\320\275\320\260 \321\201\321\202\321\200\320\276\320\272\320\260", nullptr));
        configComboBox->setItemText(1, QCoreApplication::translate("MainWindow", "\320\264\320\262\320\265 \321\201\321\202\321\200\320\276\320\272\320\270", nullptr));
        configComboBox->setItemText(2, QCoreApplication::translate("MainWindow", "\321\202\321\200\320\270 \321\201\321\202\321\200\320\276\320\272\320\270", nullptr));
        configComboBox->setItemText(3, QCoreApplication::translate("MainWindow", "\321\207\320\265\321\202\321\213\321\200\320\265 \321\201\321\202\321\200\320\276\320\272\320\270", nullptr));
        configComboBox->setItemText(4, QCoreApplication::translate("MainWindow", "\320\276\320\264\320\270\320\275 \321\201\321\202\320\276\320\273\320\261\320\265\321\206", nullptr));
        configComboBox->setItemText(5, QCoreApplication::translate("MainWindow", "\320\264\320\262\320\260 \321\201\321\202\320\276\320\273\320\261\321\206\320\260", nullptr));
        configComboBox->setItemText(6, QCoreApplication::translate("MainWindow", "\321\202\321\200\320\270 \321\201\321\202\320\276\320\273\320\261\321\206\320\260", nullptr));
        configComboBox->setItemText(7, QCoreApplication::translate("MainWindow", "\321\207\320\265\321\202\321\213\321\200\320\265 \321\201\321\202\320\276\320\273\320\261\321\206\320\260", nullptr));
        configComboBox->setItemText(8, QCoreApplication::translate("MainWindow", "1\321\2051", nullptr));
        configComboBox->setItemText(9, QCoreApplication::translate("MainWindow", "1\321\2052", nullptr));
        configComboBox->setItemText(10, QCoreApplication::translate("MainWindow", "1\321\2053", nullptr));
        configComboBox->setItemText(11, QCoreApplication::translate("MainWindow", "2\321\2053", nullptr));
        configComboBox->setItemText(12, QCoreApplication::translate("MainWindow", "2\321\2052", nullptr));
        configComboBox->setItemText(13, QCoreApplication::translate("MainWindow", "3\321\2053", nullptr));
        configComboBox->setItemText(14, QCoreApplication::translate("MainWindow", "4\321\2054", nullptr));
        configComboBox->setItemText(15, QCoreApplication::translate("MainWindow", "8\321\2058", nullptr));

        configComboBox->setCurrentText(QCoreApplication::translate("MainWindow", "\320\276\320\264\320\275\320\260 \321\201\321\202\321\200\320\276\320\272\320\260", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
