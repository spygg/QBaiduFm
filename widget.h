#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QEventLoop>
#include <QNetworkCookieJar>
#include <QNetworkCookie>
#include <QMediaPlayer>

#include "logindlg.h"
#include "channelswidget.h"
#include "lyrics.h"
#include "volpannel.h"
#include "widget.h"
#include "aristpicture.h"
#include "getsongplaylistid.h"
#include "getsongreallink.h"
#include "getaristpic.h"
#include "getlrc.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

public slots:
    void on_settingtoolButton_clicked();

    void on_voltoolButton_clicked();

    void on_showChanneltoolButton_clicked();

    void channelChangedSlot(CHANNEL_INFO channel);
    void on_nexttoolButton_clicked();

    void on_prevtoolButton_clicked();

    void pauseSlot(bool pause);
    void on_downloadtoolButton_clicked();

private:
    Ui::Widget *ui;
    void closeEvent(QCloseEvent *event);


private:
    QMediaPlayer *m_pPlayer;
    LoginDlg *m_userLonIndlg;
    CHANNEL_INFO m_currentChannelId;
    ChannelsWidget *m_channelWidget;
    QList <QString> m_songIdList;
    int m_iCurrentSongIndex;
    SONG_INFO m_currentSongInfo;

    QNetworkCookieJar *m_cookJar;
    void play(int index);
    void nextSong();
    void prevSong();
    void initSongIdList();
    void mediaStatusChanged(QMediaPlayer::MediaStatus status);
    void positionChanged(qint64 position);
    void durationChanged(qint64 duration);
    void setPosition(qint64 position);
    void volChannged(qint64 position);

signals:
    void channelInfoSignal(QString channel);
private slots:
    void errorOccur(QMediaPlayer::Error);
};

#endif // WIDGET_H


