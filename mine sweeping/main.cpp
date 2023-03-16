#include "mainwindow.h"

#include <QApplication>
#include <playarea.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow *w=new MainWindow(nullptr);
    w->show();
    //PlayArea *p=new PlayArea(nullptr);
    //p->show();
    return a.exec();
}
