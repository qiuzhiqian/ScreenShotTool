#include "screenshottool.h"
#include "ui_screenshottool.h"

/*
 * 画图原理
 * 1\首先在全屏幕设置一个透明画布
 * 2\然后在透明画布上绘制矩形
 * 3\对画布上的矩形区域截图
 * 4\保存截图到文件夹或剪切板
 * 修正
 * 1\全屏截屏，同时生成一个暗色图片
 * 2\设置全屏画布，然后将暗色全屏图片显示到画布
 * 3\在暗色全屏图片上画矩形，矩形内显示亮色图片
 * 4\对矩形区域截图
 * 5\保存截图到文件夹或剪切板
 * */

ScreenShotTool::ScreenShotTool(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ScreenShotTool)
{
    ui->setupUi(this);

    connect(ui->btn_start,SIGNAL(clicked(bool)),this,SLOT(slt_ss_start()));
}

ScreenShotTool::~ScreenShotTool()
{
    delete ui;
}

void ScreenShotTool::slt_ss_start()
{
    QScreen *screen = QGuiApplication::primaryScreen();
    fullPixmap = screen->grabWindow(0);

    screenCanvas=new Canvas(0);     //创建画布

    screenCanvas->setbgPixmap(fullPixmap);
}


