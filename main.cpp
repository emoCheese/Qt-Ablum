#include "mainwindow.h"

#include <QApplication>
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QFile qss(":/res/style/style.qss");
    if(qss.open(QFile::ReadOnly))
    {
        qDebug("open qss success");
        QString style = QLatin1String(qss.readAll());
        a.setStyleSheet(style);
    }
    else
    {
        qDebug("Open qss failed");
        return 0;
    }
    MainWindow w;
    w.setWindowTitle("Album");
    w.show();
    return a.exec();
}
