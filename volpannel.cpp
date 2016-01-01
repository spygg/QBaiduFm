#include "volpannel.h"
#include <QPropertyAnimation>

volPannel::volPannel(QWidget *parent):QSlider(parent)
{

}


void volPannel::leaveEvent(QEvent *)
{
    setVisible(false);
}
