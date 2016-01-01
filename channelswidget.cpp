#include "channelswidget.h"
#include "ui_channelswidget.h"
#include <QPropertyAnimation>
#include <QSettings>

ChannelsWidget::ChannelsWidget(QWidget *parent, QNetworkCookieJar *cookJar) :
    QFrame(parent),
    ui(new Ui::ChannelsWidget)
{
    ui->setupUi(this);
    setFrameStyle(QFrame::NoFrame);

    m_getChannelList = new GetBaiduChannel(0, cookJar);
    m_gridlayOut = new QGridLayout;

    m_channelList = getChannelList();

    for(int i = 0, k = 0; i < m_channelList.size() / 4; i++, k += 4)
    {
        for(int j = 0; j < 4; j++)
        {
            QPushButton *btn = new QPushButton(this);

            btn->setText(m_channelList.at(k + j).channelName);
            //qDebug() << m_channelList.at(k + j).channelName;
            m_gridlayOut->addWidget(btn, i, j);
            //qDebug() << k + j;
            m_btnGroup.addButton(btn, k + j);
        }
    }

    setLayout(m_gridlayOut);

    connect( &m_btnGroup, SIGNAL(buttonClicked (int)), this, SLOT(channelSelected(int)) );
    m_iLastChannelIndex = 0;
}

void ChannelsWidget::channelSelected(int iChannel)
{
    //先还原样式表
    m_btnGroup.button(m_iLastChannelIndex)->setStyleSheet("");

    m_btnGroup.button(iChannel)->setStyleSheet("QPushButton{ font-weight:600;color:#5500ff;}");
    m_iLastChannelIndex = iChannel;
    emit channelChanged(m_channelList.at(iChannel));

    //记录当前频道信息
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "QBaiduFm", "QBaiduFm");
    settings.beginGroup("User");
    settings.setValue("ChannelId", m_channelList.at(iChannel).channelId);
    settings.setValue("ChannelName", m_channelList.at(iChannel).channelName);
    settings.setValue("ChannelIndex", iChannel);
    settings.endGroup();
}


ChannelsWidget::~ChannelsWidget()
{
    delete ui;
}

void ChannelsWidget::leaveEvent(QEvent *event)
{
    hide();
}

QList<CHANNEL_INFO> ChannelsWidget::getChannelList()
{
    return m_getChannelList->getMusicChannel();
}
