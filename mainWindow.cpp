#include "mainwindow.h"

mainWindow::mainWindow(QWidget *parent): QDialog(parent)
{
    modelOfComp = new QFileSystemModel;
    modelOfComp = new QFileSystemModel;
    modelOfFTP = new QFileSystemModel;
    modelOfFTP = new QFileSystemModel;
    modelOfComp->setRootPath("D://JAVARUSH/");
    modelOfFTP->setRootPath ("ftp://127.0.0.1/");

    tableOfComp = new QTreeView;
    tableOfComp->setModel(modelOfComp);
    tableOfComp->setRootIndex(modelOfComp->index("D://JAVARUSH/"));

    tableOfFTP = new QTreeView;
    tableOfFTP->setModel(modelOfFTP);
    tableOfFTP->setRootIndex(modelOfFTP->index("ftp://127.0.0.1/"));

    ftp = new QLineEdit;
    computer = new QLineEdit;

    menu = new QMenuBar;
    //menu->addMenu("Файл");
    //menu->addMenu("Справка");

    file = new QMenu("Файл");
    file->addAction("Подключить",this, SLOT(see()));
    file->addAction("2");

    help = new QMenu("Справка");
    help->addAction("1");
    help->addAction("2");
    help->addSeparator();

    menu->addMenu(file);
    menu->addMenu(help);

    QHBoxLayout *high = new QHBoxLayout;
    high->addWidget(menu);

    QVBoxLayout *ftpLayout = new QVBoxLayout;
    ftpLayout->addWidget(ftp);
    ftpLayout->addWidget(tableOfFTP);

    QVBoxLayout *compLayout = new QVBoxLayout;
    compLayout->addWidget(computer);
    compLayout->addWidget(tableOfComp);

    QHBoxLayout *midle = new QHBoxLayout;
    midle->addLayout(ftpLayout);
    midle->addLayout(compLayout);

    QVBoxLayout *all = new QVBoxLayout;
    all->addLayout(high);
    all->addLayout(midle);

    setLayout(all);
    setWindowTitle("Bondar's && Brovar's ftp =)");
}

void mainWindow::see()
{
    qDebug("Тут будет типо подключение");
}

