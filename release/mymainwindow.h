#ifndef MYMAINWINDOW_H
#define MYMAINWINDOW_H

#include <QMainWindow>


QT_BEGIN_NAMESPACE
namespace Ui { class MyMainWindow; }
QT_END_NAMESPACE

class MyMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MyMainWindow(QWidget *parent = nullptr);
    ~MyMainWindow();

private slots:
    void on_actionQuit_triggered();

private:
    Ui::MyMainWindow *ui;

protected:
    void paintEvent(QPaintEvent *event);
};
#endif // MYMAINWINDOW_H
