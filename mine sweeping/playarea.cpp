#include "playarea.h"
#include "ui_playarea.h"
#include <QPen>


PlayArea::PlayArea(QWidget *parent,int &nx,int &ny,int &mine_num,double &mine_per,QSet<int>& ts) :
    QWidget(parent),numx(nx),numy(ny),mine_num(mine_num),mine_per(mine_per),mine_grid(ts),
    ui(new Ui::PlayArea)
{
    ui->setupUi(this);
    pause=200*0.5*numx;
    ui->Blue->setGeometry(0,0,0,0);
    initializeBtn1();
    PlayProcedure();
}



void PlayArea::initializeBtn1(){
    this->setAttribute(Qt::WA_DeleteOnClose);
    QVector<MyBtn*> row_congig;
    for(int i=1;i<=numx;i++){
        for(int j=1;j<=numy;j++){
            MyBtn *btn=new MyBtn(this->width(),this->height(),i,j,numx,numy,this);
            btn->setParent(this);                      //对象树了不用析构
            connect(btn,&MyBtn::clicked,btn,&MyBtn::emitstart);
            connect(btn,&MyBtn::Rightclicked,btn,&MyBtn::emitRstart);
            connect(btn,&MyBtn::start,this,&PlayArea::initializeMine);
            connect(btn,&MyBtn::start,this,&PlayArea::BFS_Search);
            connect(btn,&MyBtn::Rstart,this,&PlayArea::Mark);
            connect(btn,&MyBtn::Mclicked,this,&PlayArea::Msetcolor);
            connect(btn,&MyBtn::Mrelease,this,&PlayArea::Mbackcolor);
            connect(btn,&MyBtn::toMakeup,this,&PlayArea::Makeup);
            connect(btn,&MyBtn::setflaged,this,[=](){emit checkflag();});
            row_congig.push_back(btn);                 //行向量
        }
        grid_config.push_back(row_congig);                   //二维向量
        row_congig.clear();
    }
    first=grid_config[0][0];
}

void PlayArea::posBtn(int i, int j){
    grid_config[i-1][j-1]->setPos(this->width(),this->height(),i,j,numx,numy);
}

void PlayArea::paintEvent(QPaintEvent *){
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/img/resources/background.png");
    painter.drawPixmap(0,0,this->width(),this->height(),pix);
}


void PlayArea::initializeMine(int px,int py){
    if(mine_grid.size()==0) {
        if(isfirst){
            isfirst=0;
            // qDebug()<<px<<py;
            int tempx,tempy,judge_insert;
            qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
            while(mine_grid.size()<mine_num)
            {
                tempx=qrand()%(numx)+1;
                tempy=qrand()%(numy)+1;
                if(abs(tempx-px)>=2 || abs(tempy-py)>=2 ){
                    judge_insert=tempx*1000+tempy;
                    mine_grid.insert(judge_insert);
                    grid_config[tempx-1][tempy-1]->line_status()=-1;
                }
                // qDebug()<<mine_grid.size();
            }
        }
    }
    else{
        QSet<int>::iterator it;
        for(it=mine_grid.begin();it!=mine_grid.end();it++){
            int tempx=*it/1000,tempy=*it-tempx*1000;
            //            qDebug()<<tempx<<tempy;
            grid_config[tempx-1][tempy-1]->line_status()=-1;
        }
    }
    //    qDebug()<<mine_grid.size();
    emit isstart();
    start=1;                              //初始化好了雷，开始进入游戏
    for(int i=1;i<=numx;i++){
        for(int j=1;j<=numy;j++){
            disconnect(grid_config[i-1][j-1],&MyBtn::start,this,&PlayArea::initializeMine);
        }
    }
}

void PlayArea::initialzeBtn2(){
    for(int i=1;i<=numx;i++){
        for(int j=1;j<=numy;j++){
            int mine_count=0;
            if(grid_config[i-1][j-1]->line_status()!=-1){
                for(int dirx=-1;dirx<=1;dirx++){
                    for(int diry=-1;diry<=1;diry++){
                        int tx=i+dirx,ty=j+diry;
                        if(tx>=1 && tx<=numx && ty>=1 && ty<=numy && grid_config[tx-1][ty-1]->line_status()==-1)
                            mine_count+=1;
                    }
                }
                grid_config[i-1][j-1]->line_status()=mine_count;
            }
        }
    }
}

void PlayArea::BFS_Search(int px, int py){
    // game_over(1,1);
    //return ;
    if(this->isover)   return ;
    if(grid_config[px-1][py-1]->isopen || grid_config[px-1][py-1]->RightClickedNum==1)   return ;
    if(grid_config[px-1][py-1]->line_status()==-1)     { game_over(px,py);   return ;}
    QQueue <int> q;
    q.push_back(px*1000+py);
    while(!q.empty()){
        //int tp=q.front();
        // qDebug()<<tp;
        int tempx=qMin(q.front()/1000,1000),tempy=qMin(q.front()-1000*tempx,1000);
        //qDebug()<<tempx<<tempy;
        q.pop_front();
        MyBtn *bt=grid_config[tempx-1][tempy-1];
        bt->open();
        bt->setcolor();;        //翻开了新的
        if(bt->RightClickedNum==2)              // ?特判
            bt->setIcon(QPixmap(""));
        int val=bt->line_status();
        //qDebug()<<val;
        if(val!=0 && val!=-1) bt->setText(QString::number(val));
        // grid_config[px-1][py-1]->setcolor();
        if(bt->RightClickedNum==2)
            bt->setIcon(QPixmap(""));
        if(val!=0)  {
            //qDebug()<<countBtn;
            if(countBtn== numx*numy-mine_num)
                game_over(1,1,1);
            return ;
        }
        for(int dirx=-1;dirx<=1;dirx++){
            for(int diry=-1;diry<=1;diry++){
                if(dirx==0 && diry==0)   continue;
                int tx=tempx+dirx,ty=tempy+diry;
                if(tx>=1 && tx<=numx && ty>=1 && ty<=numy &&!grid_config[tx-1][ty-1]->isopen
                        && grid_config[tx-1][ty-1]->RightClickedNum!=1){
                    MyBtn *btb=grid_config[tx-1][ty-1];
                    int val=grid_config[tx-1][ty-1]->line_status();
                    btb->setcolor();
                    if(btb->RightClickedNum==2)
                        btb->setIcon(QPixmap(""));
                    if(val>0){
                        btb->open();
                        btb->setText(QString::number(val));
                    }
                    else if(val==0){
                        q.push_back(tx*1000+ty);
                        btb->open();
                        countBtn-=1;
                    }
                }
            }
        }
    }
    if(countBtn== numx*numy-mine_num)
        game_over(1,1,1);
    // qDebug()<<countBtn;
    return ;
}


void PlayArea::Mark(int px, int py){
    if(this->isover)   return ;
    MyBtn * tb=grid_config[px-1][py-1];
    if(!tb->isopen && tb->RightClickedNum==1){
        QPixmap pix;

        pix.load(":/img/resources/Flag.png");
        tb->setIcon(pix);
        tb->setIconSize(QSize(tb->width(),tb->height()));
    }
    if(!tb->isopen && tb->RightClickedNum==2){
        QPixmap pix;
        pix.load(":/img/resources/questionmark.png");
        tb->setIcon(pix);
        tb->setIconSize(QSize(tb->width(),tb->height()));
    }
    if(!tb->isopen && tb->RightClickedNum==0){
        QPixmap pix;
        pix.load("");
        tb->setIcon(pix);
        tb->setIconSize(QSize(tb->width(),tb->height()));
    }
}

void PlayArea::Msetcolor(int px, int py){
    if(this->isover)   return;
    MyBtn *bt=grid_config[px-1][py-1];
    for(int dirx=-1;dirx<=1;dirx++){
        for(int diry=-1;diry<=1;diry++){
            int tx=px+dirx,ty=py+diry;
            if(tx>=1 && tx<=numx && ty>=1 && ty<=numy){
                MyBtn *tbt=grid_config[tx-1][ty-1];
                if(!tbt->isopen  && tbt->RightClickedNum!=1)
                    tbt->setStyleSheet("background-color: rgb(222,222,220); border:none");
            }
        }
    }
}

void PlayArea::Mbackcolor(int px, int py){
    if(this->isover)  return;
    MyBtn *bt=grid_config[px-1][py-1];
    for(int dirx=-1;dirx<=1;dirx++){
        for(int diry=-1;diry<=1;diry++){
            int tx=px+dirx,ty=py+diry;
            if(tx>=1 && tx<=numx && ty>=1 && ty<=numy){
                MyBtn *tbt=grid_config[tx-1][ty-1];
                tbt->setcolor();
                if(!tbt->isopen)
                    tbt->setStyleSheet("background-color: rgb(186,189,182); border:none");
            }
        }
    }
}

void PlayArea::Makeup(int px, int py){
    if(this->isover)   return ;
    int toover=0,first=1;
    if(grid_config[px-1][py-1]->RightClickedNum!=0 || !grid_config[px-1][py-1]->isopen || isover )
        return ;
    QVector <int> search,forBfs;
    for(int dirx=-1;dirx<=1;dirx++){
        for(int diry=-1;diry<=1;diry++){
            int tx=px+dirx,ty=py+diry;
            if(tx>=1 && tx<=numx && ty>=1 && ty<=numy){
                if(tx==px && ty==py )  continue;
                MyBtn *bt=grid_config[tx-1][ty-1];
                if(first && bt->line_status()==-1 && bt->RightClickedNum!=1){
                    toover=1000*tx+ty;    first=0;
                }
                if(!bt->isopen &&  bt->RightClickedNum==1)   search.push_back(tx*1000+ty);
                if(!bt->isopen && bt->RightClickedNum!=1)
                    forBfs.push_back(tx*1000+ty);
            }
        }
    }
    // qDebug()<<search.size();
    if(search.size()==grid_config[px-1][py-1]->line_status()){
        for(int i=0;i<search.size();i++){
            int tempx=search[i]/1000;   int tempy=search[i]-tempx*1000;
            // qDebug()<<tempx<<tempy;
            if(grid_config[tempx-1][tempy-1]->line_status()!=-1){
                game_over(toover/1000,toover-1000*(toover/1000));
                return ;
            }
            // qDebug()<<forBfs.size();
            for(int i=0;i<forBfs.size();i++)
                BFS_Search(forBfs[i]/1000,forBfs[i]-1000*(forBfs[i]/1000));
        }
    }
}

void PlayArea::PlayProcedure(){
    connect(this,&PlayArea::isstart,this,&PlayArea::initialzeBtn2);
}

void PlayArea::game_over(int px, int py,bool win){
    if(isover)  return;
    emit over();   this->isover=1;
    if(win){
        blueposy=this->height()-40;
        ui->Blue->setGeometry(0,blueposy,this->width(),40);
        QTimer *Btimer=new QTimer(this);
        Btimer->start(20);
        connect(Btimer,&QTimer::timeout,this,&PlayArea::drawblueline);
        iswin=1;
        QTime time;
        time.start();
        while(time.elapsed() <pause){
            QCoreApplication::processEvents();
            connect(this,&PlayArea::stop_pause,this,[=](){pause=0;});
            if(pause==0)   break;
        }
    }
    for(int i=1;i<=numx;i++){
        for(int j=1;j<=numy;j++){
            if(!grid_config[i-1][j-1]->isopen){
                if(grid_config[i-1][j-1]->line_status()!=-1){
                    grid_config[i-1][j-1]->setcolor();
                    if(grid_config[i-1][j-1]->line_status()!=0)
                        grid_config[i-1][j-1]->setText(QString::number(grid_config[i-1][j-1]->line_status()));
                    if(grid_config[i-1][j-1]->RightClickedNum==1){
                        grid_config[i-1][j-1]->setIcon(QPixmap(""));
                        grid_config[i-1][j-1]->setStyleSheet("background-color: rgb(255,0,0); border:none");
                    }
                    if(grid_config[i-1][j-1]->RightClickedNum==2)
                        grid_config[i-1][j-1]->setIcon(QPixmap(""));
                }
                else{
                    QPixmap pix;
                    grid_config[i-1][j-1]->setStyleSheet("background-color: rgb(136,138,133); border:none");
                    if(grid_config[i-1][j-1]->RightClickedNum==0)
                        pix.load(":/img/resources/Mine.png");
                    else if(grid_config[i-1][j-1]->RightClickedNum==1)
                        pix.load(":/img/resources/MF.png");
                    else
                        pix.load(":/img/resources/MQ.png");
                    grid_config[i-1][j-1]->setIcon(pix);
                    grid_config[i-1][j-1]->setIconSize(QSize(grid_config[i-1][j-1]->width(),grid_config[i-1][j-1]->height()));
                }
            }
            grid_config[i-1][j-1]->open();
        }
    }
    if(win!=1)
        grid_config[px-1][py-1]->setStyleSheet("background-color: rgb(255,0,0)");
    emit torestart(win);
}


void PlayArea::drawblueline(){
    if(blueposy>-50){
        ui->Blue->setGeometry(0,blueposy,this->width(),40);
        QPixmap pix;
        pix.load(":/img/resources/blueliine.png");
        pix=pix.scaled(this->width(),blueposy,Qt::IgnoreAspectRatio);
        ui->Blue->setPixmap(pix);
        blueposy-=20;
    }
}

void PlayArea::mousePressEvent(QMouseEvent *event){
    if(event->button()==Qt::LeftButton && isover)
        emit stop_pause();
    QWidget::mousePressEvent(event);
}

bool PlayArea::get_over(){
    return this->isover;
}

QSet<int> &PlayArea::get_mine_grid(){
    return this->mine_grid;
}

int PlayArea::get_mine_num(){return mine_num;}

PlayArea::~PlayArea()
{
    delete ui;
}















