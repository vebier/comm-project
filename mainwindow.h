#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include"logindialog.h"
#include"registerdialog.h"
#include"resetdialog.h"
/******************************************************************************
 *
 * @file       mainwindow.h
 * @brief      主窗口
 *
 * @author     vabie
 * @date       2024/06/05
 * @history
 *****************************************************************************/

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public slots:
    void SlotSwitchReg();
    void SlotSwitchLogin();
    void SlotSwitchReset();
    void New_pwd_Return();
private:
    Ui::MainWindow *ui;
    LoginDialog * _login_dlg;
    RegisterDialog* _reg_dlg;
    ResetDialog* _reset_dlg;
};
#endif // MAINWINDOW_H
