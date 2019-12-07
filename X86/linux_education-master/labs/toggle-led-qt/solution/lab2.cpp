/****************************************************************
**
** Lab 2
**
** My references in creating this (starting with most recent):
** http://doc.trolltech.com/4.6/qabstractbutton.html#signals
** http://doc.qt.nokia.com/4.6/widgets-groupbox.html
** http://lists.trolltech.com/qt-interest/1997-06/thread00071-0.html
** http://doc.trolltech.com/4.7/moc.html
** http://lists.trolltech.com/qt-interest/2007-06/thread01171-0.html
** http://cartan.cas.suffolk.edu/oopdocbook/html/
** http://doc.trolltech.com/4.3/signalsandslots.html
** http://doc.trolltech.com/4.3/tutorial-t2.html
** http://doc.trolltech.com/3.0/toggleaction-example.html
** http://sector.ynet.sk/qt4-tutorial/my-first-qt-gui-application.html
** http://doc.trolltech.com/4.3/tutorial-t1.html
** http://www.gidforums.com/t-13690.html
** http://doc.trolltech.com/3.0/t1.html
**
****************************************************************/

#include <qapplication.h>
#include <qpushbutton.h>
#include "lab2.h"

void myQtObj::doSomething(bool checked)
{
    const char * fileName = "/sys/class/leds/beagleboard::usr0/trigger";
    FILE * fileTrigger = NULL;

    if(!fileTrigger)
    {
        fileTrigger = fopen(fileName, "w");
    }

    if(!fileTrigger)
    {
        printf("ERROR: unable to open %s", fileName);
        exit(-1);
    }

    if(checked)
    {
        fprintf(fileTrigger, "none");
        fflush(fileTrigger);
        printf("trigger: none\n");
        fflush(stdout);
    }
    else
    {
        fprintf(fileTrigger, "heartbeat");
        fflush(fileTrigger);
        printf("trigger: heartbeat\n");
        fflush(stdout);
    }
}

int main(int argc, char **argv)
{
    QApplication a(argc, argv);

    QPushButton b("Push me!", 0);
    b.setCheckable(true);
    b.setChecked(false);
    b.resize(100, 30);

    myQtObj c;

    QObject::connect(&b, SIGNAL(toggled(bool)), &c, SLOT(doSomething(bool)));

    b.show();
    return a.exec();
}



