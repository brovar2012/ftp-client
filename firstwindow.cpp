@@ -1,42 +0,0 @@
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

    QVBoxLayout *all = new QVBoxLayout;
    all->addLayout(high);
    all->addLayout(passwordLayout);
    all->addLayout(button);

    connect(exit, SIGNAL(clicked()), this, SLOT(close()));
    setLayout(all);
    setWindowTitle("Подключение к серверу");
}
