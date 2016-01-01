#include "getlrc.h"

GetLrc::GetLrc(QObject *parent) : QObject(parent)
{

}

QString GetLrc::getLrc(QString url)
{
    QNetworkRequest networkRequest;
    QNetworkAccessManager networkManager;

    url = "http://musicdata.baidu.com" + url;
    networkRequest.setUrl(QUrl(url));

    QNetworkReply *reply = networkManager.get(networkRequest);

    QEventLoop loop;
    connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();
    reply->deleteLater();

    QString lrc;
    lrc = reply->readAll();

    return lrc;
}

