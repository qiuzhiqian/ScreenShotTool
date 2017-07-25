#include "screenshottool.h"
#include "ui_screenshottool.h"

#include <QDebug>

/*
 * Author:qiuzhiqian
 * Email:xia_mengliang@163.com
 * Github:https://github.com/qiuzhiqian
 * Date:2017.07.23
 **/

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

/*
 * 截图区域调整策略
 * 左边--pointLT.x
 * 右边--pointRB.x
 * 上边--pointLT.y
 * 下边--pointRB.y
 *
 * 为简化操作，将角调整转化为同时两个边的调整
 * 左上--同时作用左边、上边
 * 右下--同时作用右边、下边
 * 左下--同时作用左边、下边
 * 右上--同时作用右边、上边
 * */

ScreenShotTool::ScreenShotTool(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ScreenShotTool)
{
    ui->setupUi(this);

    QWidget * xparent = new QWidget;        //设置一个空的父对象，从而达到掩藏任务栏图标的目的

    setParent(xparent);

    this->setWindowFlags(Qt::Dialog|Qt::WindowCloseButtonHint|Qt::WindowStaysOnTopHint);
    this->setWindowTitle(tr("设置"));
    this->setWindowIcon(QIcon(":/ss.png"));
    this->setAttribute(Qt::WA_ShowModal, true);
    this->setWindowModality(Qt::ApplicationModal);

    sc_set=new ShotCut(this);
    sc_set->setReadOnly(true);
    sc_set->setGeometry(110,30,113,20);

    initTray();                             //托盘显示
}

ScreenShotTool::~ScreenShotTool()
{
    delete ui;
}

void ScreenShotTool::ss_start()
{
    QScreen *screen = QGuiApplication::primaryScreen();
    fullPixmap = screen->grabWindow(0);

    screenCanvas=new Canvas(0);     //创建画布

    screenCanvas->setbgPixmap(fullPixmap);  //传递全屏背景图片
}

void ScreenShotTool::initTray()
{
    m_systemTray = new QSystemTrayIcon(this);
    m_systemTray->setIcon(QIcon(":/ss.png"));
    m_systemTray->setToolTip("ScreenShot");

    setAction=new QAction(tr("设置"),this);
    aboutAction=new QAction(tr("关于"),this);
    exitAction=new QAction(tr("退出"),this);

    QMenu *trayMenu=new QMenu(this);
    trayMenu->addAction(setAction);
    trayMenu->addAction(aboutAction);
    trayMenu->addSeparator();
    trayMenu->addAction(exitAction);

    m_systemTray->setContextMenu(trayMenu);

    m_systemTray->show();

    connect(m_systemTray,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),this,SLOT(slt_clickTray(QSystemTrayIcon::ActivationReason)));
    connect(setAction,SIGNAL(triggered(bool)),this,SLOT(slt_setAction()));
    connect(aboutAction,SIGNAL(triggered(bool)),this,SLOT(slt_aboutAction()));
    connect(exitAction,SIGNAL(triggered(bool)),this,SLOT(slt_exitAction()));
}

void ScreenShotTool::slt_clickTray(QSystemTrayIcon::ActivationReason reason)
{
    switch(reason)
    {
    case QSystemTrayIcon::Trigger:
        //单击托盘图标,开始截图
        ss_start();
        break;
    case QSystemTrayIcon::DoubleClick:
        //双击托盘图标
        //双击后显示设置窗口
        this->show();
        break;
    default:
        break;
    }
}

void ScreenShotTool::slt_setAction()
{
    this->show();
}

void ScreenShotTool::slt_aboutAction()
{

}

void ScreenShotTool::slt_exitAction()
{
    closeFlag=true;
    this->close();
    QApplication::quit();       //程序退出
}

void ScreenShotTool::closeEvent(QCloseEvent *event)
{
    if(closeFlag==true)
    {
        event->accept();
    }
    else
    {
        this->hide();
        event->ignore();
    }
}


