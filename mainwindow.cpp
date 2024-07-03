#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //创建一个CentralWidget, 并将其设置为MainWindow的中心部件
    _login_dlg = new LoginDialog(this);
    _login_dlg->setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
    setCentralWidget(_login_dlg);
    //_login_dlg->show();

    //创建和注册消息的链接
    connect(_login_dlg, &LoginDialog::switchRegister,
            this, &MainWindow::SlotSwitchReg);
    //连接登录窗口忘记密码信号
    connect(_login_dlg, &LoginDialog::switchReset, this, &MainWindow::SlotSwitchReset);
}

MainWindow::~MainWindow()
{
    delete ui;
    // if(_login_dlg){
    //     delete _login_dlg;
    //     _login_dlg=nullptr;
    // }

    // if(_reg_dlg){
    //     delete _reg_dlg;
    //     _reg_dlg=nullptr;
    // }
}

void MainWindow::SlotSwitchReg()
{
    _reg_dlg = new RegisterDialog(this);
    _reg_dlg->setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);

    connect(_reg_dlg,&RegisterDialog::sigSwitchLogin,this,&MainWindow::SlotSwitchLogin);

    setCentralWidget(_reg_dlg);
    _login_dlg->hide();
    _reg_dlg->show();

}

void MainWindow::SlotSwitchLogin()
{
    _login_dlg=new LoginDialog(this);
    _login_dlg->setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
    setCentralWidget(_login_dlg);

    _login_dlg->hide();
    _reset_dlg->show();
    //连接登录界面注册信号
    connect(_login_dlg, &LoginDialog::switchRegister, this, &MainWindow::SlotSwitchReg);


}

void MainWindow::SlotSwitchReset()
{
    //把注册窗口通过覆盖的方式显示在主窗口
    _reset_dlg=new ResetDialog(this);
    _reset_dlg->setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
    setCentralWidget(_reset_dlg);

    _login_dlg->hide();
    _login_dlg->show();
    //绑定注册窗口登录信号到登录窗口
    connect(_reset_dlg,&ResetDialog::switchLogin,this,&MainWindow::New_pwd_Return);
}

void MainWindow::New_pwd_Return()
{
    _login_dlg=new LoginDialog(this);
    _login_dlg->setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
    setCentralWidget(_login_dlg);

    _reset_dlg->hide();
    _login_dlg->show();

    connect(_login_dlg,&LoginDialog::switchReset,this,&MainWindow::SlotSwitchReset);
    connect(_login_dlg,&LoginDialog::switchRegister,this,&MainWindow::SlotSwitchReg);
}


