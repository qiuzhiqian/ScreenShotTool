#ifndef TEXTPAINT_H
#define TEXTPAINT_H

#include <QLineEdit>

class TextPaint : public QLineEdit
{
    Q_OBJECT
public:
    TextPaint(QWidget *parent = Q_NULLPTR);

private:
    void autoResize(void);

public slots:
    void slt_autoResize(void);
    void slt_editEnd(void);
};

#endif // TEXTPAINT_H
