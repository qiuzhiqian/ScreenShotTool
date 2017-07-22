#include "screenshottool.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ScreenShotTool w;
    w.show();

    return a.exec();
}
