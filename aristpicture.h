#ifndef ARISTPICTURE_H
#define ARISTPICTURE_H

#include <QFrame>
#include <QTimer>
#include <QImage>

#include "channelswidget.h"

namespace Ui {
class AristPicture;
}

enum PLAYER_STATE
{
    STATE_STOP,
    STATE_PAUSE,
    STATE_PALYING
};

class AristPicture : public QFrame
{
    Q_OBJECT

public:
    explicit AristPicture(QWidget *parent = 0);
    ~AristPicture();

private:
    Ui::AristPicture *ui;
    QPixmap m_mixedPic;
    QPixmap m_mixedCurrentPic;
    QTimer *m_timer;

private:
    ushort m_iCurrentRotateAngle;
    int m_iCurrentState;
    qint64 m_totalTime;
    QString m_channelInfo;
    QString m_userName;

public:
    void setAristPic(QPixmap pixmap);


public slots:
    void setDefultPic(qint64 totalTime);
    void channelChangedSlot(QString channel);
    void userLoninSlot(QString userInfo);

    void scrollInfo();
private slots:
    void clickSlot();
    void leaveSlot();
    void enterSlot();
    void positionChanged(qint64 position);
    void showwhirlPicSlot(qint64 position);
signals:
    void pauseSignal(bool pause);
    void showPicSignal(qint64 position);

};

#endif // ARISTPICTURE_H
