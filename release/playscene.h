#ifndef PLAYSCENE_H
#define PLAYSCENE_H

#include <QMainWindow>
#include"mymainwindow.h"
#include"coinbutton.h"

class PlayScene : public MyMainWindow
{
    Q_OBJECT
public:
    PlayScene(int level,QWidget *parent = nullptr);
    void flip(int row,int col);     //翻动row行，col列的硬币

    //判断胜利
    void judegeWin();

signals:
    void backBtnClicked();      //游戏场景中点击返回按钮发射的信号

protected:
    void paintEvent(QPaintEvent *event);

private:
    //胜利的状态
    bool mHasWin;
    //硬币按钮数组
    CoinButton *mCoins[4][4];   //存取每个按钮的指针

};

#endif // PLAYSCENE_H
