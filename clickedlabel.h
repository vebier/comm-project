#ifndef CLICKEDLABEL_H
#define CLICKEDLABEL_H
#include"global.h"
#include<QLabel>

class ClickedLabel:public QLabel
{
    Q_OBJECT
public:
    ClickedLabel(QWidget *parent=nullptr);
    //点击事件
    virtual void mousePressEvent(QMouseEvent *ev) override;
    //悬停事件
    virtual void enterEvent(QEnterEvent *event) override;
    //离开事件
    virtual void leaveEvent(QEvent *event) override;
    void SetState(QString normal="",QString hover="",QString press="",
                  QString selected="",QString selected_hover="",QString selected_press="");
    ClickLbState GetCurState();
private:
    //普通状态、普通悬浮状态，普通点击状态
    QString _normal;
    QString _normal_hover;
    QString _normal_press;

    //选中状态，选中悬浮状态，选中点击状态
    QString _selected;
    QString _selected_hover;
    QString _selected_press;

    ClickLbState _curstate;
signals:
    void clicked(void);
};

#endif // CLICKEDLABEL_H
