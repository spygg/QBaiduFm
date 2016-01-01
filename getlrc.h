#ifndef GETLRC_H
#define GETLRC_H

#include <QObject>
#include <QString>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QEventLoop>
#include <QPixmap>

class GetLrc : public QObject
{
    Q_OBJECT
public:
    explicit GetLrc(QObject *parent = 0);

public:
    QString getLrc(QString url);
};

#endif // GETARISTPIC_H
