#ifndef MYSCROLL_H
#define MYSCROLL_H

#include <QWidget>
#include <QScrollArea>
#include "playarea.h"

class PlayArea;

class myScroll : public QScrollArea
{
    Q_OBJECT

private:
    PlayArea *p=nullptr;

public:
    explicit myScroll(QWidget *parent = nullptr);
    void setPlayArea(PlayArea*);
    void wheelEvent(QWheelEvent *e);
signals:

};

#endif // MYSCROLL_H
