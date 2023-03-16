#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "QPushButton"
#include "mybtn.h"
#include <QResizeEvent>
#include <QMenuBar>
#include <QScrollArea>
#include <playarea.h>
#include <QSet>
#include <QStatusBar>
#include "restart.h"
#include "QTimer"
#include "QPainter"

class PlayArea;
class Restart;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    friend class Restart;
    MainWindow(QWidget *parent = nullptr,int nx=8,int ny=8,int mn=10,double mp=10*100.0/64,QSet<int> smt=sm);
    ~MainWindow();
    void  resizeEvent(QResizeEvent *e);
    void  initialize_Menu();
    void  initialize_Status();
    void  initialize_Signal();
    void  SetTime();
    void  reset(int nx,int ny,int mn,double mp);
    PlayArea * getPlayArea ();

private:
    static QSet<int> sm;
    Ui::MainWindow *ui;
    int numx,numy;                          //行列的方块个数
    int mine_num;   double mine_per;                      //类的数据
    int s=0,m=0,h=0;                            //时分秒
   // QVector<QVector<MyBtn*>> grid_config;              // 存放各个按钮
    QSet<int> sms;
    QMenuBar *bar;
    QStatusBar *stBar;
    QMenu *file,*option;
    QScrollArea *area;
    PlayArea *p;
    QTimer *timer;
    QLabel *labTime=nullptr,*labMine=nullptr,*labNum=nullptr;
    void paintEvent(QPaintEvent *);

public slots:
    void askrestart(bool iswin);
};
#endif // MAINWINDOW_H





















