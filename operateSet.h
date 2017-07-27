#ifndef OPERATESET_H
#define OPERATESET_H

#include <QVariant>
#include <QString>
#include <QSettings>

/*
 * Author:qiuzhiqian
 * Email:xia_mengliang@163.com
 * Github:https://github.com/qiuzhiqian
 * Date:2017.07.23
 * Description:这个类是由一些静态方法组成，主要完成对ini文件的读写
 **/

class OperateSet{
public:
    static void writeSetting(const QString &group, const QString &key, const QVariant &value)      //写ini文件
    {
        QSettings settings("./setting.ini", QSettings::IniFormat); // 当前目录的INI文件
        settings.beginGroup(group);
        settings.setValue(key,value);
    }
    
    static QVariant readSetting(const QString &group, const QString &key, const QVariant &defaultvalue)        //读ini文件
    {
        QVariant val;
        QString keypos=group+"/"+key;
        QSettings settings("./setting.ini", QSettings::IniFormat); // 当前目录的INI文件
        val=settings.value(keypos,defaultvalue);
        return val;
    }
};

#endif //OPERATESET_H
