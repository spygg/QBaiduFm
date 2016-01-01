#ifndef VOLPANNEL_H
#define VOLPANNEL_H
#include <QSlider>

class volPannel : public QSlider
{
    Q_OBJECT
public:
    volPannel(QWidget *parent = 0);

private:

    void leaveEvent(QEvent *);
};

#endif // VOLPANNEL_H
