#ifndef MULTICONTROL_H
#define MULTICONTROL_H

#include <QWidget>
#include <QPushButton>
#include <QFont>

namespace Ui {
class MultiControl;
}

class MultiControl : public QWidget
{
    Q_OBJECT

public:
    explicit MultiControl(QWidget *parent = nullptr);
    ~MultiControl();

    void setdouble(bool isdouble=0);
    //int部分
    void  setlimit(int l,int r);
    void  setVal(int val);
    int   getVal() const;
    void  BtnConnection();
    void  LineConnection();

    //double部分
    void  setlimit(double l,double r,bool isdouble);
    void  setVal(double val,bool is_double);
    double getVal(bool) const;
    void  BtnConnection(bool isdouble);
    void  LineConnection(bool isdouble);
//    void  mousePressEvent(QMouseEvent *e);

private:
    Ui::MultiControl *ui;
    bool isa,ism;
    bool isdouble;
    double L,R;

signals:
    void  valuechanged();
    void  check(int,int);
    void  check(double,double,bool);
    void  setdefaultInt();
    void  setdefaultDouble();

public slots:
    void assign(int lowerlimit,int upperlimit);
    void assign(double lowerlimit, double upperlimit,bool isdouble);
};

#endif // MULTICONTROL_H




















