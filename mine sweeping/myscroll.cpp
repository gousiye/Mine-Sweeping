#include "myscroll.h"

myScroll::myScroll(QWidget *parent) : QScrollArea(parent)
{

}


void myScroll::setPlayArea(PlayArea *tp){
    p=tp;
}


void myScroll::wheelEvent(QWheelEvent *e){
   // p->move(200,200);
    QScrollArea::wheelEvent(e);
}
