#include "aristpicture.h"
#include "ui_aristpicture.h"
#include "getcirclescaledpixmap.h"
#include <QDebug>

AristPicture::AristPicture(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::AristPicture)
{
    ui->setupUi(this);
    setFrameStyle(QFrame::NoFrame);


    m_timer = new QTimer(this);
    m_iCurrentRotateAngle = 0;
    m_mixedPic.load(":/img/cd.png");

    connect(ui->label, SIGNAL(enterSignal()), this, SLOT(enterSlot()));
    connect(ui->label, SIGNAL(leaveSignal()), this, SLOT(leaveSlot()));
    connect(ui->label, SIGNAL(clickSignal()), this, SLOT(clickSlot()));
    connect(this, SIGNAL(showPicSignal(qint64)), this, SLOT(showwhirlPicSlot(qint64)) );
    connect(m_timer, SIGNAL(timeout()), this,  SLOT(scrollInfo()));

    m_timer->setInterval(1000);
    m_timer->start();

    m_iCurrentState = STATE_STOP;
    m_userName = "未知用户";
    m_channelInfo = "空";
}

AristPicture::~AristPicture()
{
    delete ui;
}

void AristPicture::scrollInfo()
{
    static int nPos = 0;
    static int iturn = 0;

    QString scrollStr;

    if(iturn)
    {
        scrollStr += QString("当前登录:");
        scrollStr += m_userName;
    }
    else
    {
        scrollStr += "频道: ";
        scrollStr += m_channelInfo;
    }

    if (nPos > scrollStr.length())
    {
        nPos = 0;

        iturn = ~iturn;
    }


    QString info = QString("<p><span style=' font-weight:600; color:#aaaa00;'>");
    info += scrollStr.mid(nPos);
    info += QString("</span></p>");

    ui->label_info->setText(info);
    nPos++;
}

void AristPicture::channelChangedSlot(QString channel)
{
    m_channelInfo = channel;
}

void AristPicture::userLoninSlot(QString userInfo)
{
    qDebug() << "用户" << userInfo;
    m_userName = userInfo;
}

void AristPicture::setDefultPic(qint64 totalTime)
{
    m_iCurrentRotateAngle = 0;
    m_totalTime = totalTime;
    m_mixedPic.load(":/img/cd.png");
    ui->label->setPixmap(m_mixedPic);
}

void AristPicture::enterSlot()
{
    disconnect(this, SIGNAL(showPicSignal(qint64)), this, SLOT(showwhirlPicSlot(qint64)) );

    if(m_iCurrentState == STATE_PALYING)
    {
        QPixmap pixmap;
        pixmap.load(":/img/pause.png");

        QPixmap pixMix;

        GetCircleScaledPixmap destPic;

        pixMix = destPic.getCirclePictureAtCenter(pixmap, m_mixedCurrentPic, 85, 85);//290, 290

        ui->label->setPixmap(pixMix);

    }

}

void AristPicture::clickSlot()
{
    if(m_iCurrentState == STATE_PALYING)
    {
        m_iCurrentState = STATE_PAUSE;
        emit pauseSignal(true);


        QPixmap pixmap;
        pixmap.load(":/img/play.png");

        QPixmap pixMix;

        GetCircleScaledPixmap destPic;

        pixMix = destPic.getCirclePictureAtCenter(pixmap, m_mixedCurrentPic, 85, 85);//290, 290

        ui->label->setPixmap(pixMix);


    }
    else if(m_iCurrentState == STATE_PAUSE)
    {
        m_iCurrentState = STATE_PALYING;
        emit pauseSignal(false);

        QPixmap pixmap;
        pixmap.load(":/img/pause.png");

        QPixmap pixMix;

        GetCircleScaledPixmap destPic;

        pixMix = destPic.getCirclePictureAtCenter(pixmap, m_mixedCurrentPic, 85, 85);//290, 290

        ui->label->setPixmap(pixMix);
    }
}

void AristPicture::leaveSlot()
{
    if(STATE_PALYING == m_iCurrentState)
    {
        connect(this, SIGNAL(showPicSignal(qint64)), this, SLOT(showwhirlPicSlot(qint64)) );
    }
}

void AristPicture::setAristPic(QPixmap pixmap)
{
    QPixmap picBackGournd(":/img/cd.png");
    GetCircleScaledPixmap destPic;

    m_mixedCurrentPic = m_mixedPic = destPic.getCirclePictureAtCenter(pixmap, picBackGournd);//290, 290

    if(STATE_PALYING != m_iCurrentState)
    {
        m_iCurrentState = STATE_PALYING;
    }
}


void AristPicture::positionChanged(qint64 position)
{
    emit showPicSignal(position);
}

void AristPicture::showwhirlPicSlot(qint64 position)
{
    m_iCurrentRotateAngle = position / (1.0 * m_totalTime) * 360;

    QTransform trs;
    trs.rotate(m_iCurrentRotateAngle);

    QPixmap pix;
    pix = m_mixedPic.transformed(trs)/*.scaled(QSize(240, 240), Qt::KeepAspectRatio, Qt::SmoothTransformation)*/;

    qreal fx = 0;
    qreal fy = 0;

    //qDebug() << pix.size().width() << m_mixedPic.size().width() << pix.size().height() << m_mixedPic.height();

    fx = (pix.size().width() - m_mixedPic.size().width())/2.0;
    fy = (pix.size().height() - m_mixedPic.height())/2.0;
    pix = pix.copy(fx, fy, m_mixedPic.height(), m_mixedPic.height());

    m_mixedCurrentPic = pix = pix.scaled(QSize(180, 180), Qt::KeepAspectRatio, Qt::SmoothTransformation);
//    qDebug() << pix.size().width() << m_mixedPic.size().width() << pix.size().height() << m_mixedPic.height();
    ui->label->setPixmap(pix);
}
