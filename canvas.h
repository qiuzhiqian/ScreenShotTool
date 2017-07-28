#ifndef CANVAS_H
#define CANVAS_H

/*
 * Author:qiuzhiqian
 * Email:xia_mengliang@163.com
 * Github:https://github.com/qiuzhiqian
 * Date:2017.07.23
 **/

#include <QApplication>
#include <QWidget>
#include <QLabel>
#include <QMouseEvent>
#include <QPainter>
#include <QPaintEvent>
#include <QDesktopWidget>
#include <QScreen>
#include <QPixmap>
#include <QGuiApplication>
#include <QPen>
#include <QApplication>

#include <QPushButton>

#include <QFileDialog>

#include <QHBoxLayout>
#include <QClipboard>


class Canvas : public QWidget
{
    Q_OBJECT
public:
    explicit Canvas(QWidget *parent = 0);

    void setbgPixmap(QPixmap &px);
    void shootScreen(QRectF &rect);

    void initToolBar();
    void showToolBar();
    void hideToolBar();

    QRectF getRectF(QPointF p1,QPointF p2);    //通过两个坐标点生成矩形

signals:

public slots:
    void slt_saveFile();            //保存到文件
    void slt_saveClipboard();       //保存到剪切板
    void slt_cancel();

private:
    int screen_width=0,screen_height=0;
    quint8 okFlag=0;

    QPointF pointS;     //鼠标绘制起点
    QPointF pointE;     //鼠标绘制终点
    QRectF shotArea;    //截图区域

    QPointF pointDrag;  //拖拽点

    quint8 rectFlag=0;

    QPixmap fullPixmap;         //原始全屏图片
    QPixmap originalPixmap;

    QWidget *toolbar;           //工具条
    QPushButton *btn_saveFile;
    QPushButton *btn_saveClipboard;
    QPushButton *btn_cancel;

    QClipboard *clipboard;

    quint8 cursorCaptureFlag=0;

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *e);
};

#endif // CANVAS_H
