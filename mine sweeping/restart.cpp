#include "restart.h"
#include "ui_restart.h"

Restart::Restart(QWidget *parent, MainWindow *past,bool iswin) :
    QWidget(parent),
    ui(new Ui::Restart)
{
    ui->setupUi(this);

    setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
    setWindowModality(Qt::WindowModal);
    this->setWindowIcon(QIcon(QPixmap(":/img/resources/Mine.png")));
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setWindowTitle("游戏结束");
    this->setFixedSize(700,400);
    QFont ft1,ft2;
    ft1.setPointSize(40);
    ft2.setPointSize(14);
    ui->label->setFont(ft1);

    //按钮的字体大小
    ui->restartBtn->setFont(ft2);
    ui->optionBtn->setFont(ft2);
    ui->newBtn->setFont(ft2);
    ui->CancelBtn->setFont(ft2);

    if(iswin){
        QString  tex=QString("游戏结束，扫雷成功!!!").arg(past->numx).arg(past->numy).arg(past->mine_num);
        ui->label->setText(tex);
    }

    BuildBtnConnection(past);
}

void Restart::BuildBtnConnection(MainWindow *past){
    //重新开始按钮
    connect(ui->restartBtn,&QPushButton::clicked,this,[=](){
        MainWindow *newWindow = new MainWindow(nullptr,past->numx,past->numy,past->mine_num,past->mine_per,past->sms);
        newWindow->setGeometry(past->geometry());
        this->close();    past->close();
        newWindow->show();
    });

    //改变难度按钮
    connect(ui->optionBtn,&QPushButton::clicked,this,[=](){
        SelectWidget * choosescene= new SelectWidget(nullptr,past->numx,past->numy,past->mine_num,past->mine_per,past);
        choosescene->setGeometry(past->geometry());
        this->close();
        past->hide();
        choosescene->show();
    });

    //再来一局的按钮
    connect(ui->newBtn,&QPushButton::clicked,this,[=](){
        //qDebug()<<past->numx;
        MainWindow *newWindow = new MainWindow(nullptr,past->numx,past->numy,past->mine_num,past->mine_per);
        newWindow->setGeometry(past->geometry());
        this->close();    past->close();
        newWindow->show();
    });

    //取消按钮
    connect(ui->CancelBtn,&QPushButton::clicked,this,[=](){
        this->close();
    });
}


void Restart::paintEvent(QPaintEvent *){
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/img/resources/back.png");
    painter.drawPixmap(0,0,this->width(),this->height(),pix);

}




Restart::~Restart()
{
    delete ui;
}


























