#include "widget.h"
#include "ui_widget.h"
#include "lyrics.h"
#include "aristpicture.h"
#include "downloadfile.h"

#include <QPropertyAnimation>
#include <QSettings>
#include <QDir>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    layout()->setSizeConstraint(QLayout::SetFixedSize);
    layout()->setAlignment( Qt::AlignHCenter);
    m_cookJar = new QNetworkCookieJar;
    m_userLonIndlg = new LoginDlg(0, m_cookJar);
    m_channelWidget = new ChannelsWidget(0, m_cookJar);
    m_pPlayer = new QMediaPlayer(this);
    m_iCurrentSongIndex = 0;
    ui->volSlider->hide();
    ui->channelwidget->hide();



    connect(m_userLonIndlg, SIGNAL(loginSuccess(QString)), ui->aristPicWidget, SLOT(userLoninSlot(QString)));
    connect(this, SIGNAL(channelInfoSignal(QString)), ui->aristPicWidget, SLOT(channelChangedSlot(QString)));
    connect(ui->channelwidget, SIGNAL(channelChanged(CHANNEL_INFO)), this, SLOT(channelChangedSlot(CHANNEL_INFO)));
    connect(m_pPlayer, &QMediaPlayer::mediaStatusChanged, this, &Widget::mediaStatusChanged);
    connect(m_pPlayer, &QMediaPlayer::positionChanged, this, &Widget::positionChanged);
    connect(m_pPlayer, &QMediaPlayer::durationChanged, this, &Widget::durationChanged);
    connect(m_pPlayer, SIGNAL(positionChanged(qint64)), ui->lycWidget, SLOT(setTickLrc(qint64)));
    connect(ui->volSlider,  &QSlider::valueChanged, this, &Widget::volChannged);
    connect(ui->progressSlider, &QSlider::sliderMoved, this, &Widget::setPosition);
    connect(ui->aristPicWidget, SIGNAL(pauseSignal(bool)), this, SLOT(pauseSlot(bool)));
    connect(m_pPlayer, SIGNAL(durationChanged(qint64)), ui->aristPicWidget, SLOT(setDefultPic(qint64)));
    connect(m_pPlayer, SIGNAL(positionChanged(qint64)), ui->aristPicWidget, SLOT(positionChanged(qint64)));

    ui->volSlider->setRange(0, 100);
    ui->volSlider->setValue(60);


    //歌曲频道信息
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "QBaiduFm", "QBaiduFm");
    settings.beginGroup("User");
    int iChannel = settings.value("ChannelIndex").toInt();
    int iCheck = settings.value("RememberMe").toInt();
    settings.endGroup();

    //登录
    if(iCheck)
    {
        m_userLonIndlg->logIn();
    }
    //获取频道列表
    ui->channelwidget->channelSelected(iChannel);
}

void Widget::pauseSlot(bool pause)
{
    if(pause)
        m_pPlayer->pause();
    else
        m_pPlayer->play();
}

void Widget::volChannged(qint64 position)
{
    m_pPlayer->setVolume(position);
}
void Widget::mediaStatusChanged(QMediaPlayer::MediaStatus status)
{
    if(status == QMediaPlayer::EndOfMedia /*|| status == QMediaPlayer::NoMedia*/)
    {
        //qDebug() << "播放完成";
        nextSong();
    }
}

void Widget::durationChanged(qint64 duration)
{
    ui->progressSlider->setRange(0, duration);
}

void Widget::setPosition(qint64 position)
{
    m_pPlayer->setPosition(position);
}

void Widget::positionChanged(qint64 position)
{
    ui->progressSlider->setValue(position);
}


void Widget::channelChangedSlot(CHANNEL_INFO channel)
{
    m_songIdList.clear();
    m_iCurrentSongIndex = 0;


    m_currentChannelId.channelId = channel.channelId;
    m_currentChannelId.channelName = channel.channelName;

    //初始化某个频道列表下的所有歌曲
    initSongIdList();
}

void Widget::initSongIdList()
{
    //第二步,获取某个频道下的歌曲ID列表
    GetSongPlayListId getPlayListId(this, m_cookJar);
    m_songIdList = getPlayListId.getSongIdList(m_currentChannelId.channelId);

    qDebug() << "频道:" << m_currentChannelId.channelName << m_currentChannelId.channelId<< "共有歌曲" << m_songIdList.size();


    QString info = m_currentChannelId.channelName + " 共有歌曲: " + QString::number(m_songIdList.size());
    emit channelInfoSignal(info);
    play(0);
}

Widget::~Widget()
{
    delete m_userLonIndlg;
    delete ui;
}

void Widget::on_settingtoolButton_clicked()
{
    m_userLonIndlg->showDlg();
}

void Widget::closeEvent(QCloseEvent *)
{
    m_userLonIndlg->close();
}

void Widget::on_voltoolButton_clicked()
{
    if(ui->volSlider->isHidden())
    {
        ui->volSlider->show();
    }
    else
        ui->volSlider->hide();
}

void Widget::on_prevtoolButton_clicked()
{
    prevSong();
}

void Widget::on_nexttoolButton_clicked()
{
    nextSong();
}

void Widget::on_showChanneltoolButton_clicked()
{
    if(ui->channelwidget->isHidden())
    {
        ui->channelwidget->show();

//        QPropertyAnimation *anim = new QPropertyAnimation(ui->channelwidget, "geometry");
//        anim->setDuration(1500);

//        anim->setEasingCurve(QEasingCurve::OutCubic);
//        ui->channelwidget->setVisible(true);
//        connect(anim, &QPropertyAnimation::finished, [this] () {
//            ui->channelwidget->setVisible(false);
//        });
//        anim->start(QPropertyAnimation::DeleteWhenStopped);

    }
    else
        ui->channelwidget->hide();
}


void Widget::prevSong()
{
    qDebug() << "上一曲";

    if(m_iCurrentSongIndex < 0 || m_iCurrentSongIndex >= m_songIdList.size() - 1)
    {
        initSongIdList();
        return;
    }

    m_iCurrentSongIndex--;

    play(m_iCurrentSongIndex);
}


void Widget::nextSong()
{
    //qDebug() << "下一首" << m_iCurrentSongIndex << m_songIdList.size();
    if(m_iCurrentSongIndex < 0 || m_iCurrentSongIndex >= m_songIdList.size() - 1)
    {
        initSongIdList();
        return;
    }

    m_iCurrentSongIndex++;

    play(m_iCurrentSongIndex);
}

void Widget::play(int index)
{
    if(index > m_songIdList.size())
        return;

    GetSongRealLink getSongLink;
    m_currentSongInfo = getSongLink.getSongRealLinkById(m_songIdList.at(index));

    m_pPlayer->setMedia(QUrl(m_currentSongInfo.songRealLink));
    m_pPlayer->play();


    //获取歌词
    GetLrc getLrc;
    QString lrc = getLrc.getLrc(m_currentSongInfo.lrcLink);
    ui->lycWidget->currentSongChannged(m_currentSongInfo.songName, m_currentSongInfo.artistName, m_currentSongInfo.albumName);
    ui->lycWidget->setLrc(lrc);

    //显示歌手图片
    GetAristPic getPic;
    QPixmap picMap = getPic.getAristPic(m_currentSongInfo.songPicRadio);
    ui->aristPicWidget->setAristPic(picMap);
}


//下载歌曲
void Widget::on_downloadtoolButton_clicked()
{
    QString fileName;
    DownLoadFile downMp3;

    QDir dir;
    if(!dir.exists("./download"))
    {
        dir.mkdir("./download");
    }
    fileName = "./download/" + m_currentSongInfo.songName + ".mp3";

    qDebug() << fileName;

    downMp3.getMp3File(fileName, m_currentSongInfo.songRealLink);

    qDebug() << "下载完成";
}
