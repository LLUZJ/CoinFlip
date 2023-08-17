#ifndef COINBUTTON_H
#define COINBUTTON_H

#include <QWidget>
#include<QPushButton>
#include<QTimer>

class CoinButton : public QPushButton
{
    Q_OBJECT
public:
    explicit CoinButton(QWidget *parent = nullptr);

    int stat() const;
    void setStat(int Stat);

    //setStat带动画的函数
    void setStatWithAnimation(int stat);        //设置一个什么状态进来，就会自动播放动画

    //硬币翻转函数
    void flip();

signals:

private:
    //硬币状态，0表示硬币，1表示金币
    int mStat;          //按住Alt+Enter,选择Generate Getter and Setter,可以开始创建值和set
    //记录当前动画帧数
    int mFrame;
    //定时器
    QTimer mTimer;

protected:
    void paintEvent(QPaintEvent *ev);

};

#endif // COINBUTTON_H
