#ifndef SELECTWIDGET_H
#define SELECTWIDGET_H

#include <QMainWindow>
#include <QEvent>
#include <QMouseEvent>
#include "mainwindow.h"
#include <QPainter>

class MainWindow;

namespace Ui {
class SelectWidget;
}

class SelectWidget : public QMainWindow
{
    Q_OBJECT

public:
    explicit SelectWidget(QWidget *parent = nullptr,int nx=0,int ny=0,int mn=0,double mp=0,MainWindow *past=nullptr);
    ~SelectWidget();
    void buildconnect();
    void paintEvent(QPaintEvent *e);

private:
    Ui::SelectWidget *ui;
    int scalx,scaly,mine_num ;
    double mine_per;

};

#endif // SELECTWIDGET_H
