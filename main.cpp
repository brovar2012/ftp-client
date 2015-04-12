#include <QApplication>
#include "firstwindow.h"
#include  "mainwindow.h"
#include "qftp.h"
#include "qurlinfo.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //firstWindow *w = new firstWindow(0);
    //w->show();
    mainWindow *window= new mainWindow(0);
    window->resize(850,400);
    window->show();

    return a.exec();
}
