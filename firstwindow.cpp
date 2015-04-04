#include "firstwindow.h"

firstWindow::firstWindow(QWidget *parent): QDialog(parent)
{
    serverAdress = new QLabel("Адрес сервера:");
    port = new QLabel ("Порт:");
    password = new QLabel ("Пароль администратора:");
    ok = new QPushButton("Подкючить");
    exit = new QPushButton("Выход");
    serverAdressLine = new QLineEdit;
    portLine = new QLineEdit;
    passwordLine = new QLineEdit;
    model = new QFileSystemModel;
    table = new QTreeView;
    table->setModel(model);
    table->setRootIndex(model->index("D://JAVARUSH/"));


    QVBoxLayout *server = new QVBoxLayout;
    server->addWidget(serverAdress);
    server->addWidget(serverAdressLine);

    QVBoxLayout *portLayout = new QVBoxLayout;
    portLayout->addWidget(port);
    portLayout->addWidget(portLine);

    QVBoxLayout *passwordLayout = new QVBoxLayout;
    passwordLayout->addWidget(password);
    passwordLayout->addWidget(passwordLine);

    QHBoxLayout *button = new QHBoxLayout;
    button->addWidget(ok);
    button->addWidget(exit);

    QHBoxLayout *high = new QHBoxLayout;
    high->addLayout(server);
    high->addLayout(portLayout);
    high->addWidget(table);

    QVBoxLayout *all = new QVBoxLayout;
    all->addLayout(high);
    all->addLayout(passwordLayout);
    all->addLayout(button);

    connect(exit, SIGNAL(clicked()), this, SLOT(close()));
    connect (ok,SIGNAL(clicked()), this, SLOT(openPaPka()));
    setLayout(all);
    setWindowTitle("Подключение к серверу");
}

void firstWindow::openPaPka()
{
        QString dir("D://JAVARUSH/");
        model->setRootPath(dir);
}
