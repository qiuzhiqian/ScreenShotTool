#include "shotcut.h"

ShotCut::ShotCut(QWidget *parent) :
    QLineEdit(parent)
{
    keymod=Qt::ControlModifier;
    keyval=Qt::Key_F1;
    setText("Ctrl+F1");
    this->setFocusPolicy(Qt::TabFocus|Qt::ClickFocus);
    this->setStyleSheet("QLineEdit{border:2px solid rgb(240, 240, 240);}");
}

void ShotCut::focusInEvent(QFocusEvent *e)
{
    QLineEdit::focusInEvent(e);
    this->setStyleSheet("QLineEdit{border:2px solid rgb(163, 219, 144);}");
}

void ShotCut::focusOutEvent(QFocusEvent *e)
{
    QLineEdit::focusOutEvent(e);
    this->setStyleSheet("QLineEdit{border:2px solid rgb(240, 240, 240);}");
}

void ShotCut::keyPressEvent(QKeyEvent *event)
{
    QString showtext="";
    //qDebug("modify=%x,key=%x",event->modifiers(),event->key());
    keymod=event->modifiers();
    keyval=Qt::Key(event->key());

    sgn_hotKeyChanged(keyval,keymod);

    switch(keymod)
    {
    case Qt::NoModifier:
        showtext="";
        break;
    case Qt::ShiftModifier:
        showtext="Shift+";
        break;
    case Qt::ControlModifier:
        showtext="Ctrl+";
        break;
    case Qt::AltModifier:
        showtext="Alt+";
        break;
    default:
        break;
    }

    switch(keyval)
    {
    case Qt::Key_Escape:
        showtext+="Esc";
        break;
    case Qt::Key_Tab:
        showtext+="Tab";
        break;
    case Qt::Key_Backtab:
        showtext+="Tab";
        break;
    case Qt::Key_Backspace:
        showtext+="Tab";
        break;
    case Qt::Key_Return:
        showtext+="Enter";
        break;
    case Qt::Key_Insert:
        showtext+="Inster";
        break;
    case Qt::Key_Delete:
        showtext+="Delete";
        break;
    case Qt::Key_Pause:
        showtext+="Pause";
        break;
    case Qt::Key_Home:
        showtext+="Home";
        break;
    case Qt::Key_End:
        showtext+="End";
        break;
    case Qt::Key_Left:
        showtext+="Left";
        break;
    case Qt::Key_Up:
        showtext+="Up";
        break;
    case Qt::Key_Right:
        showtext+="Right";
        break;
    case Qt::Key_Down:
        showtext+="Down";
        break;
    case Qt::Key_PageUp:
        showtext+="PageUp";
        break;
    case Qt::Key_PageDown:
        showtext+="PageDown";
        break;
    case Qt::Key_F1:
        showtext+="F1";
        break;
    case Qt::Key_F2:
        showtext+="F2";
        break;
    case Qt::Key_F3:
        showtext+="F3";
        break;
    case Qt::Key_F4:
        showtext+="F4";
        break;
    case Qt::Key_F5:
        showtext+="F5";
        break;
    case Qt::Key_F6:
        showtext+="F6";
        break;
    case Qt::Key_F7:
        showtext+="F7";
        break;
    case Qt::Key_F8:
        showtext+="F8";
        break;
    case Qt::Key_F9:
        showtext+="F9";
        break;
    case Qt::Key_F10:
        showtext+="10";
        break;
    case Qt::Key_F11:
        showtext+="F11";
        break;
    case Qt::Key_F12:
        showtext+="F12";
        break;
    case Qt::Key_Space:
        showtext+="Space";
        break;
    case Qt::Key_0:
        showtext+="0";
        break;
    case Qt::Key_1:
        showtext+="1";
        break;
    case Qt::Key_2:
        showtext+="2";
        break;
    case Qt::Key_3:
        showtext+="3";
        break;
    case Qt::Key_4:
        showtext+="4";
        break;
    case Qt::Key_5:
        showtext+="5";
        break;
    case Qt::Key_6:
        showtext+="6";
        break;
    case Qt::Key_7:
        showtext+="7";
        break;
    case Qt::Key_8:
        showtext+="8";
        break;
    case Qt::Key_9:
        showtext+="9";
        break;
    case Qt::Key_A:
        showtext+="A";
        break;
    case Qt::Key_B:
        showtext+="B";
        break;
    case Qt::Key_C:
        showtext+="C";
        break;
    case Qt::Key_D:
        showtext+="D";
        break;
    case Qt::Key_E:
        showtext+="E";
        break;
    case Qt::Key_F:
        showtext+="F";
        break;
    case Qt::Key_G:
        showtext+="G";
        break;
    case Qt::Key_H:
        showtext+="H";
        break;
    case Qt::Key_I:
        showtext+="I";
        break;
    case Qt::Key_J:
        showtext+="J";
        break;
    case Qt::Key_K:
        showtext+="K";
        break;
    case Qt::Key_L:
        showtext+="L";
        break;
    case Qt::Key_M:
        showtext+="M";
        break;
    case Qt::Key_N:
        showtext+="N";
        break;
    case Qt::Key_O:
        showtext+="O";
        break;
    case Qt::Key_P:
        showtext+="P";
        break;
    case Qt::Key_Q:
        showtext+="Q";
        break;
    case Qt::Key_R:
        showtext+="R";
        break;
    case Qt::Key_S:
        showtext+="S";
        break;
    case Qt::Key_T:
        showtext+="T";
        break;
    case Qt::Key_U:
        showtext+="U";
        break;
    case Qt::Key_V:
        showtext+="V";
        break;
    case Qt::Key_W:
        showtext+="W";
        break;
    case Qt::Key_X:
        showtext+="X";
        break;
    case Qt::Key_Y:
        showtext+="Y";
        break;
    case Qt::Key_Z:
        showtext+="Z";
        break;
    default:
        showtext+="";
    }

    setText(showtext);
}
