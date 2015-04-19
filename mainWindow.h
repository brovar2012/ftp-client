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
#include "firstwindow.h"

class mainWindow:public QDialog
{
    Q_OBJECT
public:
    mainWindow(QWidget *parent=0);
    QSize sizeHint();
private:
    // - для главного окна
    QMenuBar *menu;
    QMenu *fileMenu;
    QLineEdit *ftpAdress,*computerAdress;
    QFileSystemModel *modelOfComp;
    QTreeView *tableOfComp;
    QLabel *choiseOfCurrentPath;
    QPushButton *buttonLoad;
    QDialogButtonBox *buttonBox1;
    QString adressOfComp;
    // - для окна FTP
    QLabel *ftpServerLabel,*currentPathLabel;
    QLineEdit *ftpServerLineEdit;
    QLabel *statusLabel;
    QTreeWidget *fileList;
    QPushButton *cdToParentButton;
    QPushButton *connectButton;
    QPushButton *downloadButton;
    QPushButton *deleteButton;
    QDialogButtonBox *buttonBox;
    QProgressDialog *progressDialog;
    QHash<QString, bool> isDirectory;
    QString currentPath;
    QFtp *ftp;
    QFile *file;
public slots:
    // для компа
    void callConnectionWindow();
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
    void adressOfCompIsChange();
    void removeFile ();
    void loadFile();
};

#endif // MAINWINDOW_H
