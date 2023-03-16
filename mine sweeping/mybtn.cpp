#include "mybtn.h"
#include <QtMath>


MyBtn::MyBtn(int scale_width,int scale_height,int x,int y,int numx,int numy,PlayArea *p)
{
    fatherscene=p;
    setMouseTracking(true);
    this->setStyleSheet("background-color: rgb(186,189,182); border:none");
    nx=x,ny=y;
    setPos(scale_width,scale_height,x,y,numx,numy);
}

void MyBtn::open(){
    this->isopen=1;
    fatherscene->countBtn+=1;
}

void MyBtn::emitstart(){
    //qDebug()<<"sdsd";
    emit start(nx,ny);
}

void MyBtn::emitRstart(){
    emit Rstart(nx,ny);
}


int& MyBtn::line_status(){
    return this->status;
}


void MyBtn::setPos(int scale_width, int scale_height, int x, int y, int numx, int numy){
    QFont ft;
    int tmp;
    tmp=qMin((scale_height/(1.05*numy)),scale_width/(1.05*numx));    //边长
    scale=qMax(45,tmp);
    //scale-=3;
    ft.setPointSize(scale*0.7);
    this->setFont(ft);
    posx=(scale_width-(numx)*scale)/2+(x-1)*(scale);                   //居中
    posy=(scale_height-(numy)*scale)/2+(y-1)*(scale);
    this->setGeometry(posx,posy,scale*19.0/20,scale*19.0/20);
    //设置背景图片
    if(this->status==-1 && fatherscene->get_over()){
        QPixmap pix;
        pix.load(":/img/resources/Mine.png");
        this->setIcon(pix);
        this->setIconSize(QSize(this->width(),this->height()));
    }
}

void MyBtn::setcolor(){
    switch (this->status) {
    case 0:
        this->setStyleSheet("background-color: rgb(222,222,220); border:none");
        break;
    case 1:
        this->setStyleSheet("background-color: rgb(221,250,195); border:none");
        break;
    case 2:
        this->setStyleSheet("background-color: rgb(236,237,191); border:none");
        break;
    case 3:
        this->setStyleSheet("background-color: rgb(237,218,180); border:none");
        break;
    case 4:
        this->setStyleSheet("background-color: rgb(237,195,138); border:none");
        break;
    case 5:
        this->setStyleSheet("background-color: rgb(247,161,162); border:none");
        break;
    case 6:
        this->setStyleSheet("background-color: rgb(254,167,133); border:none");
        break;
    case 7:
        this->setStyleSheet("background-color: rgb(255,125,96); border:none");
        break;
    case 8:
        this->setStyleSheet("background-color: rgb(134,95,197); border:none");
        break;
    }
}

void MyBtn::enterEvent(QEvent *e){
    //emit Mclicked(nx,ny);
    if(!isopen)
        this->setStyleSheet("background-color: rgb(155,250,249); border:none");
    //if(Qt::MidButton)
    //  emit Mclicked(nx,ny);
    QPushButton::enterEvent(e);
}

void MyBtn::leaveEvent(QEvent *e){
    QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(e);
    if(!isopen)
        setStyleSheet("background-color: rgb(186,189,182); border:none");
    else if(isopen && this->status==-1 && !fatherscene->get_over())
        this->setStyleSheet("background-color: rgb(136,138,133); border:none");
    QPushButton::enterEvent(e);
}

//设置右键的信号
void MyBtn::mousePressEvent(QMouseEvent *e){
    if(!fatherscene->get_over()){
        if(e->button()==Qt::LeftButton && this->isopen){
            emit Mclicked(nx,ny);
        }
        if(e->button()==Qt::RightButton){
            if(!isopen)  RightClickedNum=(RightClickedNum+1)%3;
            if(RightClickedNum==1)   fatherscene->flaged+=1;
            else if(RightClickedNum==2) {
                fatherscene->flaged-=1;
                if(fatherscene->start && this->status==-1)
                     fatherscene->countFlag-=1;
            }
            emit setflaged();
            emit Rightclicked();
            if(RightClickedNum==1 && this->status==-1)
                fatherscene->countFlag+=1;
            qDebug()<<fatherscene->countFlag;
            if(fatherscene->countFlag==fatherscene->get_mine_num() && fatherscene->flaged==fatherscene->get_mine_num())
                fatherscene->game_over(1,1,1);
        }
        if(e->button()==Qt::MidButton){
            emit Mclicked(nx,ny);
        }
    }
    QPushButton::mousePressEvent(e);
}


void MyBtn::mouseReleaseEvent(QMouseEvent *e){
    if(!fatherscene->get_over()){
        if(e->button()==Qt::MidButton){
            emit Mrelease(nx,ny);
            emit toMakeup(nx,ny);
        }
        if(e->button()==Qt::LeftButton && this->isopen){
            emit Mrelease(nx,ny);
            emit toMakeup(nx,ny);
        }
    }
    QPushButton::mouseReleaseEvent(e);
}









































