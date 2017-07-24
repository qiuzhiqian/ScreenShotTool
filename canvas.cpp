#include "canvas.h"
#include <QDebug>

/*
 * Author:qiuzhiqian
 * Email:xia_mengliang@163.com
 * Github:https://github.com/qiuzhiqian
 * Date:2017.07.23
 **/

#define cmp_min(x1,x2)  do{(x1<x2) ?  x1 : x2}while(0)
#define cmp_max(x1,x2)  do{(x1<x2) ?  x1 : x2}while(0)

//考虑继承QLabel
Canvas::Canvas(QWidget *parent) : QWidget(parent)
{
    okFlag=1;
    const QRect screenGeometry = QApplication::desktop()->screenGeometry(this);
    screen_width=screenGeometry.width();
    screen_height=screenGeometry.height();

    this->setWindowFlags(Qt::WindowStaysOnTopHint);     //窗口置顶
    this->showFullScreen();             //画布全屏显示

    initToolBar();

    clipboard = QApplication::clipboard();   //获取系统剪贴板指针
}

void Canvas::mousePressEvent(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton)
    {
        if(rectFlag==0)
        {
            pointS.setX(event->x());
            pointS.setY(event->y());
            pointE.setX(event->x());
            pointE.setY(event->y());
            rectFlag=1;
        }
        //update();
        //dragFlag=0;
    }
    else if(event->button()==Qt::RightButton)
    {
        slt_cancel();
    }
}

void Canvas::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons()&Qt::LeftButton)
    {
        if(rectFlag==1)
        {
            pointE.setX(event->x());
            pointE.setY(event->y());
            //dragFlag=1;
        }
        else
        {

        }


        update();

    }
    else if(event->buttons()&Qt::NoButton)      //没有按键按下
    {

    }
}

void Canvas::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton)
    {
        if(rectFlag==1)
        {
            pointE.setX(event->x());
            pointE.setY(event->y());
            rectFlag=2;         //矩形绘制完成
            addToolBar(shotArea.bottomLeft().x(),shotArea.bottomLeft().y());
        }

        update();
    }
}

void Canvas::paintEvent(QPaintEvent *e)
{
    shotArea=getRectF(pointS,pointE);

    QPainter painter(this);

    QPixmap tempmask(screen_width, screen_width);
    tempmask.fill((QColor(0, 0, 0, 160)));

    painter.drawPixmap(0,0,fullPixmap);     //先绘制全屏原图背景
    painter.drawPixmap(0,0,tempmask);       //然后绘制半透明背景，用来降低亮度

    switch(rectFlag)        //截图状态机
    {
    case 0:
    {
        break;
    }
    case 1:
    {
        painter.setPen(QPen(Qt::green,2,Qt::DashLine));//设置画笔形式
        //painter.setBrush(Qt::white);
        painter.drawRect(shotArea);            //然后绘制矩形框
        painter.drawPixmap(shotArea,fullPixmap,shotArea);     //然后将矩形框中的半透明图像替换成原图
        break;
    }
    case 2:
    {
        painter.setPen(QPen(Qt::green,2,Qt::DashLine));//设置画笔形式
        //painter.setBrush(Qt::white);
        painter.drawRect(shotArea);            //然后绘制矩形框
        painter.drawPixmap(shotArea,fullPixmap,shotArea);     //然后将矩形框中的半透明图像替换成原图
        shootScreen(shotArea);

        rectFlag=3;
        break;
    }
    case 3:
    {
        painter.setPen(QPen(Qt::green,2,Qt::DashLine));//设置画笔形式
        //painter.setBrush(Qt::white);
        painter.drawRect(shotArea);            //然后绘制矩形框
        painter.drawPixmap(shotArea,fullPixmap,shotArea);     //然后将矩形框中的半透明图像替换成原图
        break;
    }
    default:
    {
        break;
    }
    }
    QWidget::paintEvent(e);
}

void Canvas::setbgPixmap(QPixmap &px)
{
    fullPixmap=px;
}

void Canvas::shootScreen(QRectF &rect)
{
    QScreen *screen = QGuiApplication::primaryScreen();

    originalPixmap = screen->grabWindow(0,rect.x(),rect.y(),rect.width(),rect.height());

}

void Canvas::initToolBar()
{
    toolbar=new QWidget(this);
    QHBoxLayout *toolLayout=new QHBoxLayout();

    btn_cancel=new QPushButton(tr("取消"));
    btn_saveClipboard=new QPushButton(tr("复制"));
    btn_saveFile=new QPushButton(tr("保存"));

    toolLayout->addWidget(btn_cancel);
    toolLayout->addWidget(btn_saveClipboard);
    toolLayout->addWidget(btn_saveFile);

    toolbar->setLayout(toolLayout);
    toolbar->setVisible(false);

    connect(btn_cancel,SIGNAL(clicked(bool)),this,SLOT(slt_cancel()));
    connect(btn_saveClipboard,SIGNAL(clicked(bool)),this,SLOT(slt_saveClipboard()));
    connect(btn_saveFile,SIGNAL(clicked(bool)),this,SLOT(slt_saveFile()));
}

void Canvas::addToolBar(int x,int y)
{
    toolbar->setGeometry(x,y,180,50);
    toolbar->setVisible(true);
}

void Canvas::deleteToolBar()
{
    toolbar->setVisible(false);
}

void Canvas::slt_saveFile()
{
    QString fileName = QFileDialog::getSaveFileName(this,
            tr("Save File"),
            ".",
            tr("JPEG File (*.jpg)"));
    originalPixmap.save(fileName,"jpg");

    slt_cancel();
}

void Canvas::slt_saveClipboard()
{
    clipboard->setPixmap(originalPixmap);
    slt_cancel();
}

void Canvas::slt_cancel()
{
    rectFlag=0;
    this->close();
}

//通过任意两点构造一个矩形
QRectF Canvas::getRectF(QPointF p1, QPointF p2)
{
    float x1,y1,x2,y2;
    if(p1.x()<p2.x())
    {
        x1=p1.x();
        x2=p2.x();
    }
    else
    {
        x1=p2.x();
        x2=p1.x();
    }

    if(p1.y()<p2.y())
    {
        y1=p1.y();
        y2=p2.y();
    }
    else
    {
        y1=p2.y();
        y2=p1.y();
    }

    QPointF ps(x1,y1);
    QPointF pe(x2,y2);
    QRectF rect(ps,pe);
    return rect;
}
