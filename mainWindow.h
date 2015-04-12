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
#include <QTreeWidget>
#include <QDialogButtonBox>
#include <QProgressDialog>
#include <QMessageBox>
#include <QtNetwork>

#include "qftp.h"
#include "qurlinfo.h"

class mainWindow:public QDialog
{
    Q_OBJECT
public:
    mainWindow(QWidget *parent=0);
    QSize sizeHint();
public:
    // - для главного окна
    QMenuBar *menu;
    QMenu *fileMenu;
    QMenu *help;
    QLineEdit *ftpAdress,*computerAdress;
    QFileSystemModel *modelOfComp;
    QTreeView *tableOfComp;
    // - для окна FTP
    QLabel *ftpServerLabel;
    QLineEdit *ftpServerLineEdit;
    QLabel *statusLabel;
    QTreeWidget *fileList;
    QPushButton *cdToParentButton;
    QPushButton *connectButton;
    QPushButton *downloadButton;
    QPushButton *quitButton;
    QDialogButtonBox *buttonBox;
    QProgressDialog *progressDialog;
    QHash<QString, bool> isDirectory;
    QString currentPath;
    QFtp *ftp;
    QFile *file;
public slots:
    // для компа
    void see();
    // для FTP
    void connectOrDisconnect();
    void downloadFile();
    void cancelDownload();

    void ftpCommandFinished(int commandId, bool error);
    void addToList(const QUrlInfo &urlInfo);
    void processItem(QTreeWidgetItem *item, int column);
    void cdToParent();
    void updateDataTransferProgress(qint64 readBytes,
                                    qint64 totalBytes);
    void enableDownloadButton();
};

#endif // MAINWINDOW_H
