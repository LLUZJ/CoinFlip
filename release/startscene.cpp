#include "startscene.h"
#include<QPushButton>
#include"mypushbutton.h"
#include<QTimer>
#include<QSound>

StartScene::StartScene(QWidget *parent)
    : MyMainWindow{parent}
{
    this->setWindowTitle("开始场景");

    //------创建Start按钮       //通过传参的方式传入两个图片，一个是正常normal一个是按压按钮pressed的
    MyPushButton *btnStart =new MyPushButton(":/res/MenuSceneStartButton.png",":/res/MenuSceneStartButton.png",this);
    btnStart->resize(114,114);
    //按钮水平居中，垂直方向在3/4位置
    //即x= 窗口宽度/2 -按钮宽度/2 , y=窗口高度*3/4 -按钮高度/2
    btnStart->move((this->width()-btnStart->width())/2,
                   this->height()*3/4-btnStart->height()/2);

    //------设置开始按钮动画
    connect(btnStart,&MyPushButton::clicked,[=](){

        //开始按钮点击音效
        QSound::play(":/res/TapButtonSound.wav");

        //按下按钮之后，设置按钮为不可用，---因为避免当按键速度过快，会影响动画播放，即前两个动画还没播放完就点击了按钮，又重新播放，可以把按钮固定在原处
        btnStart->setEnabled(false);

        //播放向下动画
        btnStart->moveDown();
        //往上走的动画，会覆盖向下的动画
        //所以需要等待之前动画播放完 在继续调用
        QTimer::singleShot(150,[=](){           //单发的定时器函数，不需要声明，singleShot只执行一次（时间，槽）
            btnStart->moveUp();
        });

        QTimer::singleShot(350,[=](){
            //将按钮重新设置为可用
            btnStart->setEnabled(true);

            //-------场景切换
            //隐藏当前窗口
            this->hide();
            //显示第二个窗口
            this->mSelectScene.show();      //因为存在内存释放问题，所以切换第二个场景时，在第一个场景中声明这个成员变量

            //将关卡选择界面窗口移动到当前窗口的位置
            this->mSelectScene.move(this->pos());
        });
    });

    //第二个窗口返回按钮被点击
    connect(&this->mSelectScene,&SelectScene::backBtnClicked,[=](){

        //关卡选择界面 返回按钮点击音效
        QSound::play(":/res/BackButtonSound.wav");

        //当第二个窗口的返回按钮被点击时，自定义信号传过来
        this->show();               //开始场景显示
        this->mSelectScene.hide();  //关卡选择场景隐藏

        //返回时，将开始界面窗口移动到当前窗口
        this->move(this->mSelectScene.pos());
    });
}
