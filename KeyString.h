#ifndef KEYSTRING_H
#define KEYSTRING_H

#include <QWidget>
#include <QMap>

/*
 * Author:qiuzhiqian
 * Email:xia_mengliang@163.com
 * Github:https://github.com/qiuzhiqian
 * Date:2017.07.23
 **/

class KeyString{
public:
    KeyString();
    QString Key2String(Qt::Key t_key, Qt::KeyboardModifiers t_mod);
    void String2Key(QString str,Qt::Key &t_key, Qt::KeyboardModifiers &t_mod);

private:
    QMap<Qt::Key, QString> keymap;
};

#endif
