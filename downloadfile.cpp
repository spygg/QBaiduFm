#include "downloadfile.h"

DownLoadFile::DownLoadFile(QObject *parent) : QObject(parent)
{

}

void DownLoadFile::getMp3File(QString fileName, QString url)
{
    QNetworkRequest networkRequest;
    QNetworkAccessManager networkManager;

    networkRequest.setUrl(QUrl(url));
    qDebug() << url;

    QNetworkReply *reply = networkManager.get(networkRequest);

    QEventLoop loop;
    connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();
    reply->deleteLater();

    QFile file;
    file.setFileName(fileName);
    bool bOk = file.open(QIODevice::WriteOnly);
    if(bOk)
    {
        file.write(reply->readAll());
    }
}

