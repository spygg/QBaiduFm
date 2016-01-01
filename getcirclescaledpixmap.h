#ifndef GETCIRCLESCALEDPIXMAP_H
#define GETCIRCLESCALEDPIXMAP_H

#include <QObject>
#include <QPixmap>
#include <QRegion>
#include <QPainter>


class GetCircleScaledPixmap : public QObject
{
    Q_OBJECT
public:
    explicit GetCircleScaledPixmap(QObject *parent = 0);

signals:

public slots:

public:

    QPixmap getCirclePictureAtCenter(QPixmap srcPic, QPixmap backPic/*, int width, int height*/);
    QPixmap getCirclePictureAtCenter(QPixmap srcPic, QPixmap backPic, int width, int height);
};

#endif // GETCIRCLESCALEDPIXMAP_H
