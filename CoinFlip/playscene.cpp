#include "playscene.h"
#include "mypushbutton.h"
#include<QPainter>
#include<QMenuBar>
#include<QLabel>
#include "coinbutton.h"
#include "dataconfig.h"
#include<QTimer>
#include<QMessageBox>
#include<QLabel>
#include<QPropertyAnimation>
#include<QSound>

PlayScene::PlayScene(int level,QWidget *parent)
    : MyMainWindow{parent}
{
    //胜利状态判断
    mHasWin = false;

    //游戏场景中的返回按钮
    MyPushButton *btnBack =new MyPushButton(":/res/BackButton.png",":/res/BackButtonSelected.png",this);
    btnBack->resize(72,32);

    connect(btnBack,&QPushButton::clicked,this,&PlayScene::backBtnClicked);       //信号连接信号，按钮按下时触发自定义信号

    btnBack->move(this->width()-btnBack->width(),this->height()-btnBack->height());

    //添加左下角的label 即关卡数
    QLabel *label =new QLabel(this);
    label->setText(QString("level: %1").arg(level));
    //设置label框的大小
    label->resize(150,50);
    //设置字体
    label->setFont(QFont("华文新魏",20));
    //移动到左下角
    label->move(30,this->height()-label->height());


    //取出第几关的二位数组数据
    dataConfig data;
    QVector<QVector <int>> dataArray = data.mData[level];

    //----16个硬币按钮
    const int colWidth  =50;    //列宽
    const int rowHeight =50;    //行高

    const int xOffset =57;      //x y偏移     原点偏移，不然按钮会怼到开始菜单那里
    const int yOffset =200;

    for(int row =0;row<4;++row)
    {
        for(int col=0;col<4;++col)
        {
            CoinButton *btn=new CoinButton(this);

            //存储按钮到数组第几行第几列
            mCoins[row][col] =btn;

            int x = col *colWidth + xOffset;      //x坐标 = 列数 * 列宽 +x偏移
            int y = row *rowHeight +yOffset;      //y坐标 = 行数 * 行高 +y偏移
            btn->setGeometry(x,y,50,50);

            //设置row行col列的值，从dataConfig的数据中获取
            //btn->setStat(1);
            btn->setStat(dataArray[row][col]);

            //当每次点击硬币的时候就改变状态，状态置返
            connect(btn,&CoinButton::clicked,[=](){
                //btn->flip();        //进行状态置反
                this->flip(row,col);    //翻动某行某列的一个按钮   点击之后，调用flip传入一个第几行第几列的参数，下面续
            });
        }
    }
}

void PlayScene::flip(int row, int col)
{
    //如果胜利了，就跳出flip函数
    if(mHasWin)
    {
        return;
    }

    this->mCoins[row][col]->flip(); //点击之后，调用flip传入一个第几行第几列的参数，然后就在mCoins中将这个参数翻动

    //翻金币时 音效
    QSound::play(":/res/ConFlipSound.wav");

    //翻动上下左右的硬币
    //翻完中心那个之后，延时翻动其他硬币
    QTimer::singleShot(250,[=](){

        //下一行的硬币翻动
        if(row+1<4)
        {
            this->mCoins[row+1][col]->flip();       //单单如此,没有if判断进行保护的话，点击边边的按钮时，会发生数组越界，导致崩溃
        }
        //上一行的硬币翻动
        if(row-1>=0)
        {
            this->mCoins[row-1][col]->flip();
        }
        //左一列的硬币翻动
        if(col-1>=0)
        {
            this->mCoins[row][col-1]->flip();
        }
        //右一列的硬币翻动
        if(col+1<4)
        {
            this->mCoins[row][col+1]->flip();
        }


        //---判断游戏是否胜利
        this->judegeWin();
    });

}

void PlayScene::judegeWin()
{
    //16个硬币按钮判断状态都是1
    for(int row =0;row<4;++row)
    {
        for(int col=0;col<4;++col)
        {
            //判断某个硬币是硬币
            if(!this->mCoins[row][col]->stat())
            {
                //只要有一个是银币，就返回，游戏继续
                return;
            }
        }
    }
    //所有都是金币的情况下就胜利
    //QMessageBox::information(this,"恭喜","你已经胜利了！");

    //播放胜利之前将胜利状态更改
    mHasWin=true;

    //游戏胜利时音效
    QSound::play(":/res/LevelWinSound.wav");

    //播放胜利动画
    QLabel *labelWin=new QLabel(this);
    QPixmap pix=QPixmap(":/res/LevelCompletedDialogBg.png");
    labelWin->setPixmap(pix);
    labelWin->resize(pix.size());   //设置label大小和图片大小相同
    labelWin->show();
    labelWin->move((this->width()-labelWin->width())/2,-labelWin->height());        //水平居中，垂直方向将图片设置为负高度，就可以隐藏在顶上

    QPropertyAnimation *animation =new QPropertyAnimation(labelWin,"geometry",this);    //(动画目标，属性，对象树)
    animation->setStartValue(labelWin->geometry());                                     //开始的位置大小
    animation->setEndValue(QRect(labelWin->x(),labelWin->y()+150,labelWin->width(),labelWin->height()));    //x,w,h都不变，y方向 向下移动150像素
    animation->setDuration(1000);   //播放时长
    //设置动画的运动曲线
    animation->setEasingCurve(QEasingCurve::OutBounce);
    //动画播放完，自动删除动画对象
    animation->start(QAbstractAnimation::DeleteWhenStopped);
}

void PlayScene::paintEvent(QPaintEvent *event)
{
    //绘制背景图片
    QPainter painter(this);
    //搬动画家
    painter.translate(0,this->menuBar()->height());     //搬动画家到菜单项下面，不然背景会被遮住一部分
    //画背景图
    QPixmap pix(":/res/PlayLevelSceneBg.png");
    painter.drawPixmap(0,0,this->width(),this->height(),pix);
    //添加第二张照片，绘制logo
    pix.load(":/res/Title.png");
    pix  = pix.scaled(pix.width()/2,pix.height()/2);        //进行缩放
    painter.drawPixmap(0,0,pix);
}
