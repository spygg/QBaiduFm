#include "getbaiduchannel.h"

const QString channelUrl = "http://fm.baidu.com/dev/api/?tn=channellist";

GetBaiduChannel::GetBaiduChannel(QFrame *parent, QNetworkCookieJar *cookie) : QFrame(parent)
{
    m_cookJar = cookie;
}

QList<CHANNEL_INFO> GetBaiduChannel::getMusicChannel()
{
    QList<CHANNEL_INFO> channelInfoList;

    QNetworkRequest networkRequest;
    QNetworkAccessManager *networkManager = new QNetworkAccessManager;

    networkRequest.setUrl(QUrl(channelUrl));

    networkManager->setCookieJar(m_cookJar);
    QNetworkReply *reply = networkManager->get(networkRequest);

    QEventLoop loop;
    connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();
    reply->deleteLater();

    QByteArray buf = reply->readAll();

    //qDebug() << buf;

    QJsonParseError jsonError;
    QJsonDocument jsonRes = QJsonDocument::fromJson(buf, &jsonError);;

    if(jsonError.error == QJsonParseError::NoError)
    {
        if(jsonRes.isObject())
        {
            QVariantMap result = jsonRes.toVariant().toMap();

            foreach(QVariant channel, result["channel_list"].toList())
            {
                QVariantMap channelMap = channel.toMap();


                CHANNEL_INFO channelInfo;
                channelInfo.channelId = channelMap["channel_id"].toString();
                channelInfo.channelName = channelMap["channel_name"].toString();

                channelInfoList.append(channelInfo);

            }
        }
    }

    return channelInfoList;
}
