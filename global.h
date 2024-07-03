#ifndef GLOBAL_H
#define GLOBAL_H

#include <QWidget>
#include <functional>
#include "QStyle"
#include<QRegularExpression>
#include<QNetworkReply>
#include <memory>
#include<iostream>
#include<mutex>
#include <QJsonObject>
#include <QJsonDocument>
#include<QDir>
#include<QSettings>
#include<QString>
#include<QMap>

/**
 * @brief repolish 用来刷新qss
 */
extern std::function<void(QWidget*)> repolish;
extern std::function<QString(QString)> XOR;
enum ReqId{
    ID_GET_VARIFY_CODE = 1001, //获取验证码
    ID_REG_USER = 1002, //注册用户
    ID_RESET_PWD = 1003,
    ID_LOGIN_USER = 1004,
    ID_CHAT_LOGIN = 1005,
    ID_CHAT_LOGIN_RSP = 1006,
};

enum ErrorCodes{
    SUCCESS = 0,
    ERR_JSON = 1, //Json解析失败
    ERR_NETWORK = 2,
};

enum Modules{
    REGISTERMOD = 0,
    RESETMOD = 1,
};

enum TipErr{
    TIP_SUCCESS = 0,
    TIP_EMAIL_ERR = 1,
    TIP_PWD_ERR = 2,
    TIP_CONFIRM_ERR = 3,
    TIP_PWD_CONFIRM = 4,
    TIP_VARIFY_ERR = 5,
    TIP_USER_ERR = 6
};

enum ClickLbState{
    Normal = 0,
    Selected = 1
};

extern QString gate_url_prefix;

#endif // GLOBAL_H
