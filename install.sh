#!/bin/sh

SHARE=/usr/share
PIXMAP=/usr/share/pixmaps
APP=/usr/share/applications
BIN=/usr/bin

if [ "$1" = "-u" ]; then
    rm $BIN/QBaiduFm
    rm $PIXMAP/baiduFm.png
    rm $APP/QBaiduFm.desktop
    rm -r $SHARE/QBaiduFm
else
    mkdir -p $SHARE/QBaiduFm
    cp QBaiduFm $SHARE/QBaiduFm
    #mkdir -p $SHARE/QBaiduFm/lang/
    #cp -r lang/*.qm $SHARE/QBaiduFm/lang/
    rm -f $BIN/QBaiduFm
    ln -s $SHARE/QBaiduFm/QBaiduFm $BIN/QBaiduFm
    cp QBaiduFm.desktop $APP
    cp ./img/baiduFm.png $PIXMAP
fi
