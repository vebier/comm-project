#include "global.h"

QString gate_url_prefix="";

std::function<void(QWidget*)> repolish =[](QWidget *w){
    w->style()->unpolish(w);
    w->style()->polish(w);
};
std::function<QString(QString)> XOR =[](QString str)->QString{
    auto len=str.length();
    len=len%255;
    for(auto& s:str){
        s=QChar(static_cast<ushort>(s.unicode()^static_cast<ushort>(len)));
    }
    return str;
};


