#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mybtn.h"
#include <QVector>
#include <QAction>
#include <QScrollArea>
#include <QScrollBar>
#include <selectwidget.h>
#include <playarea.h>

QSet<int> MainWindow::sm;

MainWindow::MainWindow(QWidget *parent,int nx,int ny,int mn,double mp,QSet<int> smt)
    : QMainWindow(parent),numx(nx),numy(ny),mine_num(mn),mine_per(mp),sms(smt),
      ui(new Ui::MainWindow)
{
    //默认初级难度, PlatArea 实现滚动窗口
    ui->setupUi(this);
    this->setWindowTitle("扫雷");
    this->setWindowIcon(QIcon(QPixmap(":/img/resources/Mine.png")));
    area=new QScrollArea(this);
    area->setGeometry(0,26,this->width(),this->height()-80);
    area->setFrameStyle(QFrame::Plain);
    p=new PlayArea(area,numx,numy,mine_num,mine_per,sms);
    p->setAttribute(Qt::WA_DeleteOnClose);
    p->setGeometry(0,0,this->width(),this->height()-80);
    p->move((this->width()-p->width())/2,0);
    area->setWidget(p);
    this->setMinimumSize(580,620);
    initialize_Menu();
    initialize_Status();
    connect(p,&PlayArea::torestart,this,&MainWindow::askrestart);
}
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent *ev){
    for(int i=1;i<=numx;i++){
        for(int j=1;j<=numy;j++){
            if(numx*45<this->width()-45 &&  numy*45<this->height()-45){
                area->setGeometry(0,26,this->width(),this->height()-80);
                p->setGeometry(0,0,this->width(),this->height()-80);
                p->posBtn(i,j);
            }
            else{
                area->setGeometry(0,26,this->width(),this->height()-80);
                p->setGeometry(0,0,numx*45+1920,numy*45+90);
                //p->fixdmove((this->width()-p->width())/2,26);
                p->posBtn(i,j);
                //总使游戏界面居中
                if(p->width()<this->width()){
                    int tx=(this->width()-p->width())/2;
                    area->setGeometry(tx,26,this->width()-tx,this->height()-80);
                }
                if(p->height()<this->height()){
                    int ty=(this->height()-p->height())/2;
                    area->setGeometry(0,ty+26,this->width(),this->height()-80-ty);
                }
            }
        }
    }
}

void MainWindow::reset(int nx, int ny,int mx,double mp){
    MainWindow *Scene=new MainWindow(nullptr,nx,ny,mx,mp);
    Scene->setGeometry(this->geometry());
    p->close();
    this->close();
    Scene->show();
}

PlayArea* MainWindow::getPlayArea(){
    return this->p;
}

void MainWindow::initialize_Menu(){
    bar=menuBar();
    file = bar->addMenu("文件");
    option = bar->addMenu("选项");
    QAction *Afile = file->addAction("退出");
    QAction *Ap =option->addAction("初级");
    QAction *Am =option->addAction("中级");
    QAction *Ah= option->addAction("高级");
    QAction *AD= option->addAction("自定义");
    setMenuBar(bar);
    connect(Afile,&QAction::triggered,this,[=](){
        this->close();
    });
    connect(Ap,&QAction::triggered,this,[=](){             //初级难度8*8
        reset(8,8,10,10.0/64);
    });
    connect(Am,&QAction::triggered,this,[=](){             //中级难度16*16
        reset(16,16,40,40.0/265);
    });
    connect(Ah,&QAction::triggered,this,[=](){             //高级难度30*16
        reset(30,16,99,99.0/480);
    });
    connect(AD,&QAction::triggered,this,[=](){             //自定义难度
        SelectWidget * choosescene= new SelectWidget(nullptr,numx,numy,mine_num,mine_per,this);
        choosescene->setGeometry(this->geometry());
        this->hide();
        choosescene->show();
    });
    connect(p,&PlayArea::isstart,this,[=](){
        QAction *AR = option->addAction("重新开始");              //重开
        connect(AR,&QAction::triggered,this,[=](){
            //qDebug()<<p->mine_grid.size();
            MainWindow *newWindow = new MainWindow(nullptr,numx,numy,mine_num,mine_per,p->get_mine_grid());
            newWindow->setGeometry(geometry());
            this->close();
            newWindow->show();
        });
    });
}


void MainWindow::initialize_Status(){

    timer=new QTimer(this);
    stBar= statusBar();
    stBar->setMinimumHeight(54);
    QFont ft;
    ft.setPointSize(20);
    setStatusBar(stBar);
           labMine=new QLabel(this);
           labNum=new QLabel(this);
           labTime=new QLabel(this);

          //雷图标初始化
            QPixmap pix1;
            pix1.load(":/img/resources/Mine.png");
            pix1=pix1.scaled(QSize(40,40),Qt::IgnoreAspectRatio);
            labMine->setPixmap(pix1);
            stBar->addWidget(labMine);


            //标记数量初始化
            labNum->setParent(this);     labNum->setFont(ft);
            QString tmp=QString("0 / %1").arg(mine_num);
            labNum->setText(tmp);
            stBar->addWidget(labNum);

            connect(p,&PlayArea::checkflag,this,[=](){
                QString str_mine=QString("%1 / %2").arg(p->flaged).arg(mine_num);
                labNum->setText(str_mine);
                stBar->addWidget(labNum);
            });

            //时间初始化
            labTime->setParent(this);    labTime->setFont(ft);
            labTime->setText("用时：0 时 0 分 0 秒");
            stBar->addPermanentWidget(labTime);
            connect(p,&PlayArea::isstart,this,[=](){
                timer->start(1000);
                connect(timer,&QTimer::timeout,this,[=](){SetTime();});
            });
            connect(p,&PlayArea::over,this,[=](){timer->stop();});
}

void MainWindow::SetTime(){
    s+=1;
    if(s==60)   {m+=1;s-=60;}
    if(m==60)    {h+=1;m-=60;}
    QString timetp=QString("用时： %1 时 %2 分 %3 秒").arg(h).arg(m).arg(s);
    labTime->setText(timetp);
    stBar->addPermanentWidget(labTime);
}

void MainWindow::paintEvent(QPaintEvent *){
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/img/resources/back.png");
    painter.drawPixmap(0,this->height()-54,this->width(),54,pix);
}

void MainWindow::askrestart(bool win){
    sms=p->get_mine_grid();
    Restart * rest=new Restart(nullptr,this,win);
    rest->setWindowModality(Qt::ApplicationModal);
    rest->show();

}
















