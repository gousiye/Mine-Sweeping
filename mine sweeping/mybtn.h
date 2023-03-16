#ifndef MYBTN_H
#define MYBTN_H

#include <QWidget>
#include <QPushButton>
#include <QEvent>
#include "QResizeEvent"
#include "QMouseEvent"
#include <QPainter>
#include <QPixmap>
#include <QFont>
#include <QMouseEvent>
#include <playarea.h>
#include <QMouseEvent>
#include <QApplication>

class PlayArea;

class MyBtn : public QPushButton
{
    Q_OBJECT

private:
    int posx,posy,scale;
    int nx,ny;
    int status=10;
    PlayArea *fatherscene;
    int count_right_button=0;

public:
   // explicit MyBtn(QWidget *parent = nullptr);
    bool isopen=0;
    int  RightClickedNum=0;
    void open();
    MyBtn (int scale_width,int scale_height,int x,int y,int numx,int numy,PlayArea *p=nullptr);
    void setPos(int scale_width,int scale_height,int x,int y,int numx,int numy);
    int &line_status();
    void setcolor();
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);
   // void paintEvent(QPaintEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    //void mouseMoveEvent(QMouseEvent *e);

signals:
    void  start(int px,int py);
    void  Rstart(int px,int py);
    void  Mclicked(int px,int py);
    void  Mrelease(int px,int py);
    void  toMakeup(int px,int py);
    void  Rightclicked();
    void  setflaged();

public slots:
    void  emitstart();
    void  emitRstart();
};

#endif // MYBTN_H
