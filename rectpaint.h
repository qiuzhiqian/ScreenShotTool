#ifndef RECTPAINT_H
#define RECTPAINT_H

#include <QWidget>
#include <QRectF>
#include <QPen>
#include <QPointF>


class RectPaint : public QRectF
{
public:
    RectPaint();
    RectPaint(const QPointF &topLeft, const QSizeF &size);
    RectPaint(const QPointF &topLeft, const QPointF &bottomRight);
    RectPaint(qreal x, qreal y, qreal width, qreal height);
    RectPaint(const QRect &rectangle);


    void setPen(QPen pen);
    QPen getPen();

private:
    QPen drawPen;
};

#endif // RECTPAINT_H
