#ifndef MYWINDOW_H
#define FIRSTWINDOW_H


#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QCheckBox>


class firstWindow:public QDialog
{
     Q_OBJECT

public:
    firstWindow(QWidget *parent=0);
private:
    QLabel *serverAdress,*port,*password;
    QLineEdit *serverAdressLine,*portLine,*passwordLine;
    QPushButton *ok,*exit;

};
#endif
