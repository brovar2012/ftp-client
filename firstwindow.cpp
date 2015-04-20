#include "firstwindow.h"

firstWindow::firstWindow(QWidget *parent): QDialog(parent)
{
    serverAdress = new QLabel("Адрес сервера:");
    serverAdressLine = new QLineEdit;
    serverAdressLine->setText("127.0.0.1");

    password = new QLabel ("Пароль:");
    passwordLine = new QLineEdit;

    login = new QLabel("Логин:");
    loginLine = new QLineEdit;

    ok = new QPushButton("Подключить");
    exit = new QPushButton("Отмена подключения");

    box = new QCheckBox();
    anonymus = new QLabel("Анонимный вход");

    box->setChecked(true);
    passwordLine->setEnabled(false);
    loginLine->setEnabled(false);

    QVBoxLayout *server = new QVBoxLayout;
    server->addWidget(serverAdress);
    server->addWidget(serverAdressLine);

    QVBoxLayout *passwordLayout = new QVBoxLayout;
    passwordLayout->addWidget(login);
    passwordLayout->addWidget(loginLine);
    passwordLayout->addWidget(password);
    passwordLayout->addWidget(passwordLine);

    QHBoxLayout *button = new QHBoxLayout;
    button->addWidget(ok);
    button->addWidget(exit);

    QHBoxLayout *login1 = new QHBoxLayout;
    login1->addWidget(box);
    login1->addWidget(anonymus);

    QVBoxLayout *all = new QVBoxLayout;
    all->addLayout(server);
    all->addLayout(passwordLayout);
    all->addLayout(login1);
    all->addLayout(button);

    connect(exit, SIGNAL(clicked()), this, SLOT(close()));
    connect (ok,SIGNAL(clicked()), this, SLOT(connectToServer()));
    connect (box,SIGNAL(clicked()),this,SLOT (anonymusHasChanged()));
    setLayout(all);
    setWindowTitle("Подключение к серверу");
}

void firstWindow::connectToServer()
{
    if (serverAdressLine->text().isEmpty())
    {
        QMessageBox::information(this, tr("Ошибка"),
                                 tr("Строка адреса пуста."));
        return;
    }
    else
        closeOrNot = true;
        close();
}

void firstWindow :: anonymusHasChanged()
{
   if (box->isChecked())
   {
       passwordLine->setEnabled(false);
       loginLine->setEnabled(false);
   }
    else
   {
       passwordLine->setEnabled(true);
       loginLine->setEnabled(true);
   }
}

QString firstWindow::getAdress()
{
    return serverAdressLine->text();
}

QString firstWindow::getLogin()
{
    return loginLine->text();
}

QString firstWindow::getPassword()
{
    return passwordLine->text();
}

bool firstWindow::getAnonymus()
{
    if (box->isChecked())
        return true;
    else
        return false;
}

bool firstWindow::closeWindow()
{
    return closeOrNot;
}

