#include "hotkeybar.h"

/*
 * Author:qiuzhiqian
 * Email:xia_mengliang@163.com
 * Github:https://github.com/qiuzhiqian
 * Date:2017.07.23
 * Description:这个类主要用来创建一个添加了一些自定义功能的LineEdit，这个LineEdit用来设置和显示当前的截图快捷键
 **/

HotKeyBar::HotKeyBar(QWidget *parent) :
    QLineEdit(parent)
{
    keymod=Qt::ControlModifier;
    keyval=Qt::Key_F1;
    this->setFocusPolicy(Qt::FocusPolicy(Qt::TabFocus|Qt::ClickFocus));
    this->setStyleSheet("QLineEdit{border:2px solid rgb(240, 240, 240);}");
}

void HotKeyBar::setHotKey(Qt::Key t_key, Qt::KeyboardModifiers t_mod)
{
    keyval=t_key;
    keymod=t_mod;
    setShowText(t_key,t_mod);
}

void HotKeyBar::setKeyString(KeyString *t_keystring)
{
    keystring=t_keystring;
}

void HotKeyBar::setShowText(Qt::Key t_key, Qt::KeyboardModifiers t_mod)
{
    QString showtext=keystring->Key2String(t_key,t_mod);
    setText(showtext);
}

void HotKeyBar::focusInEvent(QFocusEvent *e)
{
    QLineEdit::focusInEvent(e);
    this->setStyleSheet("QLineEdit{border:2px solid rgb(163, 219, 144);}");
}

void HotKeyBar::focusOutEvent(QFocusEvent *e)
{
    QLineEdit::focusOutEvent(e);
    this->setStyleSheet("QLineEdit{border:2px solid rgb(240, 240, 240);}");
}

void HotKeyBar::keyPressEvent(QKeyEvent *event)
{
    keymod=event->modifiers();
    keyval=Qt::Key(event->key());

    sgn_hotKeyChanged(keyval,keymod);

    setShowText(keyval,keymod);


}
