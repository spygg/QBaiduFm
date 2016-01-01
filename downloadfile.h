#ifndef DOWNLOADFILE_H
#define DOWNLOADFILE_H

#include <QObject>
#include <QWidget>
#include <QString>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QEventLoop>
#include <QFile>

class DownLoadFile: public QObject
{
    Q_OBJECT
public:
    explicit DownLoadFile(QObject *parent = 0);

public:
    void getMp3File(QString fileName, QString url);
};

#endif // DOWNLOADFILE_H
