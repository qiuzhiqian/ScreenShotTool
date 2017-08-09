#ifndef LINEPAINT_H
#define LINEPAINT_H

#include <QWidget>
#include <QLine>
#include <QPen>
#include <QPoint>

class LinePaint : public QLine
{
public:
    LinePaint();
    LinePaint(const QPoint &p1, const QPoint &p2);
    LinePaint(int x1, int y1, int x2, int y2);

    void setPen(QPen pen);
    QPen getPen();

private:
    QPen drawPen;
};

#endif // LINEPAINT_H
