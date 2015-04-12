#include "mainwindow.h"

mainWindow::mainWindow(QWidget *parent): QDialog(parent),ftp(0)
{
    // для главного окна
    modelOfComp = new QFileSystemModel;
    modelOfComp = new QFileSystemModel;

    QString adressOfComp = "C:/Qt/Qt5.3.2/Tools/QtCreator/bin/build-untitled1-Desktop_Qt_5_3_MSVC2010_OpenGL_32bit-Debug";
    modelOfComp->setRootPath(adressOfComp);

    tableOfComp = new QTreeView;
    tableOfComp->setModel(modelOfComp);
    tableOfComp->setRootIndex(modelOfComp->index(adressOfComp));

    ftpAdress = new QLineEdit;
    computerAdress = new QLineEdit;
    computerAdress->setText(adressOfComp);

    menu = new QMenuBar;
    //menu->addMenu("Файл");
    //menu->addMenu("Справка");

    fileMenu = new QMenu("Файл");
    fileMenu->addAction("Подключить",this, SLOT(see()));
    fileMenu->addAction("2");

    help = new QMenu("Справка");
    help->addAction("1");
    help->addAction("2");
    help->addSeparator();

    menu->addMenu(fileMenu);
    menu->addMenu(help);

    //------------------------------------------

    ftpServerLabel = new QLabel(tr("Ftp &server:"));
    ftpServerLineEdit = new QLineEdit("127.0.0.1");
    ftpServerLabel->setBuddy(ftpServerLineEdit);

    statusLabel = new QLabel(tr("Please enter the name of an FTP server."));

    fileList = new QTreeWidget;
    fileList->setEnabled(false);
    fileList->setRootIsDecorated(false);
    fileList->setHeaderLabels(QStringList() << tr("Name") << tr("Size") << tr("Owner") << tr("Group") << tr("Time"));
    fileList->header()->setStretchLastSection(false);

    connectButton = new QPushButton(tr("Connect"));
    connectButton->setDefault(true);

    cdToParentButton = new QPushButton;
    cdToParentButton->setIcon(QPixmap("cdtoparent.png"));
    cdToParentButton->setEnabled(false);

    downloadButton = new QPushButton(tr("Download"));
    downloadButton->setEnabled(false);

    quitButton = new QPushButton(tr("Quit"));

    buttonBox = new QDialogButtonBox;
    buttonBox->addButton(downloadButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(quitButton, QDialogButtonBox::RejectRole);

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
    mainLayout->addWidget(buttonBox);
    //_______________________________________________
    QHBoxLayout *high = new QHBoxLayout;
    high->addWidget(menu);

    QVBoxLayout *ftpLayout = new QVBoxLayout;
    ftpLayout->addLayout(mainLayout);

    QVBoxLayout *compLayout = new QVBoxLayout;
    compLayout->addWidget(computerAdress);
    compLayout->addWidget(tableOfComp);

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
    connect(quitButton, SIGNAL(clicked()), this, SLOT(close()));

    setLayout(all);
    setWindowTitle(tr("Bondar's && Brovar's ftp =)"));
}

void mainWindow::see()
{
    qDebug("Тут будет типо подключение");
}

QSize mainWindow::sizeHint()
{
    return QSize(450, 400);
}

void mainWindow::connectOrDisconnect()
{
    if (ftp) {
        ftp->abort();
        ftp->deleteLater();
        ftp = 0;
        fileList->setEnabled(false);
        cdToParentButton->setEnabled(false);
        downloadButton->setEnabled(false);
        connectButton->setEnabled(true);
        connectButton->setText(tr("Connect"));
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
        ftp->login();
    } else {
        ftp->connectToHost(url.host(), url.port(21));

        if (!url.userName().isEmpty())
            ftp->login(QUrl::fromPercentEncoding(url.userName().toLatin1()), url.password());
        else
            ftp->login();
        if (!url.path().isEmpty())
            ftp->cd(url.path());
    }

    fileList->setEnabled(true);
    connectButton->setEnabled(false);
    connectButton->setText(tr("Disconnect"));
    statusLabel->setText(tr("Connecting to FTP server %1...")
                         .arg(ftpServerLineEdit->text()));
}

void mainWindow::downloadFile()
{
    QString fileName = fileList->currentItem()->text(0);

    if (QFile::exists(fileName)) {
        QMessageBox::information(this, tr("FTP"),
                                 tr("There already exists a file called %1 in "
                                    "the current directory.")
                                 .arg(fileName));
        return;
    }

    file = new QFile(fileName);
    if (!file->open(QIODevice::WriteOnly)) {
        QMessageBox::information(this, tr("FTP"),
                                 tr("Unable to save the file %1: %2.")
                                 .arg(fileName).arg(file->errorString()));
        delete file;
        return;
    }

    ftp->get(fileList->currentItem()->text(0), file);

    progressDialog->setLabelText(tr("Downloading %1...").arg(fileName));
    downloadButton->setEnabled(false);
    progressDialog->exec();
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
                                     tr("Unable to connect to the FTP server "
                                        "at %1. Please check that the host "
                                        "name is correct.")
                                     .arg(ftpServerLineEdit->text()));
            connectOrDisconnect();
            return;
        }

        statusLabel->setText(tr("Logged onto %1.")
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
            statusLabel->setText(tr("Canceled download of %1.")
                                 .arg(file->fileName()));
            file->close();
            file->remove();
        } else {
            statusLabel->setText(tr("Downloaded %1 to current directory.")
                                 .arg(file->fileName()));
            file->close();
        }
        delete file;
        enableDownloadButton();
        progressDialog->hide();
    } else if (ftp->currentCommand() == QFtp::List) {
        if (isDirectory.isEmpty()) {
            fileList->addTopLevelItem(new QTreeWidgetItem(QStringList() << tr("<empty>")));
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
