#include "KeyString.h"

/*
 * Author:qiuzhiqian
 * Email:xia_mengliang@163.com
 * Github:https://github.com/qiuzhiqian
 * Date:2017.07.23
 * Description:这个类主要用来转换快捷按键的键值和字符串
 **/

KeyString::KeyString()
{
    keymap[Qt::Key_Escape]="Esc";
    keymap[Qt::Key_Tab]="Tab";
    keymap[Qt::Key_Backtab]="Backtab";
    keymap[Qt::Key_Backspace]="Backspace";
    keymap[Qt::Key_Return]="Return";
    keymap[Qt::Key_Insert]="Insert";
    keymap[Qt::Key_Delete]="Delete";
    keymap[Qt::Key_Pause]="Pause";
    keymap[Qt::Key_Home]="Home";
    keymap[Qt::Key_End]="End";
    keymap[Qt::Key_Left]="Left";
    keymap[Qt::Key_Up]="Up";
    keymap[Qt::Key_Right]="Right";
    keymap[Qt::Key_Down]="Down";
    keymap[Qt::Key_PageUp]="PageUp";
    keymap[Qt::Key_PageDown]="PageDown";
    keymap[Qt::Key_F1]="F1";
    keymap[Qt::Key_F2]="F2";
    keymap[Qt::Key_F3]="F3";
    keymap[Qt::Key_F4]="F4";
    keymap[Qt::Key_F5]="F5";
    keymap[Qt::Key_F6]="F6";
    keymap[Qt::Key_F7]="F7";
    keymap[Qt::Key_F8]="F8";
    keymap[Qt::Key_F9]="F9";
    keymap[Qt::Key_F10]="F10";
    keymap[Qt::Key_F11]="F11";
    keymap[Qt::Key_F12]="F12";
    keymap[Qt::Key_Space]="Space";
    keymap[Qt::Key_0]="0";
    keymap[Qt::Key_1]="1";
    keymap[Qt::Key_2]="2";
    keymap[Qt::Key_3]="3";
    keymap[Qt::Key_4]="4";
    keymap[Qt::Key_5]="5";
    keymap[Qt::Key_6]="6";
    keymap[Qt::Key_7]="7";
    keymap[Qt::Key_8]="8";
    keymap[Qt::Key_9]="9";
    keymap[Qt::Key_A]="A";
    keymap[Qt::Key_B]="B";
    keymap[Qt::Key_C]="C";
    keymap[Qt::Key_D]="D";
    keymap[Qt::Key_E]="E";
    keymap[Qt::Key_F]="F";
    keymap[Qt::Key_G]="G";
    keymap[Qt::Key_H]="F";
    keymap[Qt::Key_I]="I";
    keymap[Qt::Key_J]="J";
    keymap[Qt::Key_K]="K";
    keymap[Qt::Key_L]="L";
    keymap[Qt::Key_M]="M";
    keymap[Qt::Key_N]="N";
    keymap[Qt::Key_O]="O";
    keymap[Qt::Key_P]="P";
    keymap[Qt::Key_Q]="Q";
    keymap[Qt::Key_R]="R";
    keymap[Qt::Key_S]="S";
    keymap[Qt::Key_T]="T";
    keymap[Qt::Key_U]="U";
    keymap[Qt::Key_V]="V";
    keymap[Qt::Key_W]="W";
    keymap[Qt::Key_X]="X";
    keymap[Qt::Key_Y]="Y";
    keymap[Qt::Key_Z]="Z";
}

QString KeyString::Key2String(Qt::Key t_key, Qt::KeyboardModifiers t_mod)
{
    QString showtext="";
    QString modstr="";
    QString keystr="";
    switch(t_mod)
    {
    case Qt::NoModifier:
        modstr="";
        break;
    case Qt::ShiftModifier:
        modstr="SHIFT";
        break;
    case Qt::ControlModifier:
        modstr="CTRL";
        break;
    case Qt::AltModifier:
        modstr="ATL";
        break;
    default:
        modstr="";
        break;
    }

    keystr=keymap[t_key];
    
    if(modstr!="")
    {
        showtext=modstr+"+"+keystr;
    }
    else
    {
        showtext=keystr;
    }
    
    return showtext;
}
    
void KeyString::String2Key(QString str,Qt::Key &t_key, Qt::KeyboardModifiers &t_mod)
{
    QString modstr="";
    QString keystr="";
        
    QStringList keyList=str.split('+');
    
    if(keyList.size()==2)
    {
        modstr=keyList[0];
        keystr=keyList[1];
    }
    else
    {
        keystr=keyList[0];
    }
    
    if(modstr=="")
    {
        t_mod=Qt::NoModifier;
    }
    else if(modstr=="SHIFT")
    {
        t_mod=Qt::ShiftModifier;
    }
    else if(modstr=="CTRL")
    {
        t_mod=Qt::ControlModifier;
    }
    else if(modstr=="ATL")
    {
        t_mod=Qt::AltModifier;
    }
    
    t_key=Qt::Key(keymap.keys(keystr)[0]);
}
