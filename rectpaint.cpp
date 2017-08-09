#include "rectpaint.h"

RectPaint::RectPaint()
{

}

RectPaint::RectPaint(const QPointF &topLeft, const QSizeF &size)
    :QRectF(topLeft,size)
{
}
RectPaint::RectPaint(const QPointF &topLeft, const QPointF &bottomRight)
    :QRectF(topLeft,bottomRight)
{
}

RectPaint::RectPaint(qreal x, qreal y, qreal width, qreal height)
    :QRectF(x,y,width,height)
{
}

RectPaint::RectPaint(const QRect &rectangle)
    :QRectF(rectangle)
{

}

void RectPaint::setPen(QPen pen)
{
    drawPen=pen;
}

QPen RectPaint::getPen()
{
    return drawPen;
}
