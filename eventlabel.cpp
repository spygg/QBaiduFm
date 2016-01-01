#include "eventlabel.h"
#include <QDebug>
#include <QPainter>

EventLabel::EventLabel(QWidget *parent):QLabel(parent)
{

}

EventLabel::~EventLabel()
{

}

void EventLabel::enterEvent(QEvent *)
{
    emit enterSignal();
}

void EventLabel::mousePressEvent(QMouseEvent * ev)
{
    emit clickSignal();
}


void EventLabel::leaveEvent(QEvent *)
{
    emit leaveSignal();
}
