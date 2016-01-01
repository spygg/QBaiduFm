#ifndef GETBAIDUCHANNEL_H
#define GETBAIDUCHANNEL_H

#include <QGridLayout>
#include <QFrame>
#include <QPushButton>
#include <QString>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QEventLoop>
#include <QNetworkCookieJar>

typedef struct _CHANNEL_INFO
{
    QString channelId;
    QString channelName;
}CHANNEL_INFO;

class GetBaiduChannel : public QFrame
{
    Q_OBJECT

public:
    explicit GetBaiduChannel(QFrame *parent = 0, QNetworkCookieJar *cookie = NULL);

signals:

public slots:

public:
    QList<CHANNEL_INFO> getMusicChannel();

private:
    QNetworkCookieJar *m_cookJar;
};

#endif // GETBAIDUCHANNEL_H
