#include "screenshottool.h"
#include "ui_screenshottool.h"

#include <QDebug>
#include <QSettings>

#include "operateSet.h"

#include <QTranslator>


/*
 * Author:qiuzhiqian
 * Email:xia_mengliang@163.com
 * Github:https://github.com/qiuzhiqian
 * Date:2017.07.23
 * Description:这个类主要用来进行截图工具的设置
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
    this->setWindowTitle(tr("Setting"));
    this->setWindowIcon(QIcon(":/ss.ico"));

    keystring=new KeyString();

    sc_set=new HotKeyBar();
    sc_set->setKeyString(keystring);                //传递keystring指针
    sc_set->setReadOnly(true);
    ui->gridLayout->addWidget(sc_set,0,1);
    connect(sc_set,SIGNAL(sgn_hotKeyChanged(Qt::Key,Qt::KeyboardModifiers)),this,SLOT(slt_changeHotKey(Qt::Key,Qt::KeyboardModifiers)));
    connect(ui->checkBox,SIGNAL(stateChanged(int)),this,SLOT(slt_auto_run(int)));
    connect(ui->comboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(slt_language_set(int)));

    initTray();                             //托盘显示

    QString keystr=OperateSet::readSetting("HotKey","ScreenShot","Ctrl+F1").toString();
    sc_set->setShowText(keystr);        //设置快捷键显示条内容

    isAutoRun=bool(OperateSet::readSetting("Setting","AutoRun",0).toInt());
    ui->checkBox->setChecked(isAutoRun);
    setAutoRun(isAutoRun);

    QString language=OperateSet::readSetting("Setting","Language","en").toString();
    changeLanguage(language);
    if(language=="en")
    {
        ui->comboBox->setCurrentIndex(0);
    }
    else
    {
        ui->comboBox->setCurrentIndex(1);
    }

    this->setFixedSize(this->size());

    //设置快捷键
    shortcut = new QxtGlobalShortcut(this);
    shortcut->setShortcut(QKeySequence(keystr));
    connect(shortcut, SIGNAL(activated()),this,SLOT(slt_doShot()));
}

ScreenShotTool::~ScreenShotTool()
{
    //unregisterHotKey(key,mods);
    delete ui;
}

void ScreenShotTool::ss_start()             //开始截屏
{
    QScreen *screen = QGuiApplication::primaryScreen();
    fullPixmap = screen->grabWindow(0);

    screenCanvas=new Canvas(0);     //创建画布

    screenCanvas->setbgPixmap(fullPixmap);  //传递全屏背景图片

    if(ui->comboBox->currentIndex()==0)
    {
        screenCanvas->changeLanguage("en");
    }
    else if(ui->comboBox->currentIndex()==1)
    {
        screenCanvas->changeLanguage("zh_cn");
    }
}

void ScreenShotTool::initTray()             //初始化托盘
{
    m_systemTray = new QSystemTrayIcon(this);
    m_systemTray->setIcon(QIcon(":/ss.ico"));
    m_systemTray->setToolTip("ScreenShot");

    setAction=new QAction(tr("Setting"),this);
    aboutAction=new QAction(tr("About"),this);
    exitAction=new QAction(tr("Quit"),this);

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

void ScreenShotTool::setHotKey()
{
    key=Qt::Key_F1;
    mods=Qt::ControlModifier;
    //registerHotKey(key,mods);
}

void ScreenShotTool::slt_changeHotKey(Qt::Key t_key, Qt::KeyboardModifiers t_mod)      //快捷键修改
{
    //unregisterHotKey(key,mods);
    key=t_key;
    mods=t_mod;
    //registerHotKey(key,mods);

    QString strval=keystring->Key2String(key,mods);

    OperateSet::writeSetting("HotKey","ScreenShot",strval);

    qDebug()<<strval;
    shortcut->setShortcut(QKeySequence(strval));
}

void ScreenShotTool::slt_auto_run(int states)
{
    if(states == Qt::Checked)
    {
        isAutoRun=true;
        OperateSet::writeSetting("Setting","AutoRun",1);
    }
    else
    {
        isAutoRun=false;
        OperateSet::writeSetting("Setting","AutoRun",0);
    }

    setAutoRun(isAutoRun);
}

void ScreenShotTool::slt_language_set(int index)
{
    if(index==0)        //en
    {
        changeLanguage("en");
    }
    else if(index==1)   //zh_cn
    {
        changeLanguage("zh_cn");
    }
}

void ScreenShotTool::setAutoRun(bool sta)
{
    QSettings *reg=new QSettings("HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run",QSettings::NativeFormat);
    if(sta == true)
    {
        reg->setValue("ScreenShotTool",QApplication::applicationFilePath().replace("/","\\"));
    }
    else
    {
        reg->remove("ScreenShotTool");
    }
}

void ScreenShotTool::changeLanguage(QString lan)
{
    QTranslator translator;
    bool sta=false;

    if(lan=="zh_cn")    //简体中文
    {
        sta=translator.load("zh_cn.qm");
        OperateSet::writeSetting("Setting","Language","zh_cn");
    }
    else if(lan=="en")
    {
        sta=translator.load("en.qm");
        OperateSet::writeSetting("Setting","Language","en");
    }

    if(sta)
    {
        QApplication::installTranslator(&translator);
        ui->retranslateUi(this);        //刷新ui语言翻译
        reFrash();                      //刷新自定义语言翻译
    }
}

void ScreenShotTool::slt_doShot(void)
{
    qDebug()<<"doShot";
    ss_start();
}

void ScreenShotTool::reFrash()
{
    this->setWindowTitle(tr("Setting"));
    setAction->setText(tr("Setting"));
    aboutAction->setText(tr("About"));
    exitAction->setText(tr("Quit"));
}
