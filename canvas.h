#ifndef CANVAS_H
#define CANVAS_H

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
    void shootScreen(int x=0,int y=0,int width=-1,int height=-1);

    void initToolBar();
    void addToolBar();
    void deleteToolBar();

signals:

public slots:
    void slt_saveFile();            //保存到文件
    void slt_saveClipboard();       //保存到剪切板
    void slt_cancel();

private:
    int screen_width=0,screen_height=0;
    quint8 okFlag=0;
    int start_x=0;
    int start_y=0;
    int end_x=0;
    int end_y=0;

    int x1=0,y1=0,x2=0,y2=0;

    quint8 rectFlag=0;
    quint8 dragFlag=0;

    QPixmap fullPixmap;         //原始全屏图片
    QPixmap originalPixmap;

    QWidget *toolbar;           //工具条
    QPushButton *btn_saveFile;
    QPushButton *btn_saveClipboard;
    QPushButton *btn_cancel;

    QClipboard *clipboard;

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *e);
};

#endif // CANVAS_H
