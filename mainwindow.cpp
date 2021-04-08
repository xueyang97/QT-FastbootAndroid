#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QCloseEvent>
void MainWindow::closeEvent( QCloseEvent *event )
{
    event->accept();
}

QStringList searchFileName(const QString &folderPath, const QString &files)
{
    QDir dir(folderPath);
    QStringList fileNameLists = QStringList() << files;
    return dir.entryList(fileNameLists, QDir::Files|QDir::Readable, QDir::Name);
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    /* 禁止用户修改窗口大小 */
    setWindowFlags(windowFlags()& ~Qt::WindowMaximizeButtonHint);
    setFixedSize(this->width(), this->height());

    logFile = new QFile();

    http = new HttpServer(0);
    connect(http,SIGNAL(finished()),this,SLOT(on_httpFinished()));

    workUpdate = new WorkUpdate(0);
    connect(workUpdate,SIGNAL(readyReadStandardOutput()),this,SLOT(on_workReadyReadStandardOutput()));
    connect(workUpdate,SIGNAL(workingFinished()),this,SLOT(on_workingFinished()));

    adbDevices = new SearchDevice(0);
    connect(adbDevices,SIGNAL(searchFinished()),this,SLOT(on_searchFinished()));

    folderPath = QString();
    workMode   = 0;
    cacheFilename    = QStringList();
    abootFilename    = QStringList();
    bootFilename     = QStringList();
    persistFilename  = QStringList();
    recoveryFilename = QStringList();
    systemFilename   = QStringList();
    userdataFilename = QStringList();
    DDRFilename      = QStringList();
    splashFilename   = QStringList();
    // http->httpDownload("http://localhost/phpbin/upload", "123.jpg");
    // http->httpUpload("345.txt", "http://localhost/phpbin/upload");

    /* 新建日志文件夹 */
    QDir dir;
    if(!dir.exists("log")) {
        dir.mkdir("log");
    }

    // server = new ServerUpload(0);

}

MainWindow::~MainWindow()
{
    delete ui;
    delete logFile;
    delete http;
    delete workUpdate;
    delete adbDevices;
}

void MainWindow::on_workReadyReadStandardOutput(void)
{
    QString readAllStandardOutput = workUpdate->readAllStandardOutput();
    ui_editShowInfo(readAllStandardOutput);
    logFile->write(readAllStandardOutput.toUtf8());
}

void MainWindow::on_workingFinished(void)
{
    ui->comboBoxDevice->clear();
    ui->comboBoxDevice->addItems(QStringList() << "no device");
    /* 将一些按键使能 */
    ui->pushButton->setEnabled(true);
    ui->comboBoxDevice->setEnabled(true);
    ui->comboBoxFastbootMode->setEnabled(true);
    ui->pushButtonRun->setEnabled(true);
    /* 关闭日志文件 */
    logFile->close();


    if (ui->checkBoxIsUser->isChecked()) {
        QString utlName = QString("%1://%2:%3/%4").arg(ui->comboBoxScheme->currentText(),
                                                       ui->lineEditHost->text(),
                                                       ui->lineEditHttpPort->text(),
                                                       ui->lineEditHttpPath->text());
        ui_editShowInfo("运行结果上送服务器中...\n");
        http->setUrlSpec(utlName);
        http->setUploaderFile(logFile->fileName());
        http->start();
    } else {
        if (workUpdate->isSuccess()) {
            QMessageBox::information(this,"提示","设备升级成功");
        } else {
            QMessageBox::information(this,"提示","设备升级失败");
        }
    }
}

void MainWindow::on_httpFinished(void)
{
    if (http->error() == QNetworkReply::NoError) {
        ui_editShowInfo("服务器上送成功\n");
    } else {
        ui_editShowInfo("服务器上送失败\n");
    }

    if (workUpdate->isSuccess()) {
        QMessageBox::information(this,"提示","设备升级成功");
    } else {
        QMessageBox::information(this,"提示","设备升级失败");
    }
}

void MainWindow::on_searchFinished(void)
{
    ui_editShowInfo("搜索完成 ");
    QStringList device = adbDevices->getDevices();
    ui->comboBoxDevice->clear();
    if (device.isEmpty() == true) {
        ui_editShowInfo("未搜索到设备\n");
        ui->comboBoxDevice->addItems(QStringList() << "no device");
    } else {
        ui_editShowInfo("搜索到的设备 :\n");
        for (int i = 0; i < device.length(); i++) {
            ui_editShowInfo(device.at(i) + "\n");
        }
        ui->comboBoxDevice->addItems(device);
    }
    ui->pushButton->setEnabled(true);
}


/**
 * @brief MainWindow::on_pushButton_clicked adb搜索按钮按下事件处理
 */
void MainWindow::on_pushButton_clicked()
{
    ui_editShowClear();

    ui_editShowInfo("搜索设备中...\n");
    ui->pushButton->setEnabled(false);
    adbDevices->start();
}

void MainWindow::on_pushButtonRun_clicked()
{
    ui_editShowClear();

    if (ui->comboBoxDevice->currentText() == QString("no device")) {
        ui_editShowInfo("未搜索到adb设备，请重新搜索设备\n");
        QMessageBox::information(this,"错误","未搜索到adb设备");
        return;
    }

    if(folderPath.isEmpty() == true) {
        ui_editShowInfo("未指定升级的文件\n");
        QMessageBox::information(this,"错误","未指定升级的文件");
        return;
    }

    QString currentTime = QDateTime::currentDateTime().toString("yyyyMMddhhmmsszzz");
    logFile->setFileName("./log/" + currentTime + ".log");
    logFile->open(QIODevice::WriteOnly |QIODevice::Text);
    logFile->write(QString("[%1年%2月%3日 %4:%5:%6:%7]\n").arg(currentTime.mid(0,4),
                                                              currentTime.mid(4,2),
                                                              currentTime.mid(6,2),
                                                              currentTime.mid(8,2),
                                                              currentTime.mid(10,2),
                                                              currentTime.mid(12,2),
                                                              currentTime.mid(14,3)).toUtf8());

    /* 将一些按键失能 */
    ui->pushButton->setEnabled(false);
    ui->comboBoxDevice->setEnabled(false);
    ui->comboBoxFastbootMode->setEnabled(false);
    ui->pushButtonRun->setEnabled(false);

    /* 设置工作方式 */
    workMode = 0;
    if (ui->checkBoxAboot->isChecked())     {workMode |= WorkUpdate::AbootMode;     }
    if (ui->checkBoxBoot->isChecked())      {workMode |= WorkUpdate::BootMode;      }
    if (ui->checkBoxCache->isChecked())     {workMode |= WorkUpdate::CacheMode;     }
    if (ui->checkBoxDDR->isChecked())       {workMode |= WorkUpdate::DDRMode;       }
    if (ui->checkBoxPersist->isChecked())   {workMode |= WorkUpdate::PersistMode;   }
    if (ui->checkBoxReboot->isChecked())    {workMode |= WorkUpdate::RebootMode;    }
    if (ui->checkBoxRecovery->isChecked())  {workMode |= WorkUpdate::RecoveryMode;  }
    if (ui->checkBoxSplash->isChecked())    {workMode |= WorkUpdate::SplashMode;    }
    if (ui->checkBoxSystem->isChecked())    {workMode |= WorkUpdate::SystemMode;    }
    if (ui->checkBoxUserdata->isChecked())  {workMode |= WorkUpdate::UserdataMode;  }
    workUpdate->setWorkMode(workMode);
    /* 设置升级文件 */
    setWorkFileName();
    /* 设置升级设备 */
    workUpdate->setADBDevice(ui->comboBoxDevice->currentText());
    /* 开始升级系统 */
    workUpdate->start();
}





void MainWindow::ui_editShowInfo(const QString & info)
{
    ui->textEditInformation->insertPlainText(info);
}
void MainWindow::ui_editShowClear(void)
{
    ui->textEditInformation->clear();
}
void MainWindow::on_textEditInformation_textChanged()
{
    ui->textEditInformation->moveCursor(QTextCursor::End);
}
void MainWindow::setFastbootModeCheckBoxEnable(bool enable)
{
    ui->checkBoxAboot->   setEnabled(enable);
    ui->checkBoxBoot->    setEnabled(enable);
    ui->checkBoxCache->   setEnabled(enable);
    ui->checkBoxDDR->     setEnabled(enable);
    ui->checkBoxPersist-> setEnabled(enable);
    // ui->checkBoxReboot->  setEnabled(enable);
    ui->checkBoxSplash->  setEnabled(enable);
    ui->checkBoxSystem->  setEnabled(enable);
    ui->checkBoxUserdata->setEnabled(enable);
    ui->checkBoxUserdata->setEnabled(enable);
    ui->checkBoxRecovery->setEnabled(enable);
}
void MainWindow::setWorkFileName(void)
{
    if (cacheFilename.isEmpty() == false) {
        workUpdate->setWorkFileName(WorkUpdate::CacheMode, folderPath + cacheFilename.at(0));
    } else {
        workUpdate->setWorkFileName(WorkUpdate::CacheMode, UPDATA_CACHE_DEFAULT_FILENAME);
    }
    if (abootFilename.isEmpty() == false) {
        workUpdate->setWorkFileName(WorkUpdate::AbootMode, folderPath + abootFilename.at(0));
    } else {
        workUpdate->setWorkFileName(WorkUpdate::AbootMode, UPDATA_ABOOT_DEFAULT_FILENAME);
    }
    if (bootFilename.isEmpty() == false) {
        workUpdate->setWorkFileName(WorkUpdate::BootMode, folderPath + bootFilename.at(0));
    } else {
        workUpdate->setWorkFileName(WorkUpdate::BootMode, UPDATA_BOOT_DEFAULT_FILENAME);
    }
    if (persistFilename.isEmpty() == false) {
        workUpdate->setWorkFileName(WorkUpdate::PersistMode, folderPath + persistFilename.at(0));
    } else {
        workUpdate->setWorkFileName(WorkUpdate::PersistMode, UPDATA_PERSIST_DEFAULT_FILENAME);
    }
    if (recoveryFilename.isEmpty() == false) {
        workUpdate->setWorkFileName(WorkUpdate::RecoveryMode, folderPath + recoveryFilename.at(0));
    } else {
        workUpdate->setWorkFileName(WorkUpdate::RecoveryMode, UPDATA_RECOVERY_DEFAULT_FILENAME);
    }
    if (systemFilename.isEmpty() == false) {
        workUpdate->setWorkFileName(WorkUpdate::SystemMode, folderPath + systemFilename.at(0));
    } else {
        workUpdate->setWorkFileName(WorkUpdate::SystemMode, UPDATA_SYSTEM_DEFAULT_FILENAME);
    }
    if (userdataFilename.isEmpty() == false) {
        workUpdate->setWorkFileName(WorkUpdate::UserdataMode, folderPath + userdataFilename.at(0));
    } else {
        workUpdate->setWorkFileName(WorkUpdate::UserdataMode, UPDATA_USERDATA_DEFAULT_FILENAME);
    }
    if (DDRFilename.isEmpty() == false) {
        workUpdate->setWorkFileName(WorkUpdate::DDRMode, folderPath + DDRFilename.at(0));
    } else {
        workUpdate->setWorkFileName(WorkUpdate::DDRMode, UPDATA_DDR_DEFAULT_FILENAME);
    }

    /* splashFilename需要特殊处理 */
    if (ui->comboBoxSplash->count() != 0) {
        workUpdate->setWorkFileName(WorkUpdate::SplashMode, folderPath + ui->comboBoxSplash->currentText() + ".img");
    } else {
        workUpdate->setWorkFileName(WorkUpdate::SplashMode, UPDATA_SPLASH_DEFAULT_FILENAME);
    }
}






void MainWindow::on_pushButtonFolder_clicked()
{
   folderPath = QFileDialog::getExistingDirectory(this, "选择目录",
                                                  "E:\\Android\\SW_8953_android9_V01_20210120_HbSdk_factory\\SW_8953_android9_V01_20210120_HbSdk_factory\\",
                                                  QFileDialog::ShowDirsOnly) + "/";
   ui->lineEditURL->setText(folderPath);

   cacheFilename    = searchFileName(folderPath, UPDATA_CACHE_DEFAULT_FILENAME);
   abootFilename    = searchFileName(folderPath, "*appsboot*.mbn");
   bootFilename     = searchFileName(folderPath, UPDATA_BOOT_DEFAULT_FILENAME);
   persistFilename  = searchFileName(folderPath, UPDATA_PERSIST_DEFAULT_FILENAME);
   recoveryFilename = searchFileName(folderPath, UPDATA_RECOVERY_DEFAULT_FILENAME);
   systemFilename   = searchFileName(folderPath, UPDATA_SYSTEM_DEFAULT_FILENAME);
   userdataFilename = searchFileName(folderPath, UPDATA_USERDATA_DEFAULT_FILENAME);
   DDRFilename      = searchFileName(folderPath, UPDATA_DDR_DEFAULT_FILENAME);

   /* splashFilename需要特殊处理 */
   splashFilename   = searchFileName(folderPath, "splash*.img");
   ui->comboBoxSplash->clear();
   if (splashFilename.isEmpty() == false) {
       for (int i = 0; i < splashFilename.length(); i++) {
           ui->comboBoxSplash->addItem(splashFilename[i].remove(".img"));
       }
   }
}

void MainWindow::on_comboBoxFastbootMode_currentIndexChanged(int)
{
    if (ui->comboBoxFastbootMode->currentText() == QString("自定义模式")) {
        setFastbootModeCheckBoxEnable(true);
    } else {
        setFastbootModeCheckBoxEnable(false);
    }

    if (ui->comboBoxFastbootMode->currentText() == QString("Kernel Mode")) {
        ui->checkBoxAboot->   setChecked(false);
        ui->checkBoxBoot->    setChecked(true);
        ui->checkBoxCache->   setChecked(false);
        ui->checkBoxDDR->     setChecked(false);
        ui->checkBoxPersist-> setChecked(false);
        // ui->checkBoxReboot->  setEnabled(true);
        ui->checkBoxSplash->  setChecked(false);
        ui->checkBoxSystem->  setChecked(false);
        ui->checkBoxUserdata->setChecked(false);
        ui->checkBoxUserdata->setChecked(false);
        ui->checkBoxRecovery->setChecked(false);
    }
}

void MainWindow::on_checkBoxSplash_stateChanged(int arg1)
{
    if (arg1) {
        ui->comboBoxSplash->setEnabled(true);
    } else {
        ui->comboBoxSplash->setEnabled(false);
    }
}

void MainWindow::on_checkBoxIsUser_stateChanged(int arg1)
{
    if (arg1) {
        ui->comboBoxScheme  ->setEnabled(true);
        ui->lineEditHost    ->setEnabled(true);
        ui->lineEditHttpPath->setEnabled(true);
        ui->lineEditHttpPort->setEnabled(true);
    } else {
        ui->comboBoxScheme  ->setEnabled(false);
        ui->lineEditHost    ->setEnabled(false);
        ui->lineEditHttpPath->setEnabled(false);
        ui->lineEditHttpPort->setEnabled(false);
    }
}
