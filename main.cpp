#include "widget.h"
#include <QApplication>
#include <QDesktopWidget>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;

    QDesktopWidget *pDesk = QApplication::desktop();
    w.move((pDesk->width() - w.width()) / 2, (pDesk->height() - w.height()) / 8);

    w.setWindowIcon(QIcon(":/img/baiduFm.png"));
    w.show();

    return a.exec();
}
