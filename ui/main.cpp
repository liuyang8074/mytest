#include "widget.h"
#include <QApplication>
#include <Windows.h>
#include <QDebug>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    SYSTEM_INFO info;
//    GetSystemInfo(&info);
//    qDebug()<<"numbers of cpu"<<info.dwNumberOfProcessors;
//    MEMORYSTATUSEX statusex;
//    statusex.dwLength = sizeof (statusex);
//    GlobalMemoryStatusEx(&statusex);
//    qDebug()<<"物理内存使用率："<<statusex.dwMemoryLoad;
    Widget w;
    w.show();

    return a.exec();
}
