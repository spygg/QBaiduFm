#ifndef GETARISTPIC_H
#define GETARISTPIC_H

#include <QObject>
#include <QString>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QEventLoop>
#include <QPixmap>


class GetAristPic : public QObject
{
    Q_OBJECT
public:
    explicit GetAristPic(QObject *parent = 0);

signals:

public slots:


public:
    QPixmap getAristPic(QString url);
};

#endif // GETARISTPIC_H
