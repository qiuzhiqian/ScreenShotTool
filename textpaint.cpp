#include "textpaint.h"
#include <QFontMetrics>

TextPaint::TextPaint(QWidget *parent):QLineEdit(parent)
{
    //setSizeIncrement();
    connect(this,SIGNAL(textChanged(QString)),this,SLOT(slt_autoResize()));
    connect(this,SIGNAL(editingFinished()),this,SLOT(slt_editEnd()));
}

void TextPaint::slt_autoResize(void)
{
    //QStaticText tempText(this->text());
    //this->setFixedWidth(tempText.size().width()+6);
    //this->setFixedHeight(tempText.size().height()+6);
    QFontMetrics fm(this->font());
    //this->setFixedWidth(fm.boundingRect(this->text()).width());
    this->setFixedWidth(fm.width(this->text())+6);
    this->setFixedHeight(fm.height()+2);
    //this->adjustSize();
}

void TextPaint::slt_editEnd(void)
{
    this->setDisabled(true);
    //this->setReadOnly(true);
    this->setStyleSheet("background:transparent;border-width:0;border-style:outset");
    //this->setStyleSheet("color:red");//文本颜色
}
