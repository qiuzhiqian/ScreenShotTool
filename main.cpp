#include "screenshottool.h"
#include <QApplication>

/*
 * Author:qiuzhiqian
 * Email:xia_mengliang@163.com
 * Github:https://github.com/qiuzhiqian
 * Date:2017.07.23
 **/

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ScreenShotTool w;
    w.show();

    return a.exec();
}
