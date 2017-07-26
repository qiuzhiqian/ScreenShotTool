#ifndef SHOTCUT_H
#define SHOTCUT_H

#include <QWidget>
#include <QLineEdit>
#include <QKeyEvent>

class ShotCut : public QLineEdit
{
    Q_OBJECT
public:
    ShotCut(QWidget *parent = 0);
    void setShotCut(Qt::Key t_key,Qt::KeyboardModifiers t_mod);

private:
    void focusInEvent(QFocusEvent *e);          //获取焦点
    void focusOutEvent(QFocusEvent *e);         //失去焦点
    void keyPressEvent(QKeyEvent *event);

    Qt::KeyboardModifiers keymod;
    Qt::Key keyval;

signals:
    void sgn_hotKeyChanged(Qt::Key t_key,Qt::KeyboardModifiers t_mod);
};

#endif // SHOTCUT_H
