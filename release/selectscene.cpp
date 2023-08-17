#include "selectscene.h"
#include<QPushButton>
#include<QPainter>
#include<QMenuBar>
#include"mypushbutton.h"
#include"playscene.h"
#include<QSound>

SelectScene::SelectScene(QWidget *parent)
    : MyMainWindow{parent}
{
    //返回按钮
    //------开关-关卡选择切换       //对MyPushButton进行复用，塞入对应的图片资源，进而去显示相应的图片
    MyPushButton *btnBack =new MyPushButton(":/res/BackButton.png",":/res/BackButtonSelected.png",this);
    btnBack->resize(72,32);

    connect(btnBack,&QPushButton::clicked,this,&SelectScene::backBtnClicked);       //信号连接信号，按钮按下时触发自定义信号

    //------back按钮
    btnBack->move(this->width()-btnBack->width(),this->height()-btnBack->height());

    //------20个关卡按钮
    const int colWidth  =70;    //列宽
    const int rowHeight =70;    //行高
    //x y偏移     原点偏移，不然按钮会怼到开始菜单那里
    const int xOffset =25;
    const int yOffset =130;

    for(int i = 0; i<20; ++i)
    {
        MyPushButton *btn =new MyPushButton(":/res/LevelIcon.png",":/res/LevelIcon.png",this);      //同样分为两个图片Normal和Pressed
        btn->setText(QString::number(i+1));
        //排列
        int row = i / 4;            //行 i / 4
        int col = i % 4;            //列 i % 4
        int x = col *colWidth + xOffset;      //x坐标 = 列数 * 列宽 +x偏移
        int y = row *rowHeight +yOffset;      //y坐标 = 行数 * 行高 +y偏移
        btn->resize(57,57);         //按钮大小
        btn->move(x,y);


        //------场景切换    关卡选择--游戏
        //这里因为每次选择的关卡不同，进入的数据不同，所以不使用之前那种成员变量的方式去切换场景，这里直接使用new的方式去创建
        connect(btn,&MyPushButton::clicked,[=](){

            //关卡选择按钮 音效
            QSound::play(":/res/TapButtonSound.wav");

            //点击关卡按钮后，打开一个新的场景PlayScene
            PlayScene *playScene =new PlayScene(i+1);        //存在内存释放问题
            //设置playScene关闭时自动释放
            playScene->setAttribute(Qt::WA_DeleteOnClose);

            //在关卡选择界面点击关卡后，将游戏界面窗口移动到当前窗口
            playScene->move(this->pos());

            playScene->show();
            this->hide();

            connect(playScene,&PlayScene::backBtnClicked,[=](){     //在选择场景中也需要关注游戏场景中的信号

                //游戏界面 返回按钮点击音效
                QSound::play(":/res/BackButtonSound.wav");

                //在游戏界面点击back按钮时，将关卡选择界面移动到当前窗口位置
                this->move(playScene->pos());

                this->show();
                //playScene->hide();        解决内存释放问题，所以这里不能隐藏游戏场景，要关闭掉
                playScene->close();
            });
        });
    }

}

void SelectScene::paintEvent(QPaintEvent *event)
{
    //绘制背景图片
    QPainter painter(this);
    //搬动画家
    painter.translate(0,this->menuBar()->height());     //搬动画家到菜单项下面，不然背景会被遮住一部分
    //画背景图
    QPixmap pix(":/res/OtherSceneBg.png");
    painter.drawPixmap(0,0,this->width(),this->height(),pix);
    //添加第二张照片，绘制logo
    pix.load(":/res/Title.png");
    painter.drawPixmap(0,0,pix);
}
