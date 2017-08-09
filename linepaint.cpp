#include "linepaint.h"
#include <QDebug>

LinePaint::LinePaint()
{

}

LinePaint::LinePaint(const QPoint &p1, const QPoint &p2)
    :QLine(p1,p2)
{
}

LinePaint::LinePaint(int x1, int y1, int x2, int y2)
    :QLine(x1,y1,x2,y2)
{
}

void LinePaint::setPen(QPen pen)
{
    drawPen=pen;
//    qDebug()<<drawPen.color().red()<<drawPen.color().green()<<drawPen.color().blue();
//    qDebug()<<drawPen.width();
//    qDebug()<<drawPen.style();
}

QPen LinePaint::getPen()
{
    return drawPen;
}
