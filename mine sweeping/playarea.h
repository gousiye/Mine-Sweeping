#ifndef PLAYAREA_H
#define PLAYAREA_H

#include <QWidget>
#include <QPushButton>
#include <QVector>
#include <mybtn.h>
#include <QPainter>
#include <QTime>
#include <QtGlobal>
#include <QSet>
#include <QDebug>
#include <QtMath>
#include <QPixmap>
#include <QQueue>
#include <QMessageBox>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QMoveEvent>
#include <QTime>
#include "restart.h"

class MyBtn;

namespace Ui {
class PlayArea;
}

class PlayArea : public QWidget
{
    Q_OBJECT

public:
    bool start=0,isover=0;
    int flaged=0;               //显示标记的旗子数量
    int countBtn=0,countFlag=0;    //计算已出现的方块或旗子
    explicit PlayArea(QWidget *parent,int &nx,int &ny,int &mine_num,double &mine_per,QSet<int> &ts);
    void initializeBtn1();
    void  posBtn(int i,int j);

    //游戏开始了
    void initialzeBtn2();
    void initializeMine(int px,int py);
    void initializeMinerestart();
    int calcualte_mine(int px,int py);
    void BFS_Search(int px,int py);     //鼠标左键
    void Mark(int px,int py);    //鼠标右键
    void Msetcolor (int px,int py);  //鼠标中键显示周围的格子
    void Mbackcolor(int px,int py);   //还原鼠标中键的变色
    void Makeup(int px,int py);    //中键，左键补齐
    void PlayProcedure();
    void paintEvent(QPaintEvent *);

    //游戏重新开始
    void game_over(int px,int py,bool win=0);
    void drawblueline();
    void mousePressEvent(QMouseEvent *event);
    bool get_over();
    QSet<int> & get_mine_grid();
    int get_mine_num();
    ~PlayArea();

private:
    int pause;   //暂停时间
    QTimer *Btimer;
    bool isfirst=1,iswin=0;
    int blueposy;
    int &numx,&numy,&mine_num;
    double &mine_per;
    QVector<QVector<MyBtn*>> grid_config;
    QSet<int> mine_grid;
    MyBtn *first;
    Ui::PlayArea *ui;

signals:
    void isstart();
    void torestart(bool iswin);
    void checkflag();
    void over();
    void stop_pause();
};

#endif // PLAYAREA_H














