#ifndef RESTART_H
#define RESTART_H

#include <QWidget>
#include <QFont>
#include <QSet>
#include <playarea.h>
#include "mainwindow.h"
#include <selectwidget.h>
#include <QtDebug>
#include <QMainWindow>
#include <QLabel>
#include <QPainter>

class MainWindow;

namespace Ui {
class Restart;
}

class Restart : public QWidget
{
    Q_OBJECT

public:
    explicit Restart(QWidget *parent ,MainWindow *,bool iswisn=0);
    void BuildBtnConnection(MainWindow *);
    void paintEvent(QPaintEvent *event);
    ~Restart();

private:
    Ui::Restart *ui;
};

#endif // RESTART_H
