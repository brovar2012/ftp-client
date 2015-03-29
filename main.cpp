#include "mainwindow.h"
#include <QApplication>
#include "firstwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    firstWindow *w = new firstWindow(0);
    w->show();

    return a.exec();
}
