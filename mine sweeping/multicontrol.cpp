#include "multicontrol.h"
#include "ui_multicontrol.h"
#include <QMessageBox>
#include <QTimer>
#include <QDebug>

MultiControl::MultiControl(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MultiControl)
{
    ui->setupUi(this);
    L=4,R=99;
    isa=1,ism=1;                 //按钮信号和槽建立
    //长按功能的实现
    ui->pbtn->setAutoRepeat(true);
    ui->mbtn->setAutoRepeat(true);
    ui->pbtn->setAutoRepeatDelay(700);
    ui->mbtn->setAutoRepeatDelay(700);
    ui->pbtn->setAutoRepeatInterval(30);
    ui->mbtn->setAutoRepeatInterval(30);
    //BtnConnection();
    //LineConnection();
    connect(this,SIGNAL(check(int,int)),this,SLOT(assign(int,int)));
    connect(this,SIGNAL(check(double,double,bool)),this,SLOT(assign(double,double,bool)));
}

MultiControl::~MultiControl()
{
    delete ui;
}

void MultiControl::setdouble(bool i){
    isdouble=i;
}
//int 的重载
void MultiControl::setlimit(int l, int r){
    L=l,R=r;
}

void MultiControl::setVal(int val){
    ui->lineEdit->setText(QString::number(val));
}

int MultiControl::getVal() const{
    return ui->lineEdit->text().toInt();
}

void MultiControl::BtnConnection(){
    connect(ui->pbtn,&QPushButton::pressed,ui->lineEdit,[=](){
        //qDebug()<<R;
        int tmp=ui->lineEdit->text().toInt();
        if(tmp+1<=R)
            this->setVal(++tmp);
        else
            this->setVal(R);
        emit valuechanged();
    });
    connect(ui->mbtn,&QPushButton::pressed,ui->lineEdit,[=](){
        int tmp=ui->lineEdit->text().toInt();
        if(tmp-1>=L)
            this->setVal(--tmp);
        else
            this->setVal(L);
        emit valuechanged();
    });
}

void MultiControl::LineConnection(){
    connect(ui->lineEdit,&QLineEdit::editingFinished,this,[=](){
        int tmp=ui->lineEdit->text().toInt();
        bool ok;
        if(!ui->lineEdit->text().toInt(&ok,10) && ui->lineEdit->text().isEmpty())
            setVal(L);
        else
            emit check(L,R);
        emit valuechanged();
    });
}

void MultiControl::assign(int lowerlimit, int upperlimit){
    int tmp=ui->lineEdit->text().toInt();
    if(tmp<=lowerlimit )
        setVal(lowerlimit);
    if(tmp>=upperlimit)
        setVal(upperlimit);
}


//double的重载
void MultiControl::setlimit(double l, double r,bool isdouble){
    L=l,R=r;
}

void MultiControl::setVal(double val,bool is_double){
    QString str = QString::number(val, 'f', 3);
    ui->lineEdit->setText(str);
}

double MultiControl::getVal(bool isdouble) const{
    return ui->lineEdit->text().toDouble();
}

void MultiControl::BtnConnection(bool isdouble){
    connect(ui->pbtn,&QPushButton::pressed,ui->lineEdit,[=](){
        double tmp=ui->lineEdit->text().toDouble();
        if(tmp+1<=R)
            this->setVal(++tmp,1);
        else
            this->setVal(R,1);
        emit valuechanged();
    });
    connect(ui->mbtn,&QPushButton::pressed,ui->lineEdit,[=](){
        double tmp=ui->lineEdit->text().toDouble();
        if(tmp-1>=L)
            this->setVal(--tmp,1);
        else
            this->setVal(L,1);
        emit valuechanged();
    });
}

void MultiControl::LineConnection(bool isdouble){
    connect(ui->lineEdit,&QLineEdit::editingFinished,this,[=](){
        double tmp=ui->lineEdit->text().toDouble();
        bool ok;
        if(!ui->lineEdit->text().toDouble(&ok) && ui->lineEdit->text().isEmpty())
           setVal(L,1);
        else
            emit check(L,R,1);
        emit valuechanged();
    });
}

void MultiControl::assign(double lowerlimit, double upperlimit,bool isdouble){
    double tmp=ui->lineEdit->text().toDouble();
    setVal(tmp,1);
    if(tmp<=lowerlimit )
        setVal(lowerlimit,1);
    if(tmp>=upperlimit)
        setVal(upperlimit,1);
}





