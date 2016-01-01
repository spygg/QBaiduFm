#ifndef LYRICS_H
#define LYRICS_H

#include <QFrame>
#include <QTextStream>
#include <QTextCodec>
#include <QTime>

namespace Ui {
class Lyrics;
}

struct QLyric
{
    qint64 time;
    QString lyric;
};

class Lyrics : public QFrame
{
    Q_OBJECT

public:
    explicit Lyrics(QWidget *parent = 0);
    ~Lyrics();

private:
    Ui::Lyrics *ui;
    QList<QLyric> parse(QTextStream &stream);
    QList<QLyric> m_lyric;

    QString m_songName;
    QString m_aristName;
    QString m_ablumName;


    int m_iCurrentIndex;
public:
    void setLrc(QString lrc);

    void currentSongChannged(QString songName, QString aristName, QString ablumName);
public slots:
    void setTickLrc(qint64 position);
};

#endif // LYRICS_H
