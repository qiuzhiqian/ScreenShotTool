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

    //this->setWindowFlags(Qt::WindowStaysOnTopHint);     //窗口置顶
    this->raise();                      //软置顶
    this->showFullScreen();             //画布全屏显示

    clipboard = QApplication::clipboard();   //获取系统剪贴板指针

    setMouseTracking(true);                 //鼠标移动捕捉

    canvasInit();

    initToolBar();

    drawEditFlag=0;
}

void Canvas::canvasInit()
{
    pointS.rx()=0;
    pointS.ry()=0;
    pointE.rx()=0;
    pointE.ry()=0;
    pointDrag.rx()=0;
    pointDrag.ry()=0;
    rectFlag=DrawStatus::waitDraw;

    lineList.clear();
    rectList.clear();
    ellipseList.clear();

    drawPen.setBrush(Qt::red);
    drawPen.setWidthF(2);
    drawPen.setStyle(Qt::SolidLine);
}

void Canvas::mousePressEvent(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton)
    {
        if(rectFlag==DrawStatus::waitDraw)
        {
            pointS.rx()=event->x();
            pointS.ry()=event->y();
            pointE.rx()=event->x();
            pointE.ry()=event->y();
            shotArea=getRectF(pointS,pointE);
            rectFlag=DrawStatus::drawing;
        }
        else if(rectFlag==DrawStatus::drawed)    //捕捉拖拽
        {
            if(drawEditFlag==0)
            {
                pointDrag.setX(event->x());
                pointDrag.setY(event->y());
            }
            else if(drawEditFlag==1)        //画线
            {
                pointS.rx()=event->x();
                pointS.ry()=event->y();
                pointE.rx()=event->x();
                pointE.ry()=event->y();

                LinePaint tempLine(pointS.toPoint(),pointE.toPoint());
                //qDebug()<<"drawPen";
                tempLine.setPen(drawPen);
                lineList.append(tempLine);
            }
            else if(drawEditFlag==2)        //画矩形
            {
                pointS.rx()=event->x();
                pointS.ry()=event->y();
                pointE.rx()=event->x();
                pointE.ry()=event->y();

                RectPaint tempRect=getRectF(pointS,pointE);
                tempRect.setPen(drawPen);
                rectList.append(tempRect);
            }
            else if(drawEditFlag==3)        //画椭圆
            {
                pointS.rx()=event->x();
                pointS.ry()=event->y();
                pointE.rx()=event->x();
                pointE.ry()=event->y();

                RectPaint tempRect=getRectF(pointS,pointE);
                tempRect.setPen(drawPen);
                ellipseList.append(tempRect);
            }
        }
    }
    else if(event->button()==Qt::RightButton)       //重新绘制区域
    {
        setCursor(Qt::ArrowCursor);

        if(rectFlag==DrawStatus::waitDraw)
        {
            slt_cancel();
        }
        else
        {
            canvasInit();
            hideToolBar();
            update();
        }
    }
}

void Canvas::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons()&Qt::LeftButton)
    {
        if(rectFlag==DrawStatus::drawing)
        {
            pointE.setX(event->x());
            pointE.setY(event->y());
            shotArea=getRectF(pointS,pointE);
        }
        else if(rectFlag==DrawStatus::drawed)            //拖拽
        {
            QPointF tempTL,tempBR;

            tempTL=shotArea.topLeft();
            tempBR=shotArea.bottomRight();

            switch(cursorCaptureFlag)
            {
            case 0:                                 //无效区域
                setCursor(Qt::ArrowCursor);
                break;
            case 1:                                 //左上
                setCursor(Qt::SizeFDiagCursor);
                tempTL.setX(event->x());
                tempTL.setY(event->y());
                break;
            case 2:                                 //左下
                setCursor(Qt::SizeBDiagCursor);
                tempTL.setX(event->x());
                tempBR.setY(event->y());
                break;
            case 3:                                 //左边
                setCursor(Qt::SizeHorCursor);
                tempTL.setX(event->x());
                break;

            case 4:                                 //右上
                setCursor(Qt::SizeBDiagCursor);
                tempBR.setX(event->x());
                tempTL.setY(event->y());
                break;
            case 5:                                 //右下
                setCursor(Qt::SizeFDiagCursor);
                tempBR.setX(event->x());
                tempBR.setY(event->y());
                break;
            case 6:                                 //右边
                setCursor(Qt::SizeHorCursor);
                tempBR.setX(event->x());
                shotArea=getRectF(pointS,pointE);
                break;
            case 7:                                 //上边
                setCursor(Qt::SizeVerCursor);
                tempTL.setY(event->y());
                break;
            case 8:                                 //下边
                setCursor(Qt::SizeVerCursor);
                tempBR.setY(event->y());
                break;
            case 9:                                 //中央
                if(drawEditFlag==0)
                {
                    setCursor(Qt::SizeAllCursor);
                    qreal dx=event->x()-pointDrag.x();          //获取坐标差
                    qreal dy=event->y()-pointDrag.y();

                    pointDrag.setX(event->x());                 //刷新拖拽点坐标
                    pointDrag.setY(event->y());

                    if( (tempTL.x()+dx)>0 && (tempBR.x()+dx)<screen_width )
                    {
                        tempTL.rx()+=dx;
                        tempBR.rx()+=dx;
                    }

                    if( (tempTL.y()+dy)>0 && (tempBR.y()+dy)<screen_height )
                    {
                        tempTL.ry()+=dy;
                        tempBR.ry()+=dy;
                    }
                }
                else if(drawEditFlag==1)                //绘线
                {
                    pointE.setX(event->x());
                    pointE.setY(event->y());

                    lineList.last().setPoints(pointS.toPoint(),pointE.toPoint());
                }
                else if(drawEditFlag==2)                //绘矩形
                {
                    pointE.setX(event->x());
                    pointE.setY(event->y());

                    rectList.last().setTopLeft(pointS);
                    rectList.last().setBottomRight(pointE);
                }
                else if(drawEditFlag==3)                //绘椭圆
                {
                    pointE.setX(event->x());
                    pointE.setY(event->y());

                    ellipseList.last().setTopLeft(pointS);
                    ellipseList.last().setBottomRight(pointE);
                }

                break;
            }

            shotArea.setTopLeft(tempTL);
            shotArea.setBottomRight(tempBR);

            showToolBar();        //实时更新工具条位置
        }
        update();
    }
    else if(event->buttons()==Qt::NoButton)      //没有按键按下
    {
        if(rectFlag==DrawStatus::drawed)    //捕捉拖拽
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
                    setCursor(Qt::ArrowCursor);
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
        if(rectFlag==DrawStatus::drawing)
        {
            pointE.setX(event->x());
            pointE.setY(event->y());
            shotArea=getRectF(pointS,pointE);
            rectFlag=DrawStatus::drawed;         //矩形绘制完成
            showToolBar();
        }
        else if(rectFlag==DrawStatus::drawed)
        {
            if(drawEditFlag==0)
            {
                showToolBar();
            }
            else if(drawEditFlag==1)
            {
                if( (pointS.x()==pointE.x()) && (pointS.y()==pointE.y()) )//直线长度为0
                {
                    lineList.removeLast();
                }
            }
            else if(drawEditFlag==2)
            {
                if( (pointS.x()==pointE.x()) && (pointS.y()==pointE.y()) )//矩形区域为空
                {
                    rectList.removeLast();
                }
            }
            else if(drawEditFlag==3)
            {
                if( (pointS.x()==pointE.x()) && (pointS.y()==pointE.y()) )//矩形区域为空
                {
                    ellipseList.removeLast();
                }
            }
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
    case DrawStatus::waitDraw:
    {
        break;
    }
    case DrawStatus::drawing:
    {
        painter.setPen(QPen(Qt::green,2,Qt::DashLine));//设置画笔形式
        //painter.setBrush(Qt::white);
        painter.drawRect(shotArea);            //然后绘制矩形框
        painter.drawPixmap(shotArea,fullPixmap,shotArea);     //然后将矩形框中的半透明图像替换成原图
        break;
    }
    case DrawStatus::drawed:
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

    quint16 len=lineList.length();
    if(len)
    {
        for(quint16 i=0;i<len;i++)
        {
            painter.setPen(lineList[i].getPen());//设置画笔形式
            painter.drawLine(lineList[i]);            //然后绘制矩形框
        }
    }

    len=rectList.length();
    if(len)
    {
        for(quint16 i=0;i<len;i++)
        {
            painter.setPen(rectList[i].getPen());//设置画笔形式
            painter.drawRect(rectList[i]);            //然后绘制矩形框
        }
    }

    len=ellipseList.length();
    if(len)
    {
        for(quint16 i=0;i<len;i++)
        {
            painter.setPen(ellipseList[i].getPen());//设置画笔形式
            painter.drawEllipse(ellipseList[i]);            //然后绘制矩形框
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

    QWidget *MainToolBar=new QWidget();                    //主工具栏
    QHBoxLayout *mainToolLayout=new QHBoxLayout();

    btn_cancel=new QPushButton(tr("Quit"));
    btn_saveClipboard=new QPushButton(tr("Copy"));
    btn_saveFile=new QPushButton(tr("Save"));

    btn_drawLine=new QPushButton(tr("Line"));
    btn_drawRect=new QPushButton(tr("Rect"));
    btn_drawEllipse=new QPushButton(tr("Ellipse"));

    btn_cancel->setStyleSheet("background-color: rgb(255, 255, 255);");
    btn_saveClipboard->setStyleSheet("background-color: rgb(255, 255, 255);");
    btn_saveFile->setStyleSheet("background-color: rgb(255, 255, 255);");

    mainToolLayout->addWidget(btn_drawLine);
    mainToolLayout->addWidget(btn_drawRect);
    mainToolLayout->addWidget(btn_drawEllipse);
    mainToolLayout->addWidget(btn_cancel);
    mainToolLayout->addWidget(btn_saveClipboard);
    mainToolLayout->addWidget(btn_saveFile);
    mainToolLayout->setContentsMargins(0,0,0,0);            //去除边框间隙
    mainToolLayout->setSpacing(0);
    MainToolBar->setLayout(mainToolLayout);

    shapeToolBar=new QWidget();                             //拓展工具条
    QHBoxLayout *shapeToolLayout=new QHBoxLayout();

    cbx_lineSize=new QComboBox();
    btn_colorSelect=new QPushButton();
    cbx_lineStyle=new QComboBox();

    QStringList sizeitems;
    sizeitems<<"1"<<"2"<<"3"<<"4"<<"5"<<"6"<<"7"<<"8"<<"9"<<"10"<<"11"<<"12"<<"13"<<"14"<<"15"<<"16"<<"17"<<"18"<<"19"<<"20";
    cbx_lineSize->addItems(sizeitems);
    btn_colorSelect->setStyleSheet("background-color: rgb(255, 0, 0);");

    cbx_lineStyle->addItem(QIcon(":/pic/1.ico"),"");
    cbx_lineStyle->addItem(QIcon(":/pic/2.ico"),"");
    cbx_lineStyle->addItem(QIcon(":/pic/3.ico"),"");
    cbx_lineStyle->addItem(QIcon(":/pic/4.ico"),"");
    cbx_lineStyle->addItem(QIcon(":/pic/5.ico"),"");
    cbx_lineStyle->setIconSize(QSize(80,20));
    shapeToolLayout->addWidget(cbx_lineSize);
    shapeToolLayout->addWidget(btn_colorSelect);
    shapeToolLayout->addWidget(cbx_lineStyle);
    shapeToolLayout->addStretch();
    shapeToolLayout->setContentsMargins(0,0,0,0);            //去除边框间隙
    shapeToolLayout->setSpacing(0);
    shapeToolBar->setLayout(shapeToolLayout);
    shapeToolBar->setVisible(false);

    //初始化拓展工具的初始状态
    cbx_lineSize->setCurrentText(QString::number(drawPen.width()));
    QColor color=drawPen.brush().color();
    QString colorStyle=QString("background-color: rgb(%1, %2, %3);").arg(color.red()).arg(color.green()).arg(color.blue());
    btn_colorSelect->setStyleSheet(colorStyle);
    switch(drawPen.style())
    {
    case Qt::SolidLine:
        cbx_lineStyle->setCurrentIndex(0);
        break;
    case Qt::DashLine:
        cbx_lineStyle->setCurrentIndex(1);
        break;
    case Qt::DotLine:
        cbx_lineStyle->setCurrentIndex(2);
        break;
    case Qt::DashDotLine:
        cbx_lineStyle->setCurrentIndex(3);
        break;
    case Qt::DashDotDotLine:
        cbx_lineStyle->setCurrentIndex(4);
        break;
    default:
        cbx_lineStyle->setCurrentIndex(0);
        break;
    }

    QVBoxLayout *toolLayout=new QVBoxLayout();
    toolLayout->addWidget(MainToolBar);
    toolLayout->addWidget(shapeToolBar);
    toolLayout->setContentsMargins(0,0,0,0);            //去除边框间隙
    toolLayout->setSpacing(0);
    toolbar->setLayout(toolLayout);
    toolbar->setVisible(false);

    btn_drawLine->setStyleSheet("background-color: rgb(255, 255, 255);");
    btn_drawRect->setStyleSheet("background-color: rgb(255, 255, 255);");
    btn_drawEllipse->setStyleSheet("background-color: rgb(255, 255, 255);");

    hideToolBar();

    connect(btn_drawLine,SIGNAL(clicked(bool)),this,SLOT(slt_drawLine()));
    connect(btn_drawRect,SIGNAL(clicked(bool)),this,SLOT(slt_drawRect()));
    connect(btn_drawEllipse,SIGNAL(clicked(bool)),this,SLOT(slt_drawEllipse()));
    connect(btn_cancel,SIGNAL(clicked(bool)),this,SLOT(slt_cancel()));
    connect(btn_saveClipboard,SIGNAL(clicked(bool)),this,SLOT(slt_saveClipboard()));
    connect(btn_saveFile,SIGNAL(clicked(bool)),this,SLOT(slt_saveFile()));

    //拓展工具信号槽
    connect(cbx_lineSize,SIGNAL(currentTextChanged(QString)),this,SLOT(slt_changePenWidth(QString)));
    connect(btn_colorSelect,SIGNAL(clicked(bool)),this,SLOT(slt_changePenColor()));
    connect(cbx_lineStyle,SIGNAL(currentIndexChanged(int)),SLOT(slt_changePenStyle(int)));
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
    if(drawEditFlag!=1)
    {
        drawEditFlag=1;
        btn_drawLine->setStyleSheet("background-color: rgb(146, 189, 108);");
        btn_drawRect->setStyleSheet("background-color: rgb(255, 255, 255);");
        btn_drawEllipse->setStyleSheet("background-color: rgb(255, 255, 255);");
        shapeToolBar->setVisible(true);
        toolbar->adjustSize();
        showToolBar();
    }
    else
    {
        drawEditFlag=0;
        btn_drawLine->setStyleSheet("background-color: rgb(255, 255, 255);");
        btn_drawRect->setStyleSheet("background-color: rgb(255, 255, 255);");
        btn_drawEllipse->setStyleSheet("background-color: rgb(255, 255, 255);");
        shapeToolBar->setVisible(false);
        toolbar->adjustSize();
        showToolBar();
    }
}

void Canvas::slt_drawRect()
{
    if(drawEditFlag!=2)
    {
        drawEditFlag=2;
        btn_drawLine->setStyleSheet("background-color: rgb(255, 255, 255);");
        btn_drawRect->setStyleSheet("background-color: rgb(146, 189, 108);");
        btn_drawEllipse->setStyleSheet("background-color: rgb(255, 255, 255);");
        shapeToolBar->setVisible(true);
        toolbar->adjustSize();
        showToolBar();
    }
    else
    {
        drawEditFlag=0;
        btn_drawLine->setStyleSheet("background-color: rgb(255, 255, 255);");
        btn_drawRect->setStyleSheet("background-color: rgb(255, 255, 255);");
        btn_drawEllipse->setStyleSheet("background-color: rgb(255, 255, 255);");
        shapeToolBar->setVisible(false);
        toolbar->adjustSize();
        showToolBar();
    }
}

void Canvas::slt_drawEllipse()
{
    if(drawEditFlag!=3)
    {
        drawEditFlag=3;
        btn_drawLine->setStyleSheet("background-color: rgb(255, 255, 255);");
        btn_drawRect->setStyleSheet("background-color: rgb(255, 255, 255);");
        btn_drawEllipse->setStyleSheet("background-color: rgb(146, 189, 108);");
        shapeToolBar->setVisible(true);
        toolbar->adjustSize();
        showToolBar();
    }
    else
    {
        drawEditFlag=0;
        btn_drawLine->setStyleSheet("background-color: rgb(255, 255, 255);");
        btn_drawRect->setStyleSheet("background-color: rgb(255, 255, 255);");
        btn_drawEllipse->setStyleSheet("background-color: rgb(255, 255, 255);");
        shapeToolBar->setVisible(false);
        toolbar->adjustSize();
        showToolBar();
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
    rectFlag=DrawStatus::waitDraw;
    this->close();
}

void Canvas::slt_changePenWidth(QString s)
{
    drawPen.setWidth(s.toInt());
}

void Canvas::slt_changePenColor()
{
    QColor color = QColorDialog::getColor(Qt::blue);
    if(color.isValid())
    {
        //如果颜色有效的话，将colorFrame设置为选中的颜色
        //colorFrame->setPalette(QPalette(color));
        //btn_colorSelect->setPalette(QPalette(color));
        QString colorStyle=QString("background-color: rgb(%1, %2, %3);").arg(color.red()).arg(color.green()).arg(color.blue());
        btn_colorSelect->setStyleSheet(colorStyle);
    }
    drawPen.setColor(color);
}

void Canvas::slt_changePenStyle(int index)
{
    switch(index)
    {
    case 0:
        drawPen.setStyle(Qt::SolidLine);
        break;
    case 1:
        drawPen.setStyle(Qt::DashLine);
        break;
    case 2:
        drawPen.setStyle(Qt::DotLine);
        break;
    case 3:
        drawPen.setStyle(Qt::DashDotLine);
        break;
    case 4:
        drawPen.setStyle(Qt::DashDotDotLine);
        break;
    default:
        drawPen.setStyle(Qt::SolidLine);
        break;
    }
}

//通过任意两点构造一个矩形
RectPaint Canvas::getRectF(QPointF p1, QPointF p2)
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
    RectPaint rect(ps,pe);
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
