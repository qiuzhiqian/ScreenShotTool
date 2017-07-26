#ifndef MYWINEVENTFILTER_H
#define MYWINEVENTFILTER_H

#include <QWidget>
#include <QAbstractNativeEventFilter>
class MyWinEventFilter : public QAbstractNativeEventFilter
{
public:
    explicit MyWinEventFilter();
    ~MyWinEventFilter();
    virtual bool nativeEventFilter(const QByteArray &eventType, void *message, long *result);
};

#endif//MYWINEVENTFILTER_H
