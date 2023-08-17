#ifndef MYPUSHBUTTON_H
#define MYPUSHBUTTON_H

#include <QWidget>
#include<QPushButton>

class MyPushButton : public QPushButton
{
    Q_OBJECT
public:
    enum MyPushButtonStat       //表示对应状态
    {
        Normal,
        Pressed
    };

    //因为开始按钮和start按钮都是按钮重绘，所以把MyPushButton进行复用
    MyPushButton(QString normalImg,QString pressedImg,QWidget *parent = nullptr);      //通过QString的方式将资源名传进来

    //创建按钮往下网上的动画函数
    void moveDown();
    void moveUp();

protected:
    void paintEvent(QPaintEvent *ev);

    //-------图片切换，状态切换的触发条件，鼠标的按下松开
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent  *e);


signals:

private:
    //正常状态的图片和鼠标按下的图片
    QString mNormalImg;
    QString mPressedImg;

    //记录按钮状态
    MyPushButtonStat mStat;

};

#endif // MYPUSHBUTTON_H
