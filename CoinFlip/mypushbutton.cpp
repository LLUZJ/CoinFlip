#include "mypushbutton.h"
#include<QPainter>
#include<QPropertyAnimation>

MyPushButton::MyPushButton(QString normalImg,QString pressedImg,QWidget *parent)
    : QPushButton{parent}
    ,mNormalImg(normalImg)
    ,mPressedImg(pressedImg)
{
    mStat=Normal;       //初始状态
}

void MyPushButton::moveDown()
{
    //1位置大小属性发生变化
    QPropertyAnimation *animation =new QPropertyAnimation(this,"geometry",this);        //(做动画的对象this该按钮,位置大小属性,对象树父对象)
    //2给定开始的位置大小属性
    animation->setStartValue(this->geometry());
    //3给定结束的位置大小属性
    animation->setEndValue(QRect(this->x(),this->y()+10,this->width(),this->height()));     //在y轴上向下移动10像素
    //4给定速度，给定动画时长
    animation->setDuration(100);        //速度不好给，所以给定时间100ms
    animation->start(QAbstractAnimation::DeleteWhenStopped);        //执行之后，delete掉animation
}

void MyPushButton::moveUp()
{
    QPropertyAnimation *animation =new QPropertyAnimation(this,"geometry",this);
    animation->setStartValue(this->geometry());
    animation->setEndValue(QRect(this->x(),this->y()-10,this->width(),this->height()));     //在y轴上向上移动10像素
    animation->setDuration(100);
    animation->start(QAbstractAnimation::DeleteWhenStopped);        //start之后不是阻塞的，所以会直接执行moveUp进而覆盖掉了moveDown
}

void MyPushButton::paintEvent(QPaintEvent *ev)
{
    //绘制按钮的图片
    QPainter painter(this);     //以当前窗口为画布这个pushbutton
    QPixmap pix;
    if(mStat == Normal)
    {
        pix.load(mNormalImg);
    }
    if(mStat == Pressed)
    {
        pix.load(mPressedImg);
    }

    painter.drawPixmap(0,0,this->width(),this->height(),pix);

    //---绘制按钮的文字
    painter.drawText(0,0,this->width(),this->height(),Qt::AlignHCenter | Qt::AlignVCenter,this->text());
}

void MyPushButton::mousePressEvent(QMouseEvent *e)
{
    //鼠标按下时，状态切换到Pressed
    this->mStat = Pressed;
    update();
    QPushButton::mousePressEvent(e);    //保证信号和槽能实现，保证信号能够发出，信号依赖于事件发生
}

void MyPushButton::mouseReleaseEvent(QMouseEvent *e)
{
    //鼠标按下时，状态切换到Normal
    this->mStat = Normal;
    update();
    QPushButton::mouseReleaseEvent(e);
}
