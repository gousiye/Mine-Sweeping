#include "selectwidget.h"
#include "ui_selectwidget.h"
#include <QFont>
#include <QtDebug>
#include "mainwindow.h"

SelectWidget::SelectWidget(QWidget *parent,int nx,int ny,int mn,double mp,MainWindow *past) :
    QMainWindow(parent),scalx(nx),scaly(ny),mine_num(mn),mine_per(mp),
    ui(new Ui::SelectWidget)
{
    ui->setupUi(this);
    this->setWindowTitle("难度选项");
    this->setWindowIcon(QIcon(QPixmap(":/img/resources/Mine.png")));
    this->setAttribute(Qt::WA_DeleteOnClose);
    QFont ft,ftext;
    ft.setPointSize(15);
    ftext.setPointSize(13);
    //标签字体的设计
    ui->lalH->setFont(ft);
    ui->lalW->setFont(ft);
    ui->lalMnum->setFont(ft);
    ui->lalMm->setFont(ft);

    //文本框的初始化
    ui->show1->setVal(scalx);    ui->show1->setFont(ftext);
    ui->show2->setVal(scaly);    ui->show2->setFont(ftext);
    ui->show3->setVal(mine_num);    ui->show3->setFont(ftext);
    ui->show4->setVal(mine_per,1);    ui->show4->setFont(ftext);
    scalx=ui->show1->getVal();  scaly=ui->show2->getVal();

    ui->show1->BtnConnection(); ui->show1->LineConnection();  ui->show1->setlimit(4,100);
    ui->show2->BtnConnection(); ui->show2->LineConnection();  ui->show2->setlimit(4,100);
    ui->show3->BtnConnection(); ui->show3->LineConnection();  ui->show3->setlimit(1,54);
    ui->show4->setdouble(1);    ui->show4->BtnConnection(1);ui->show4->LineConnection(1);
    ui->show4->setlimit(100.0/64,5400.0/64,1);

    //各文本框信号的槽的建立
    buildconnect();

    //开始和取消按钮
    connect(ui->sBtn,&QPushButton::clicked,this,[=](){
        MainWindow *mainwindow=new MainWindow(nullptr,scalx,scaly,mine_num,mine_per);
        mainwindow->setGeometry(this->geometry());
        past->close();
        past->getPlayArea()->close();
        this->close();
        mainwindow->show();
    });
    connect(ui->cBtn,&QPushButton::clicked,this,[=](){
        past->setGeometry(this->geometry());
        this->close();
        past->show();
    });

}

SelectWidget::~SelectWidget()
{
    delete ui;
}


void SelectWidget::buildconnect(){
    //水平选项
    connect(this->ui->show1,&MultiControl::valuechanged,ui->show4,[=](){
        this->scalx=ui->show1->getVal();
        this->ui->show3->setlimit(1,scalx*scaly-10);
        if(mine_num>=scalx*scaly-10){
            mine_num=scalx*scaly-10;
            ui->show3->setlimit(1,scalx*scaly-10);
            ui->show4->setlimit(1.0/(scalx*scaly)*100,(scalx*scaly-10)*1.0/(scalx*scaly)*100,1);
            ui->show3->setVal(scalx*scaly-10);
            ui->show4->setVal((scalx*scaly-10)*100.0/(scalx*scaly),1);
        }
        else{
            double tmp=this->mine_num*100*1.0/(scalx*scaly);
            ui->show4->setlimit(1.0/(scalx*scaly)*100,(scalx*scaly-10)*1.0/(scalx*scaly)*100,1);
            ui->show4->setVal(tmp,1);
        }
    });

    //垂直选项
    connect(this->ui->show2,&MultiControl::valuechanged,ui->show4,[=](){
        this->scaly=ui->show2->getVal();
        this->ui->show3->setlimit(1,scalx*scaly-10);
        if(mine_num>=scalx*scaly-10){
            mine_num=scalx*scaly-10;
            ui->show3->setlimit(1,scalx*scaly-10);
            ui->show3->setVal(scalx*scaly-10);
            ui->show4->setVal((scalx*scaly-10)*100.0/(scalx*scaly),1);
        }
        else{
            double tmp=this->mine_num*100*1.0/(scalx*scaly);
            ui->show4->setlimit(1.0/(scalx*scaly)*100,(scalx*scaly-10)*1.0/(scalx*scaly)*100,1);
            ui->show4->setVal(tmp,1);
        }
    });

    //雷的数量的选项
    connect(this->ui->show3,&MultiControl::valuechanged,ui->show4,[=](){
        this->mine_num=ui->show3->getVal();
        double tmp=this->mine_num*100*1.0/(scalx*scaly);
        ui->show4->setVal(tmp,1);
    });

    //雷的百分比的选项
    connect(this->ui->show4,&MultiControl::valuechanged,ui->show3,[=](){
        this->mine_per=ui->show4->getVal(1);
        double tmp=this->mine_per/100*(scalx*scaly);
        this->mine_num=tmp;
        ui->show3->setVal(tmp);
    });
}


void SelectWidget::paintEvent(QPaintEvent *){
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/img/resources/background.png");
    painter.drawPixmap(0,0,this->width(),this->height(),pix);
}








