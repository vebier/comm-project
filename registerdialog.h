#ifndef REGISTERDIALOG_H
#define REGISTERDIALOG_H

#include <QDialog>
#include"global.h"
#include<QTimer>

namespace Ui {
class RegisterDialog;
}

class RegisterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RegisterDialog(QWidget *parent = nullptr);
    ~RegisterDialog();
    QMap<ReqId, std::function<void(const QJsonObject&)>> _handlers;

private slots:
    void on_get_code_clicked();
    void slot_reg_mod_finish(ReqId id, QString res, ErrorCodes err);

    void on_cancel_Button_clicked();

    void on_return_btn_clicked();

private:
    void showTip(QString str,bool b_ok);
    void initHttpHandlers();
    void AddTipErr(TipErr te,QString str);

    void ChangeTipPage();

    bool checkUserValid();
    bool checkEmailValid();
    bool checkPassValid();
    bool checkConfirmValid();
    bool checkVarifyValid();
    void DeleteTipErr(TipErr te);
    Ui::RegisterDialog *ui;
    QMap<TipErr, QString> _tip_errs;
    QTimer* _count_time;
    int _count;
signals:

    void sigSwitchLogin();
};

#endif // REGISTERDIALOG_H
