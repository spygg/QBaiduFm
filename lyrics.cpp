#include "lyrics.h"
#include "ui_lyrics.h"

#include <QRegExp>
#include <QDebug>
#include <algorithm>

Lyrics::Lyrics(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::Lyrics)
{
    ui->setupUi(this);
    setFrameStyle(QFrame::NoFrame);
    ui->label->setWordWrap(true);
    ui->label->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    m_iCurrentIndex = 0;
}

Lyrics::~Lyrics()
{
    delete ui;
}

void Lyrics::currentSongChannged(QString songName, QString aristName, QString ablumName)
{
    m_songName = songName;
    m_aristName = aristName;
    m_ablumName = ablumName;

    QString lrc;
    lrc.clear();
    lrc += QString("<p align='center'><span style=' font-style:italic;'>");
    lrc += m_aristName + "--" + m_songName;
    lrc += QString("</span></p>");
    ui->label->setText(lrc);
}

void Lyrics::setLrc(QString lrc)
{
    QTextStream stream(&lrc);
    QTextCodec *codec = QTextCodec::codecForName("utf-8");
    stream.setCodec(codec);

    m_iCurrentIndex = 0;
    m_lyric.clear();

    m_lyric = parse(stream);
}

QList<QLyric> Lyrics::parse(QTextStream &stream)
{
    QRegExp timeExp;
    timeExp.setPatternSyntax(QRegExp::RegExp);
    timeExp.setCaseSensitivity(Qt::CaseSensitive);
    timeExp.setPattern("\\[([0-9]{2}):([0-9]{2})\\.([0-9]{2})\\]");

    QList<QLyric> result;
    while (!stream.atEnd()) {
        QString line = stream.readLine();
        int ret = timeExp.indexIn(line);
        QList<QTime> ticks;
        int lastindex = 0;
        while (ret >= 0) {
            QStringList tstr = timeExp.capturedTexts();
            QTime time(0, tstr[1].toInt(), tstr[2].toInt(), tstr[3].toInt());
            ticks.append(time);
            lastindex = ret + timeExp.matchedLength();
            ret = timeExp.indexIn(line, lastindex);
        }
        QString lyricstr = line.right(line.size() - lastindex);
        for (const QTime& t : ticks) {
            QLyric lyric;
            lyric.time = t.minute() * 60 * 1000 + t.second() * 1000 + t.msec();

            lyric.lyric = lyricstr;

//            qDebug() << lyricstr;
            result.append(lyric);
        }
    }
    std::sort(result.begin(), result.end(), [] (const QLyric& a, const QLyric& b) -> bool {
        return a.time < b.time;
    });

    return result;
}


void Lyrics::setTickLrc(qint64 position)
{
    if(m_lyric.size() <= 0 || m_iCurrentIndex >= m_lyric.size())
        return;


    for(int i = 0; i < m_lyric.size(); i++)
    {

        if(position > m_lyric.at(i).time)
        {
            m_iCurrentIndex = i;
        }
    }


    if(position > m_lyric.at(m_iCurrentIndex).time)
    {
        QString lrc;
        lrc.clear();
        lrc += QString("<p align='center'><span style=' font-style:italic;'>");
        lrc += m_aristName + " - " + m_songName;
        lrc += QString("</span></p>");

        for(int j = m_iCurrentIndex - 1; j < m_lyric.size(); j++)
        {
            if(j < 0)
                continue;

            if(m_iCurrentIndex == j)
            {
                lrc += QString("<p align='center'><span style=' font-weight:600;'>");
                lrc += m_lyric.at(m_iCurrentIndex).lyric;
                lrc += QString("</span></p>");
            }
            else
            {
                lrc += "<p align='center'>";
                lrc += m_lyric.at(j).lyric;
                lrc += "</p>";
            }


        }
        ui->label->setText(lrc);
        m_iCurrentIndex++;
    }
}
