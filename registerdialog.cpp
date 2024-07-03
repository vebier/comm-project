#include "registerdialog.h"
#include "ui_registerdialog.h"
#include"global.h"
#include"httpmgr.h"
#include"clickedlabel.h"
#include<QDebug>

RegisterDialog::RegisterDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::RegisterDialog),_count(5)
{
    ui->setupUi(this);
    ui->pass_lineEdit->setEchoMode(QLineEdit::Password);
    ui->confirm_lineEdit->setEchoMode(QLineEdit::Password);

    ui->err_tip->setProperty("state","normal");
    repolish(ui->err_tip);
     initHttpHandlers();
    connect(HttpMgr::GetInstance().get(), &HttpMgr::sig_reg_mod_finish, this, &RegisterDialog::slot_reg_mod_finish);

     ui->err_tip->clear();

    connect(ui->user_lineEdit,&QLineEdit::editingFinished,this,[this](){
        checkUserValid();
    });
    connect(ui->email_lineEdit, &QLineEdit::editingFinished, this, [this](){
        checkEmailValid();
    });
    connect(ui->pass_lineEdit, &QLineEdit::editingFinished, this, [this](){
        checkPassValid();
    });
    connect(ui->confirm_lineEdit, &QLineEdit::editingFinished, this, [this](){
        checkConfirmValid();
    });
    connect(ui->varify_lineEdit, &QLineEdit::editingFinished, this, [this](){
        checkVarifyValid();
    });

    ui->pass_visible->setCursor(Qt::PointingHandCursor);
    ui->confirm_visible->setCursor(Qt::PointingHandCursor);
    ui->pass_visible->SetState("unvisible","unvisible_hover","","visible",
                               "visible_hover","");
    ui->confirm_visible->SetState("unvisible","unvisible_hover","","visible",
                                  "visible_hover","");
    connect(ui->pass_visible,&ClickedLabel::clicked,this,[this](){
        auto status=ui->pass_visible->GetCurState();
        if(status==ClickLbState::Normal){
            ui->pass_lineEdit->setEchoMode(QLineEdit::Password);
        }else{
            ui->pass_lineEdit->setEchoMode(QLineEdit::Normal);
        }
        qDebug() << "Label was clicked!";
    });

    connect(ui->confirm_visible,&ClickedLabel::clicked,this,[this](){
        auto status=ui->confirm_visible->GetCurState();
        if(status==ClickLbState::Normal){
            ui->confirm_lineEdit->setEchoMode(QLineEdit::Password);
        }else{
            ui->confirm_lineEdit->setEchoMode(QLineEdit::Normal);
        }
        qDebug() << "Label was clicked!";
    });

    _count_time=new QTimer(this);

    connect(_count_time,&QTimer::timeout,[this](){
        if(_count==0){
            _count_time->stop();
            emit sigSwitchLogin();
            return;
        }
        _count--;
        auto str=QString("注册成功，%1 s后返回登录").arg(_count);
        ui->tip_lb->setText(str);
    });
}

RegisterDialog::~RegisterDialog()
{
    qDebug()<<"destruct Registerdialog";
    delete ui;
}



void RegisterDialog::on_get_code_clicked()
{
    //验证邮箱的地址正则表达式
    auto email = ui->email_lineEdit->text();
    // 邮箱地址的正则表达式
    QRegularExpression regex(R"((\w+)(\.|_)?(\w*)@(\w+)(\.(\w+))+)");
    bool match = regex.match(email).hasMatch(); // 执行正则表达式匹配
    if(match){
        //发送http请求获取验证码
        QJsonObject json_obj;
        json_obj["email"]=email;
        HttpMgr::GetInstance()->PostHttpReq(QUrl(gate_url_prefix+"/get_varifycode"),json_obj,ReqId::ID_GET_VARIFY_CODE,
                                            Modules::REGISTERMOD);
    }else{
        //提示邮箱不正确
        showTip(tr("邮箱地址不正确"),false);
    }
}

void RegisterDialog::slot_reg_mod_finish(ReqId id, QString res, ErrorCodes err)
{
    if(err != ErrorCodes::SUCCESS){
        showTip(tr("网络请求错误"),false);
        return;
    }
    // 解析 JSON 字符串,res需转化为QByteArray
    QJsonDocument jsonDoc = QJsonDocument::fromJson(res.toUtf8());
    //json解析错误
    if(jsonDoc.isNull()){
        showTip(tr("json解析错误"),false);
        return;
    }
    //json解析错误
    if(!jsonDoc.isObject()){
        showTip(tr("json解析错误"),false);
        return;
    }
    QJsonObject jsonObj = jsonDoc.object();
    _handlers[id](jsonDoc.object());
    return;
}

void RegisterDialog::showTip(QString str,bool b_ok)
{
    if(b_ok){
       ui->err_tip->setProperty("state","normal");
    }else{
         ui->err_tip->setProperty("state","err");
    }
    ui->err_tip->setText(str);
    repolish(ui->err_tip);
}

void RegisterDialog::initHttpHandlers()
{
    //注册获取验证码回包逻辑
    _handlers.insert(ReqId::ID_GET_VARIFY_CODE, [this](QJsonObject jsonObj){
        int error = jsonObj["error"].toInt();
        if(error != ErrorCodes::SUCCESS){
            showTip(tr("参数错误"),false);
            return;
        }
        auto email = jsonObj["email"].toString();
        showTip(tr("验证码已发送到邮箱，注意查收"), true);

        qDebug()<< "email is " << email ;
    });

    //注册注册用户回包逻辑
    _handlers.insert(ReqId::ID_REG_USER, [this](QJsonObject jsonObj){
        int error = jsonObj["error"].toInt();
        if(error != ErrorCodes::SUCCESS){
            showTip(tr("参数错误"),false);
            return;
        }
        auto email = jsonObj["email"].toString();
        showTip(tr("用户注册成功"), true);
        qDebug()<<"User uuid is "<<jsonObj["uid"].toString();
        qDebug()<< "email is " << email ;
    });
}

void RegisterDialog::AddTipErr(TipErr te, QString tips)
{
    _tip_errs[te] = tips;
    showTip(tips, false);
}

void RegisterDialog::DeleteTipErr(TipErr te)
{
    _tip_errs.remove(te);
    if(_tip_errs.empty()){
        ui->err_tip->clear();
        return;
    }
    showTip(_tip_errs.first(), false);
}

bool RegisterDialog::checkUserValid()
{
    if(ui->user_lineEdit->text() == ""){
        AddTipErr(TipErr::TIP_USER_ERR, tr("用户名不能为空"));
        return false;
    }
    DeleteTipErr(TipErr::TIP_USER_ERR);
    return true;
}

bool RegisterDialog::checkEmailValid()
{
    auto email=ui->email_lineEdit->text();
    QRegularExpression regex(R"((\w+)(\.|_)?(\w*)@(\w+)(\.(\w+))+)");
    bool match = regex.match(email).hasMatch(); // 执行正则表达式匹配
    if(!match){
        AddTipErr(TipErr::TIP_EMAIL_ERR, tr("邮箱地址不正确"));
        return false;
    }
    DeleteTipErr(TipErr::TIP_EMAIL_ERR);
    return true;
}

bool RegisterDialog::checkPassValid()
{
    auto pass=ui->pass_lineEdit->text();
    if(pass.length()<6||pass.length()>15){
        AddTipErr(TipErr::TIP_PWD_ERR, tr("密码长度应为6~15"));
        return false;
    }
    // 创建一个正则表达式对象，按照上述密码要求
    // 这个正则表达式解释：
    // ^[a-zA-Z0-9!@#$%^&*]{6,15}$ 密码长度至少6，可以是字母、数字和特定的特殊字符
    QRegularExpression regExp("^[a-zA-Z0-9!@#$%^&*]{6,15}$");
    bool match = regExp.match(pass).hasMatch();
    if(!match){
        AddTipErr(TipErr::TIP_PWD_ERR, tr("不能包含非法字符"));
        return false;
    }
    DeleteTipErr(TipErr::TIP_PWD_ERR);
    return true;
}

bool RegisterDialog::checkConfirmValid()
{
    auto pass1=ui->confirm_lineEdit->text();
    if(pass1.length()<6||pass1.length()>15){
        AddTipErr(TipErr::TIP_PWD_ERR, tr("密码长度应为6~15"));
        return false;
    }
    // 创建一个正则表达式对象，按照上述密码要求
    // 这个正则表达式解释：
    // ^[a-zA-Z0-9!@#$%^&*]{6,15}$ 密码长度至少6，可以是字母、数字和特定的特殊字符
    QRegularExpression regExp("^[a-zA-Z0-9!@#$%^&*]{6,15}$");
    bool match = regExp.match(pass1).hasMatch();
    if(!match){
        AddTipErr(TipErr::TIP_PWD_ERR, tr("不能包含非法字符"));
        return false;
    }
    auto pass2=ui->pass_lineEdit->text();
    if(pass1!=pass2){
        AddTipErr(TIP_PWD_CONFIRM,tr("两次密码输入不一样"));
        return false;
    }
    DeleteTipErr(TIP_PWD_CONFIRM);
    return true;
}

bool RegisterDialog::checkVarifyValid()
{
    auto varify=ui->varify_lineEdit->text();
    if(varify==""){
        AddTipErr(TipErr::TIP_VARIFY_ERR, tr("验证码不能为空"));
        return false;
    }
    DeleteTipErr(TipErr::TIP_VARIFY_ERR);
    return true;
}

void RegisterDialog::ChangeTipPage()
{
    _count_time->stop();
    ui->stackedWidget->setCurrentWidget(ui->page_2);
    _count_time->start(1000);
}





void RegisterDialog::on_cancel_Button_clicked()
{
    bool vailed=checkUserValid();
    if(!vailed){//用户名为空，错误，返回
        return;
    }
    vailed=checkEmailValid();
    if(!vailed){//邮箱格式错误，返回
        return;
    }
    vailed=checkPassValid();
    if(!vailed){//密码格式错误，返回
        return;
    }
    vailed=checkConfirmValid();
    if(!vailed){//确认密码格式错误或者和密码不一致，返回
        return;
    }
    vailed=checkVarifyValid();
    if(!vailed){//验证码为空，错误，返回
        return;
    }

    //day11 发送http请求注册用户
    QJsonObject json_obj;
    json_obj["user"] = ui->user_lineEdit->text();
    json_obj["email"] = ui->email_lineEdit->text();
    json_obj["passwd"] = XOR(ui->pass_lineEdit->text());
    json_obj["confirm"]=XOR(ui->confirm_lineEdit->text());
    json_obj["varifycode"] = ui->varify_lineEdit->text();
    HttpMgr::GetInstance()->PostHttpReq(QUrl(gate_url_prefix+"/user_register"),
                                        json_obj, ReqId::ID_REG_USER,Modules::REGISTERMOD);
}


void RegisterDialog::on_return_btn_clicked()
{
    _count_time->stop();
    emit sigSwitchLogin();
}

