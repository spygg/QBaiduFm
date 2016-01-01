#ifndef CHANNELSWIDGET_H
#define CHANNELSWIDGET_H

#include <QFrame>
#include <QButtonGroup>

#include "getbaiduchannel.h"

namespace Ui {
class ChannelsWidget;
}

class ChannelsWidget : public QFrame
{
    Q_OBJECT

public:
    explicit ChannelsWidget(QWidget *parent = 0, QNetworkCookieJar *cookJar = NULL);
    ~ChannelsWidget();

    QList<CHANNEL_INFO> getChannelList();
private:
    GetBaiduChannel *m_getChannelList;
    Ui::ChannelsWidget *ui;
    void leaveEvent(QEvent *event);

    QGridLayout *m_gridlayOut;
    QButtonGroup m_btnGroup;

    QList <CHANNEL_INFO> m_channelList;
    int m_iLastChannelIndex;

public slots:
    void channelSelected(int iChannel);

signals:
    void channelChanged(CHANNEL_INFO Channel);
};

#endif // CHANNELSWIDGET_H
