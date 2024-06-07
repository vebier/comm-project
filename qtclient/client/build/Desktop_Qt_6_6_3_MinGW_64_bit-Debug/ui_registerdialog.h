/********************************************************************************
** Form generated from reading UI file 'registerdialog.ui'
**
** Created by: Qt User Interface Compiler version 6.6.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_REGISTERDIALOG_H
#define UI_REGISTERDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_RegisterDialog
{
public:
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    QStackedWidget *stackedWidget;
    QWidget *page;
    QVBoxLayout *verticalLayout_3;
    QSpacerItem *verticalSpacer;
    QWidget *widget;
    QVBoxLayout *verticalLayout_4;
    QLabel *err_tip;
    QHBoxLayout *horizontalLayout;
    QLabel *user_label;
    QLineEdit *user_lineEdit;
    QHBoxLayout *horizontalLayout_2;
    QLabel *email_label;
    QLineEdit *email_lineEdit;
    QHBoxLayout *horizontalLayout_3;
    QLabel *pass_label;
    QLineEdit *pass_lineEdit;
    QHBoxLayout *horizontalLayout_4;
    QLabel *confirm_label;
    QLineEdit *confirm_lineEdit;
    QHBoxLayout *horizontalLayout_5;
    QLabel *varify_label;
    QLineEdit *varify_lineEdit;
    QPushButton *get_code;
    QSpacerItem *verticalSpacer_2;
    QHBoxLayout *horizontalLayout_6;
    QPushButton *cancel_Button;
    QPushButton *confirm_Button;
    QSpacerItem *verticalSpacer_3;
    QWidget *page_2;

    void setupUi(QDialog *RegisterDialog)
    {
        if (RegisterDialog->objectName().isEmpty())
            RegisterDialog->setObjectName("RegisterDialog");
        RegisterDialog->resize(300, 500);
        RegisterDialog->setMinimumSize(QSize(300, 500));
        RegisterDialog->setMaximumSize(QSize(300, 500));
        verticalLayout_2 = new QVBoxLayout(RegisterDialog);
        verticalLayout_2->setObjectName("verticalLayout_2");
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName("verticalLayout");
        stackedWidget = new QStackedWidget(RegisterDialog);
        stackedWidget->setObjectName("stackedWidget");
        page = new QWidget();
        page->setObjectName("page");
        verticalLayout_3 = new QVBoxLayout(page);
        verticalLayout_3->setObjectName("verticalLayout_3");
        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout_3->addItem(verticalSpacer);

        widget = new QWidget(page);
        widget->setObjectName("widget");
        verticalLayout_4 = new QVBoxLayout(widget);
        verticalLayout_4->setObjectName("verticalLayout_4");
        err_tip = new QLabel(widget);
        err_tip->setObjectName("err_tip");
        err_tip->setAlignment(Qt::AlignCenter);

        verticalLayout_4->addWidget(err_tip);


        verticalLayout_3->addWidget(widget);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        user_label = new QLabel(page);
        user_label->setObjectName("user_label");
        user_label->setMinimumSize(QSize(0, 25));
        user_label->setMaximumSize(QSize(16777215, 25));

        horizontalLayout->addWidget(user_label);

        user_lineEdit = new QLineEdit(page);
        user_lineEdit->setObjectName("user_lineEdit");
        user_lineEdit->setMinimumSize(QSize(0, 25));
        user_lineEdit->setMaximumSize(QSize(16777215, 25));

        horizontalLayout->addWidget(user_lineEdit);


        verticalLayout_3->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        email_label = new QLabel(page);
        email_label->setObjectName("email_label");
        email_label->setMinimumSize(QSize(0, 25));
        email_label->setMaximumSize(QSize(16777215, 25));

        horizontalLayout_2->addWidget(email_label);

        email_lineEdit = new QLineEdit(page);
        email_lineEdit->setObjectName("email_lineEdit");
        email_lineEdit->setMinimumSize(QSize(0, 25));
        email_lineEdit->setMaximumSize(QSize(16777215, 25));

        horizontalLayout_2->addWidget(email_lineEdit);


        verticalLayout_3->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        pass_label = new QLabel(page);
        pass_label->setObjectName("pass_label");
        pass_label->setMinimumSize(QSize(0, 25));
        pass_label->setMaximumSize(QSize(16777215, 25));

        horizontalLayout_3->addWidget(pass_label);

        pass_lineEdit = new QLineEdit(page);
        pass_lineEdit->setObjectName("pass_lineEdit");
        pass_lineEdit->setMinimumSize(QSize(0, 25));
        pass_lineEdit->setMaximumSize(QSize(16777215, 25));

        horizontalLayout_3->addWidget(pass_lineEdit);


        verticalLayout_3->addLayout(horizontalLayout_3);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName("horizontalLayout_4");
        confirm_label = new QLabel(page);
        confirm_label->setObjectName("confirm_label");
        confirm_label->setMinimumSize(QSize(0, 25));
        confirm_label->setMaximumSize(QSize(16777215, 25));

        horizontalLayout_4->addWidget(confirm_label);

        confirm_lineEdit = new QLineEdit(page);
        confirm_lineEdit->setObjectName("confirm_lineEdit");
        confirm_lineEdit->setMinimumSize(QSize(0, 25));
        confirm_lineEdit->setMaximumSize(QSize(16777215, 25));

        horizontalLayout_4->addWidget(confirm_lineEdit);


        verticalLayout_3->addLayout(horizontalLayout_4);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName("horizontalLayout_5");
        varify_label = new QLabel(page);
        varify_label->setObjectName("varify_label");
        varify_label->setMinimumSize(QSize(0, 25));
        varify_label->setMaximumSize(QSize(16777215, 25));

        horizontalLayout_5->addWidget(varify_label);

        varify_lineEdit = new QLineEdit(page);
        varify_lineEdit->setObjectName("varify_lineEdit");
        varify_lineEdit->setMinimumSize(QSize(0, 25));
        varify_lineEdit->setMaximumSize(QSize(16777215, 25));

        horizontalLayout_5->addWidget(varify_lineEdit);

        get_code = new QPushButton(page);
        get_code->setObjectName("get_code");
        get_code->setMinimumSize(QSize(0, 25));
        get_code->setMaximumSize(QSize(16777215, 25));

        horizontalLayout_5->addWidget(get_code);


        verticalLayout_3->addLayout(horizontalLayout_5);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout_3->addItem(verticalSpacer_2);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName("horizontalLayout_6");
        cancel_Button = new QPushButton(page);
        cancel_Button->setObjectName("cancel_Button");
        cancel_Button->setMinimumSize(QSize(0, 25));
        cancel_Button->setMaximumSize(QSize(16777215, 25));

        horizontalLayout_6->addWidget(cancel_Button);

        confirm_Button = new QPushButton(page);
        confirm_Button->setObjectName("confirm_Button");
        confirm_Button->setMinimumSize(QSize(0, 25));
        confirm_Button->setMaximumSize(QSize(16777215, 25));

        horizontalLayout_6->addWidget(confirm_Button);


        verticalLayout_3->addLayout(horizontalLayout_6);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout_3->addItem(verticalSpacer_3);

        stackedWidget->addWidget(page);
        page_2 = new QWidget();
        page_2->setObjectName("page_2");
        stackedWidget->addWidget(page_2);

        verticalLayout->addWidget(stackedWidget);


        verticalLayout_2->addLayout(verticalLayout);


        retranslateUi(RegisterDialog);

        QMetaObject::connectSlotsByName(RegisterDialog);
    } // setupUi

    void retranslateUi(QDialog *RegisterDialog)
    {
        RegisterDialog->setWindowTitle(QCoreApplication::translate("RegisterDialog", "Dialog", nullptr));
        err_tip->setText(QCoreApplication::translate("RegisterDialog", "\351\224\231\350\257\257\346\217\220\347\244\272", nullptr));
        user_label->setText(QCoreApplication::translate("RegisterDialog", "\347\224\250\346\210\267:", nullptr));
        email_label->setText(QCoreApplication::translate("RegisterDialog", "\351\202\256\347\256\261:", nullptr));
        pass_label->setText(QCoreApplication::translate("RegisterDialog", "\345\257\206\347\240\201:", nullptr));
        confirm_label->setText(QCoreApplication::translate("RegisterDialog", "\347\241\256\350\256\244:", nullptr));
        varify_label->setText(QCoreApplication::translate("RegisterDialog", "\351\252\214\350\257\201\347\240\201:", nullptr));
        get_code->setText(QCoreApplication::translate("RegisterDialog", "\350\216\267\345\217\226", nullptr));
        cancel_Button->setText(QCoreApplication::translate("RegisterDialog", "\347\241\256\350\256\244", nullptr));
        confirm_Button->setText(QCoreApplication::translate("RegisterDialog", "\345\217\226\346\266\210", nullptr));
    } // retranslateUi

};

namespace Ui {
    class RegisterDialog: public Ui_RegisterDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_REGISTERDIALOG_H
