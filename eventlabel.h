#ifndef EVENTLABEL_H
#define EVENTLABEL_H

#include <QObject>
#include <QLabel>
#include <QFrame>

class EventLabel : public QLabel
{
    Q_OBJECT

public:
    explicit EventLabel(QWidget *parent = 0);
    ~EventLabel();

private:
    void enterEvent(QEvent*);
    void leaveEvent(QEvent*);
    void mousePressEvent(QMouseEvent * ev);

signals:
    void enterSignal();
    void leaveSignal();
    void clickSignal();
};

#endif // EVENTLABEL_H
