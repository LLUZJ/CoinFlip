#include "coinbutton.h"
#include<QPainter>

CoinButton::CoinButton(QWidget *parent)
    : QPushButton{parent}
{
    //设置初始状态
    this->setStat(0);

    //设置按钮不规则样式，去掉边框
    this->setStyleSheet("QPushButton{border:0px}");

    //动画的定时器信号和槽实现
    connect(&this->mTimer,&QTimer::timeout,[=](){
        if(this->mStat)
        {
            //银币翻金币
            this->mFrame--;
        }
        else
        {
            //金币翻硬币
            this->mFrame++;
        }

        //加载相应帧的图片
        QString frameName = QString(":/res/Coin000%1.png").arg(this->mFrame);   //将当前应该的帧数填入，实现自然切换
        this->setIcon(QIcon(frameName));
        //停止计时器
        if(this->mFrame==8 || this->mFrame==1)
        {
            this->mTimer.stop();
        }
    });
}

int CoinButton::stat() const
{
    return mStat;
}

void CoinButton::setStat(int Stat)
{
    mStat = Stat;
    //这里可以使用绘图事件重绘方式来做，但是不方便，所以采用其他方法，直接设置按钮的图标
    if(this->mStat)
    {
        //金币
        this->setIcon(QIcon(":/res/Coin0001.png"));
    }
    else
    {
        //银币
        this->setIcon(QIcon(":/res/Coin0008.png"));
    }
    //设置icon大小
    this->setIconSize(this->size());

}

void CoinButton::setStatWithAnimation(int stat)     //一开始我们设置的初始状态是0
{
    this->mStat=stat;                               //这里就进行置1，在playscene.cpp46行，所以下面起始时就是1

    //银币翻金币其实就是金币翻银币 进行反置
    if(this->mStat)                                 //这里判断的是1，其实是0->1的过程就是银币翻金币
    {
        //mStat==1,表示银币翻金币
        this->mFrame=8;
    }
    else                                            //这里判断的是0，其实是1->0的过程 金币翻银币
    {
        //金币翻硬币
        this->mFrame=1;
    }

    this->mTimer.start(30);
}

void CoinButton::flip()
{
    //翻转，状态进行置反
    //this->setStat(!this->stat());
    this->setStatWithAnimation(!this->stat());
}

void CoinButton::paintEvent(QPaintEvent *ev)
{
    //绘制按钮的图片
    QPainter painter(this);     //以当前窗口为画布这个pushbutton
    QPixmap pix;
    pix.load(":/res/BoardNode(1).png");
    painter.drawPixmap(0,0,this->width(),this->height(),pix);

    //调用父类的painEvent    //除了画框框之类的 会把这个硬币调用出来
    QPushButton::paintEvent(ev);
}
