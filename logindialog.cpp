#include "logindialog.h"
#include "ui_logindialog.h"
#include<QDebug>
#include"clickedlabel.h"

LoginDialog::LoginDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    //绑定注册按钮发送的信号，点击注册跳转到注册界面
    connect(ui->reg_btn, &QPushButton::clicked, this, &LoginDialog::switchRegister);
    ui->forget_label->SetState("normal","hover","","selected","selected_hover","");
    connect(ui->forget_label,&ClickedLabel::clicked,this,&LoginDialog::slot_forget_pwd);
}

LoginDialog::~LoginDialog()
{
    qDebug()<<"destruct LoginDlg";
    delete ui;
}

void LoginDialog::slot_forget_pwd()
{
    qDebug()<<"slot forget pwd";
    emit switchReset();
}
