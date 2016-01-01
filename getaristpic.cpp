#include "getaristpic.h"

GetAristPic::GetAristPic(QObject *parent) : QObject(parent)
{

}

QPixmap GetAristPic::getAristPic(QString url)
{
    QNetworkRequest networkRequest;
    QNetworkAccessManager networkManager;

    networkRequest.setUrl(QUrl(url));

    QNetworkReply *reply = networkManager.get(networkRequest);

    QEventLoop loop;
    connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();
    reply->deleteLater();


    QPixmap pic;
    pic.loadFromData(reply->readAll());

    return pic;
}

