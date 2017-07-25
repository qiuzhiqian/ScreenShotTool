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

private:
    void keyPressEvent(QKeyEvent *event);

    int keymod;
    int keyval;
};

#endif // SHOTCUT_H
