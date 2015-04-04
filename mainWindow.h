#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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

class mainWindow:public QDialog
{
    Q_OBJECT
public:
    mainWindow(QWidget *parent=0);
private:
    QMenuBar *menu;
    QMenu *file;
    QMenu *help;
    QLineEdit *ftp,*computer;
    QFileSystemModel *modelOfFTP;
    QFileSystemModel *modelOfComp;
    QTreeView *tableOfFTP;
    QTreeView *tableOfComp;
private slots:
    void see();
};

#endif // MAINWINDOW_H
