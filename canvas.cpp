#include "canvas.h"

#include <QFileInfo>

#include <QDebug>

#include <QDateTime>

#include "operateSet.h"
#include <QTranslator>

/*
 * Author:qiuzhiqian
 * Email:xia_mengliang@163.com
 * Github:https://github.com/qiuzhiqian
 * Date:2017.07.23
 * Description:这个类主要用来创建一个桌面的截图画布
 **/

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

    setMouseTracking(true);                 //鼠标移动捕捉

    canvasInit();
}

void Canvas::canvasInit()
{
    pointS.rx()=0;
    pointS.ry()=0;
    pointE.rx()=0;
    pointE.ry()=0;
    pointDrag.rx()=0;
    pointDrag.ry()=0;
    rectFlag=0;

    lineList.clear();
    rectList.clear();
    ellipseList.clear();

    hideToolBar();
}

void Canvas::mousePressEvent(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton)
    {
        if(rectFlag==0)
        {
            pointS.rx()=event->x();
            pointS.ry()=event->y();
            pointE.rx()=event->x();
            pointE.ry()=event->y();
            shotArea=getRectF(pointS,pointE);
            rectFlag=1;
        }
        else if(rectFlag==2)    //捕捉拖拽
        {
            pointDrag.setX(event->x());
            pointDrag.setY(event->y());
        }
        //update();
        //dragFlag=0;
    }
    else if(event->button()==Qt::RightButton)       //重新绘制区域
    {
        setCursor(Qt::ArrowCursor);

        if(rectFlag>0)
        {
            canvasInit();

            update();
        }
        else
        {
            slt_cancel();
        }
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
            shotArea=getRectF(pointS,pointE);
        }
        else if(rectFlag==2)            //拖拽
        {
            switch(cursorCaptureFlag)
            {
            case 0:                                 //无效区域
                setCursor(Qt::ArrowCursor);
                break;
            case 1:                                 //左上
                setCursor(Qt::SizeFDiagCursor);
                pointS.setX(event->x());
                pointS.setY(event->y());
                shotArea=getRectF(pointS,pointE);
                break;
            case 2:                                 //左下
                setCursor(Qt::SizeBDiagCursor);
                pointS.setX(event->x());
                pointE.setY(event->y());
                shotArea=getRectF(pointS,pointE);
                break;
            case 3:                                 //左边
                setCursor(Qt::SizeHorCursor);
                pointS.setX(event->x());
                shotArea=getRectF(pointS,pointE);
                break;

            case 4:                                 //右上
                setCursor(Qt::SizeBDiagCursor);
                pointE.setX(event->x());
                pointS.setY(event->y());
                shotArea=getRectF(pointS,pointE);
                break;
            case 5:                                 //右下
                setCursor(Qt::SizeFDiagCursor);
                pointE.setX(event->x());
                pointE.setY(event->y());
                shotArea=getRectF(pointS,pointE);
                break;
            case 6:                                 //右边
                setCursor(Qt::SizeHorCursor);
                pointE.setX(event->x());
                shotArea=getRectF(pointS,pointE);
                break;
            case 7:                                 //上边
                setCursor(Qt::SizeVerCursor);
                pointS.setY(event->y());
                shotArea=getRectF(pointS,pointE);
                break;
            case 8:                                 //下边
                setCursor(Qt::SizeVerCursor);
                pointE.setY(event->y());
                shotArea=getRectF(pointS,pointE);
                break;
            case 9:                                 //中央
                setCursor(Qt::SizeAllCursor);
                qreal dx=event->x()-pointDrag.x();          //获取坐标差
                qreal dy=event->y()-pointDrag.y();

                pointDrag.setX(event->x());                 //刷新拖拽点坐标
                pointDrag.setY(event->y());

                if( (pointS.x()+dx)>0 && (pointE.x()+dx)<screen_width )
                {
                    pointS.rx()+=dx;
                    pointE.rx()+=dx;
                }

                if( (pointS.y()+dy)>0 && (pointE.y()+dy)<screen_height )
                {
                    pointS.ry()+=dy;
                    pointE.ry()+=dy;
                }
                shotArea=getRectF(pointS,pointE);
                break;
            }

            showToolBar();        //实时更新工具条位置
        }
        update();
    }
    else if(event->buttons()==Qt::NoButton)      //没有按键按下
    {
        if(rectFlag==2)    //捕捉拖拽
        {
            cursorCaptureFlag=caputerRect(shotArea,event->x(),event->y());

            switch(cursorCaptureFlag)
            {
            case 0:                                 //无效区域
                setCursor(Qt::ArrowCursor);
                break;
            case 1:                                 //左上
                setCursor(Qt::SizeFDiagCursor);
                break;
            case 2:                                 //左下
                setCursor(Qt::SizeBDiagCursor);
                break;
            case 3:                                 //左边
                setCursor(Qt::SizeHorCursor);
                break;

            case 4:                                 //右上
                setCursor(Qt::SizeBDiagCursor);
                break;
            case 5:                                 //右下
                setCursor(Qt::SizeFDiagCursor);
                break;
            case 6:                                 //右边
                setCursor(Qt::SizeHorCursor);
                break;
            case 7:                                 //上边
                setCursor(Qt::SizeVerCursor);
                break;
            case 8:                                 //下边
                setCursor(Qt::SizeVerCursor);
                break;
            case 9:                                 //中央
                if(drawEditFlag==0)
                {
                    setCursor(Qt::SizeAllCursor);
                }
                else                                //追加图形
                {

                }
                break;
            }

        }
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
            shotArea=getRectF(pointS,pointE);
            rectFlag=2;         //矩形绘制完成
            showToolBar();
        }
        else if(rectFlag==2)
        {
            showToolBar();
        }

        update();
    }
}

void Canvas::paintEvent(QPaintEvent *e)
{

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
        pointS.setX(shotArea.topLeft().x());        //坐标标准化
        pointS.setY(shotArea.topLeft().y());
        pointE.setX(shotArea.bottomRight().x());
        pointE.setY(shotArea.bottomRight().y());
        painter.setPen(QPen(Qt::green,2,Qt::DashLine));//设置画笔形式
        //painter.setBrush(Qt::white);
        painter.drawRect(shotArea);            //然后绘制矩形框
        painter.drawPixmap(shotArea,fullPixmap,shotArea);     //然后将矩形框中的半透明图像替换成原图


        //rectFlag=3;
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

void Canvas::shootScreen(QRectF &rect)      //截图函数
{
    QScreen *screen = QGuiApplication::primaryScreen();

    originalPixmap = screen->grabWindow(0,rect.x(),rect.y(),rect.width(),rect.height());

}

void Canvas::initToolBar()                  //工具条初始化
{
    toolbar=new QWidget(this);
    QHBoxLayout *toolLayout=new QHBoxLayout();

    btn_cancel=new QPushButton(tr("Quit"));
    btn_saveClipboard=new QPushButton(tr("Copy"));
    btn_saveFile=new QPushButton(tr("Save"));

    btn_drawLine=new QPushButton(tr("Line"));
    btn_drawRect=new QPushButton(tr("Rect"));
    btn_drawEllipse=new QPushButton(tr("Ellipse"));

    toolLayout->addWidget(btn_drawLine);
    toolLayout->addWidget(btn_drawRect);
    toolLayout->addWidget(btn_drawEllipse);

    toolLayout->addWidget(btn_cancel);
    toolLayout->addWidget(btn_saveClipboard);
    toolLayout->addWidget(btn_saveFile);
    toolLayout->setContentsMargins(0,0,0,0);            //去除边框间隙
    toolLayout->setSpacing(0);

    toolbar->setLayout(toolLayout);
    toolbar->setVisible(false);

    connect(btn_cancel,SIGNAL(clicked(bool)),this,SLOT(slt_cancel()));
    connect(btn_saveClipboard,SIGNAL(clicked(bool)),this,SLOT(slt_saveClipboard()));
    connect(btn_saveFile,SIGNAL(clicked(bool)),this,SLOT(slt_saveFile()));
}

void Canvas::showToolBar()            //显示工具条
{
    qreal x,y;

    int bar_width=toolbar->width();
    int bar_height=toolbar->height();
    int offset=5;

    if(shotArea.bottomLeft().x()+bar_width+offset<screen_width)      //x轴方向边距足够
    {
        x=shotArea.bottomLeft().x()+offset;
    }
    else                                                //x轴方向边距不足
    {
        x=screen_width-(bar_width+offset);
    }

    if(screen_height-shotArea.bottomLeft().y()>(bar_height+offset))      //下边距充足
    {
        y=shotArea.bottomLeft().y()+offset;
    }
    else if(shotArea.topLeft().y()>(bar_height+offset))                  //上边距充足
    {
        y=shotArea.topLeft().y()-(bar_height+offset);
    }
    else if(shotArea.topLeft().y()<0)                   //上下边距都不够,且上边距在桌面外
    {
        y=offset;
    }
    else                                                //上下边距都不够,且上边距在桌面内
    {
        y=shotArea.topLeft().y()+offset;
    }
    toolbar->move(x,y);
    toolbar->setVisible(true);
}

void Canvas::hideToolBar()                    //掩藏工具条
{
    toolbar->setVisible(false);
}

void Canvas::refrashToolBar()
{
    btn_drawLine->setText(tr("Line"));
    btn_drawRect->setText(tr("Rect"));
    btn_drawEllipse->setText(tr("Ellipse"));
    btn_cancel->setText(tr("Quit"));
    btn_saveClipboard->setText(tr("Copy"));
    btn_saveFile->setText(tr("Save"));
}

void Canvas::slt_drawLine()
{
    if(drawEditFlag==1)
    {
        drawEditFlag=0;
    }
    else
    {
        drawEditFlag=1;
    }
}

void Canvas::slt_drawRect()
{
    if(drawEditFlag==2)
    {
        drawEditFlag=0;
    }
    else
    {
        drawEditFlag=2;
    }
}

void Canvas::slt_drawEllipse()
{
    if(drawEditFlag==3)
    {
        drawEditFlag=0;
    }
    else
    {
        drawEditFlag=3;
    }
}

void Canvas::slt_saveFile()
{
    shootScreen(shotArea);
    QDateTime current_date_time = QDateTime::currentDateTime();
    QString current_date = current_date_time.toString("yyyyMMdd_hhmmss");
    QString savefile="ScreenShot_"+current_date+".jpg";

    QString savepath=OperateSet::readSetting("Setting","QuickSaveDir",".").toString();

    QString fileName = QFileDialog::getSaveFileName(this,
            tr("Save File"),
            savepath+"/"+savefile,
            tr("JPEG File (*.jpg)"));
    originalPixmap.save(fileName,"jpg");

    QFileInfo fi = QFileInfo(fileName);
    //QString file_name = fi.fileName();
    QString file_path = fi.absolutePath();

    if(file_path!=savepath)
    {
        OperateSet::writeSetting("Setting","QuickSaveDir",file_path);
    }


    slt_cancel();
}

void Canvas::slt_saveClipboard()
{
    shootScreen(shotArea);
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

void Canvas::changeLanguage(QString lan)
{
    QTranslator translator;
    bool sta=false;

    if(lan=="zh_cn")    //简体中文
    {
        sta=translator.load("zh_cn.qm");
    }
    else if(lan=="en")
    {
        sta=translator.load("en.qm");
    }

    if(sta)
    {
        QApplication::installTranslator(&translator);
        refrashToolBar();                      //刷新自定义语言翻译
    }
}

quint8 Canvas::caputerRect(QRectF t_rect,qreal t_x,qreal t_y)
{
    quint8 pos=0;
    if(qAbs(t_x-t_rect.topLeft().x())<2)           //左
    {
        if(qAbs(t_y-t_rect.topLeft().y())<2)       //上
        {
            pos=1;
        }
        else if(qAbs(t_y-t_rect.bottomRight().y())<2)   //下
        {
            pos=2;
        }
        else if( (t_y>=t_rect.topLeft().y()+2) && (t_y<=t_rect.bottomRight().y()-2) )  //上下之间
        {
            pos=3;
        }
        else
        {
            pos=0;
        }
    }
    else if(qAbs(t_x-t_rect.bottomRight().x())<2)           //右
    {
        if(qAbs(t_y-t_rect.topLeft().y())<2)                //上
        {
            pos=4;
        }
        else if(qAbs(t_y-t_rect.bottomRight().y())<2)       //下
        {
            pos=5;
        }
        else if( (t_y>=t_rect.topLeft().y()+2) && (t_y<=t_rect.bottomRight().y()-2) )  //上下之间
        {
            pos=6;
        }
        else
        {
            pos=0;
        }
    }
    else if((t_x>=t_rect.topLeft().x()+2) && (t_x<=t_rect.bottomRight().x()-2))          //左右之间
    {
        if(qAbs(t_y-t_rect.topLeft().y())<2)            //上
        {
            pos=7;
        }
        else if(qAbs(t_y-t_rect.bottomRight().y())<2)   //下
        {
            pos=8;
        }
        else if( (t_y>=t_rect.topLeft().y()+2) && (t_y<=t_rect.bottomRight().y()-2) )  //上下之间
        {
            pos=9;
        }
        else
        {
            pos=0;
        }
    }
    else
    {
        pos=0;
    }
    return pos;
}
