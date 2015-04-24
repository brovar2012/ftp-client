#include "mainwindow.h"

mainWindow::mainWindow(QWidget *parent): QDialog(parent),ftp(0)
{
    // для главного окна
    modelOfComp = new QFileSystemModel;
    modelOfComp->setRootPath(currentPath);
    QString adressOfComp;
    tableOfComp = new QTreeView;
    tableOfComp->setModel(modelOfComp);
    tableOfComp->setRootIndex(modelOfComp->index(adressOfComp));


    currentPathLabel = new QLabel(tr("Текущая папка:"));
    choiseOfCurrentPath = new QLabel("Выберите папку для сохранения файлов или загрузки.");
    buttonLoad = new QPushButton("Загрузить файл");
    buttonLoad->setEnabled(false);
    buttonLoad->setEnabled(false);
    buttonLoad->setMaximumSize(400,23);
    ftpAdress = new QLineEdit;
    computerAdress = new QLineEdit;
    computerAdress->setText(adressOfComp);
    computerAdress->setEnabled(false);

    menu = new QMenuBar;

    fileMenu = new QMenu("Файл");
    fileMenu->addAction("Подключить",this, SLOT(callConnectionWindow()));
    fileMenu->addAction("Выход",this,SLOT(close()));

    menu->addMenu(fileMenu);

    //------------------------------------------

    ftpServerLabel = new QLabel(tr("Ftp &сервер:"));
    ftpServerLineEdit = new QLineEdit("127.0.0.1");
    ftpServerLineEdit->setEnabled(false);
    ftpServerLabel->setBuddy(ftpServerLineEdit);

    statusLabel = new QLabel(tr("Для подключения выберите Файл - Подключить."));

    fileList = new QTreeWidget;
    fileList->setEnabled(false);
    fileList->setRootIsDecorated(false);
    fileList->setHeaderLabels(QStringList() << tr("Name") << tr("Size") << tr("Owner") << tr("Group") << tr("Time"));
  // fileList->header()->setStretchLastSection(false);

    connectButton = new QPushButton(tr("Подключить"));
    connectButton->setEnabled(false);
    connectButton->setDefault(true);

    cdToParentButton = new QPushButton;
    cdToParentButton->setIcon(QPixmap("cdtoparent.png"));
    cdToParentButton->setEnabled(false);

    downloadButton = new QPushButton(tr("Скачать"));
    downloadButton->setEnabled(false);

    deleteButton = new QPushButton(tr("Удалить"));
    deleteButton->setEnabled(false);

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(downloadButton);
    buttonLayout->addWidget(deleteButton);

    progressDialog = new QProgressDialog(this);

    QHBoxLayout *topLayout = new QHBoxLayout;
    topLayout->addWidget(ftpServerLabel);
    topLayout->addWidget(ftpServerLineEdit);
    topLayout->addWidget(cdToParentButton);
    topLayout->addWidget(connectButton);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(topLayout);
    mainLayout->addWidget(fileList);
    mainLayout->addWidget(statusLabel);
    mainLayout->addLayout(buttonLayout);
    //_______________________________________________
    QHBoxLayout *high = new QHBoxLayout;
    high->addWidget(menu);

    QVBoxLayout *ftpLayout = new QVBoxLayout;
    ftpLayout->addLayout(mainLayout);

    QHBoxLayout *blabla = new QHBoxLayout;
    blabla->addWidget(currentPathLabel);
    blabla->addWidget(computerAdress);
    QVBoxLayout *compLayout = new QVBoxLayout;
    compLayout->addLayout(blabla);
    compLayout->addWidget(tableOfComp);
    compLayout->addWidget(choiseOfCurrentPath);
    compLayout->addWidget(buttonLoad);

    QHBoxLayout *midle = new QHBoxLayout;
    midle->addLayout(ftpLayout);
    midle->addLayout(compLayout);

    QVBoxLayout *all = new QVBoxLayout;
    all->addLayout(high);
    all->addLayout(midle);

    //для окна FTP

    connect(fileList, SIGNAL(itemActivated(QTreeWidgetItem *, int)),
            this, SLOT(processItem(QTreeWidgetItem *, int)));
    connect(fileList, SIGNAL(currentItemChanged(QTreeWidgetItem *, QTreeWidgetItem *)),
            this, SLOT(enableDownloadButton()));
    connect(progressDialog, SIGNAL(canceled()), this, SLOT(cancelDownload()));
    connect(connectButton, SIGNAL(clicked()), this, SLOT(connectOrDisconnect()));
    connect(cdToParentButton, SIGNAL(clicked()), this, SLOT(cdToParent()));
    connect(downloadButton, SIGNAL(clicked()), this, SLOT(downloadFile()));
    connect (tableOfComp,SIGNAL(clicked(QModelIndex)),this,SLOT(adressOfCompIsChange()));
    connect (deleteButton,SIGNAL(clicked()),this,SLOT(removeFile()));
    connect (buttonLoad,SIGNAL(clicked()),this,SLOT(loadFile()));

    setLayout(all);
    setWindowTitle(tr("Brovar's FTP-client"));
}

void mainWindow::adressOfCompIsChange()
{
    QModelIndexList lst = tableOfComp->selectionModel()->selectedIndexes();
    adressOfComp = modelOfComp->filePath(lst.at(0));
    computerAdress->setText(adressOfComp);
}

void mainWindow::callConnectionWindow()
{
    connectionWindow = new firstWindow;
    connectionWindow->show();
    connectButton->setEnabled(true);

}

QSize mainWindow::sizeHint()
{
    return QSize(450, 400);
}

void mainWindow::connectOrDisconnect()
{
    ftpServerLineEdit->setText(connectionWindow->getAdress());
    login = connectionWindow->getLogin();
    password = connectionWindow->getPassword();
    if (ftp)
    {
        ftp->abort();
        ftp->deleteLater();
        ftp = 0;
        fileList->setEnabled(false);
        cdToParentButton->setEnabled(false);
        downloadButton->setEnabled(false);
        connectButton->setEnabled(true);
        connectButton->setText(tr("Подключение"));
        setCursor(Qt::ArrowCursor);
        return;
    }
    setCursor(Qt::WaitCursor);

    ftp = new QFtp(this);
    connect(ftp, SIGNAL(commandFinished(int, bool)),
            this, SLOT(ftpCommandFinished(int, bool)));
    connect(ftp, SIGNAL(listInfo(const QUrlInfo &)),
            this, SLOT(addToList(const QUrlInfo &)));
    connect(ftp, SIGNAL(dataTransferProgress(qint64, qint64)),
            this, SLOT(updateDataTransferProgress(qint64, qint64)));

    fileList->clear();
    currentPath.clear();
    isDirectory.clear();

    QUrl url(ftpServerLineEdit->text());
    if (!url.isValid() || url.scheme().toLower() != QLatin1String("ftp")) {
        ftp->connectToHost(ftpServerLineEdit->text(), 21);
        if (!(anonymusOrNo = connectionWindow->getAnonymus()))
            ftp->login(login,password);
        else
            ftp->login();
    }
    else
    {
        ftp->connectToHost(url.host(), url.port(21));
        if (!(anonymusOrNo = connectionWindow->getAnonymus()))
            ftp->login(login,password);
        else
            ftp->login();
    }

    fileList->setEnabled(true);
    connectButton->setEnabled(false);
    deleteButton->setEnabled(true);
    buttonLoad->setEnabled(true);
    connectButton->setText(tr("Отключение"));
    statusLabel->setText(tr("Подключение к FTP серверу %1...")
                         .arg(ftpServerLineEdit->text()));
}

void mainWindow::downloadFile()
{
    QString fileName = fileList->currentItem()->text(0);
    QString fullFileName;
    if (adressOfComp.isEmpty())
        fullFileName = fileName;
    else
        fullFileName = adressOfComp+"/"+fileName;


    if (QFile::exists(fullFileName)) {
        QMessageBox::information(this, tr("FTP"),
                                 tr("В текущей дериктории уже существует файл с именем %1.")
                                 .arg(fileName));
        return;
    }
    file = new QFile(fullFileName);
    if (!file->open(QIODevice::WriteOnly)) {
        QMessageBox::information(this, tr("FTP"),
                                 tr("Невозможно сохранить файл %1: %2.")
                                 .arg(fullFileName).arg(file->errorString()));
        delete file;
        return;
    }

    ftp->get(fileList->currentItem()->text(0), file);
    progressDialog->setLabelText(tr("Скачивание %1...").arg(fileName));
    downloadButton->setEnabled(false);
    progressDialog->exec();

}

void mainWindow::loadFile()
{
    if (!anonymusOrNo)
    {
        QString fileName = adressOfComp;
        QFileInfo  fileInfo(fileName);
        QFile *upfile = new QFile(fileName);
        upfile->open(QIODevice::ReadOnly | QIODevice::Unbuffered);
        ftp->put(upfile, fileInfo.fileName());
        upfile->close();
        progressDialog->setLabelText(tr("Загрузка %1...").arg(fileInfo.fileName()));
        progressDialog->exec();
        fileList->clear();
        ftp->list();
    }
    else
        QMessageBox::information(this,"Ошибка!",AUTHORIZATION_ERROR);
}

void mainWindow::removeFile()
{
    if (!anonymusOrNo)
    {
        ftp->remove(fileList->currentItem()->text(0));
        fileList->clear();
        ftp->list();
    }
    else
        QMessageBox::information(this,"Ошибка!",AUTHORIZATION_ERROR);
}

void mainWindow::cancelDownload()
{
    ftp->abort();
}

void mainWindow::ftpCommandFinished(int, bool error)
{
    setCursor(Qt::ArrowCursor);

    if (ftp->currentCommand() == QFtp::ConnectToHost) {
        if (error) {
            QMessageBox::information(this, tr("FTP"),
                                     tr("Невозможно подключиться к"
                                        " %1.Проверьте имя хоста.")
                                     .arg(ftpServerLineEdit->text()));
            connectOrDisconnect();
            return;
        }

        statusLabel->setText(tr("Подключено к %1.")
                             .arg(ftpServerLineEdit->text()));
        fileList->setFocus();
        downloadButton->setDefault(true);
        connectButton->setEnabled(true);
        return;
    }

    if (ftp->currentCommand() == QFtp::Login)
        ftp->list();

    if (ftp->currentCommand() == QFtp::Get) {
        if (error) {
            statusLabel->setText(tr("Прервана загрузка файла %1.")
                                 .arg(file->fileName()));
            file->close();
            file->remove();
        } else {
            statusLabel->setText(tr("Файл %1 загружен в указанную папку.")
                                 .arg(file->fileName()));
            file->close();
        }
        delete file;
        enableDownloadButton();
        progressDialog->hide();
    } else if (ftp->currentCommand() == QFtp::List) {
        if (isDirectory.isEmpty()) {
            fileList->addTopLevelItem(new QTreeWidgetItem(QStringList() << tr("<пусто>")));
            fileList->setEnabled(false);
        }
    }
}

void mainWindow::addToList(const QUrlInfo &urlInfo)
{
    QTreeWidgetItem *item = new QTreeWidgetItem;
    item->setText(0, urlInfo.name());
    item->setText(1, QString::number(urlInfo.size()));
    item->setText(2, urlInfo.owner());
    item->setText(3, urlInfo.group());
    item->setText(4, urlInfo.lastModified().toString("MMM dd yyyy"));
    QPixmap pixmap(urlInfo.isDir() ? "dir.png" : "file.png");
    item->setIcon(0, pixmap);

    isDirectory[urlInfo.name()] = urlInfo.isDir();
    fileList->addTopLevelItem(item);
    if (!fileList->currentItem()) {
        fileList->setCurrentItem(fileList->topLevelItem(0));
        fileList->setEnabled(true);
    }
}

void mainWindow::processItem(QTreeWidgetItem *item, int /*column*/)
{
    QString name = item->text(0);
    if (isDirectory.value(name)) {
        fileList->clear();
        isDirectory.clear();
        currentPath += "/" + name;
        ftp->cd(name);
        ftp->list();
        cdToParentButton->setEnabled(true);
        setCursor(Qt::WaitCursor);
        return;
    }
}

void mainWindow::cdToParent()
{
    setCursor(Qt::WaitCursor);
    fileList->clear();
    isDirectory.clear();
    currentPath = currentPath.left(currentPath.lastIndexOf('/'));
    if (currentPath.isEmpty()) {
        cdToParentButton->setEnabled(false);
        ftp->cd("/");
    } else {
        ftp->cd(currentPath);
    }
    ftp->list();
}

void mainWindow::updateDataTransferProgress(qint64 readBytes,
                                           qint64 totalBytes)
{
    progressDialog->setMaximum(totalBytes);
    progressDialog->setValue(readBytes);
}

void mainWindow::enableDownloadButton()
{
    QTreeWidgetItem *current = fileList->currentItem();
    if (current) {
        QString currentFile = current->text(0);
        downloadButton->setEnabled(!isDirectory.value(currentFile));
    } else {
        downloadButton->setEnabled(false);
    }
}
