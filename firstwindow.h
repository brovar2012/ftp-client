#ifndef FIRSTWINDOW_H
#define FIRSTWINDOW_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QCheckBox>
#include <QFileSystemModel>
#include <QTreeView>
#include <QMenuBar>
#include <QMessageBox>

#include "qftp.h"
#include "qurlinfo.h"

class firstWindow:public QDialog
{
     Q_OBJECT
public:
    firstWindow(QWidget *parent=0);
    QString getAdress();
    QString getLogin();
    QString getPassword();
    bool getAnonymus();
    bool closeWindow();
private:
    QLabel *serverAdress,*password,*login;
    QLineEdit *serverAdressLine,*passwordLine,*loginLine;
    QPushButton *ok,*exit;
    QLabel *anonymus;
    QCheckBox *box;
    bool closeOrNot;
public slots:
    void connectToServer();
    void anonymusHasChanged();
};
#endif
