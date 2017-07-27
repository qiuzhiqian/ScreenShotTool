#ifndef HOTKEYBAR_H
#define HOTKEYBAR_H

#include <QWidget>
#include <QLineEdit>
#include <QKeyEvent>

#include "KeyString.h"

/*
 * Author:qiuzhiqian
 * Email:xia_mengliang@163.com
 * Github:https://github.com/qiuzhiqian
 * Date:2017.07.23
 **/

class HotKeyBar : public QLineEdit
{
    Q_OBJECT
public:
    HotKeyBar(QWidget *parent = 0);
    void setHotKey(Qt::Key t_key, Qt::KeyboardModifiers t_mod);
    void setShowText(Qt::Key t_key, Qt::KeyboardModifiers t_mod);
    void setKeyString(KeyString *t_keystring);

private:
    void focusInEvent(QFocusEvent *e);          //获取焦点
    void focusOutEvent(QFocusEvent *e);         //失去焦点
    void keyPressEvent(QKeyEvent *event);

    Qt::KeyboardModifiers keymod;
    Qt::Key keyval;

    KeyString *keystring;

signals:
    void sgn_hotKeyChanged(Qt::Key t_key,Qt::KeyboardModifiers t_mod);
};

#endif // HOTKEYBAR_H
