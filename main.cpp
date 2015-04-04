#include <QApplication>
#include "firstwindow.h"
#include  "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    firstWindow *w = new firstWindow(0);
    w->show();
    mainWindow *ww= new mainWindow(0);
    ww->resize(850,400);
    ww->show();

    return a.exec();
}
