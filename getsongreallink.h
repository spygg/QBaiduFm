#ifndef GETSONGREALLINK_H
#define GETSONGREALLINK_H

#include <QObject>
#include <QString>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QEventLoop>
#include <QNetworkCookieJar>

typedef struct _SONG_INFO
{
    QString songRealLink;
    QString songName;
    QString artistName;
    QString songPicSmall;
    QString songPicRadio;
    QString time;
    QString albumName;
    QString lrcLink;
    QString size;
}SONG_INFO;

class GetSongRealLink : public QObject
{
    Q_OBJECT
public:
    explicit GetSongRealLink(QObject *parent = 0, QNetworkCookieJar *cookie = NULL);

signals:

public slots:

    SONG_INFO getSongRealLinkById(QString songId);

private:
    QNetworkCookieJar *m_cookJar;
};

#endif // GETSONGREALLINK_H
