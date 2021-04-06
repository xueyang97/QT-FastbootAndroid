#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QCloseEvent>
void MainWindow::closeEvent( QCloseEvent *event )
{
    event->accept();
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    /* 禁止用户修改窗口大小 */
    setWindowFlags(windowFlags()& ~Qt::WindowMaximizeButtonHint);
    setFixedSize(this->width(), this->height());

    http = new HttpCommunication();
    adb = new AndroidDebugBridge(0);
    fastboot = new Fastboot(0);

    workUpdate = new WorkUpdate();
    connect(workUpdate,SIGNAL(readyReadStandardOutput()),this,SLOT(on_workReadyReadStandardOutput()));


//    qDebug() << "search adb devices :";
//    const QStringList &adbDevice = adb->searchDevice();
//    if (adbDevice.isEmpty() != true) {
//        qDebug() << "adb reboot bootloader :";
//        adb->entryBootloader(adbDevice.at(0));
//        QThread::sleep(10);
//    }

//    qDebug() << "search fastboot devices :";
//    const QStringList &fastbootDevice = fastboot->searchDevice();
//    if (fastbootDevice.isEmpty() != true) {
//        qDebug() << "fastboot flash boot boot.img :";
//        fastboot->flashBootDownload(fastbootDevice.at(0),"E:\\Android\\SW_8953_android9_V01_20210120_HbSdk_factory\\SW_8953_android9_V01_20210120_HbSdk_factory\\boot.img");

//        qDebug() << "fastboot reboot :";
//        fastboot->fsatbootReboot(fastbootDevice.at(0));
//    }

}

MainWindow::~MainWindow()
{
    delete ui;
    delete http;
    delete adb;
}

void MainWindow::on_workReadyReadStandardOutput(void)
{
    ui_editShowInfo(workUpdate->readAllStandardOutput());
}

void MainWindow::on_pushButtonLoad_clicked()
{
    // http->httpDownload("http://localhost/phpbin/upload", "123.jpg");
}

void MainWindow::on_pushButtonExit_clicked()
{
    // http->httpUpload("123.jpg", "http://localhost/phpbin/upload");
}

void MainWindow::updateDataReadProgress(qint64 a,qint64 b)
{
    ui->progressBar->setMaximum(b);
    ui->progressBar->setValue(a);
}

void MainWindow::updateDataWritedProgress(qint64 b,qint64 a)
{
    ui->progressBar->setMaximum(b);
    ui->progressBar->setValue(a);
}

void MainWindow::httpFinished()
{
}

void MainWindow::on_uploadFinished()
{
    // QMessageBox::information(this,"提示","文件已上传");
    // ui->textEditInformation->insertPlainText(QString::fromLocal8Bit(cmd->readAllStandardOutput()));
    // ui->textEditInformation->insertPlainText(QString::fromLocal8Bit(cmd->readAllStandardError()));
}

void MainWindow::on_httpError(QNetworkReply::NetworkError networkError, HttpCommunication::HttpError http_error)
{
    qDebug() << "networkError : " << QString::number(networkError) << "\nhttpError : " << QString::number(http_error);
}

/**
 * @brief MainWindow::on_pushButton_clicked adb搜索按钮按下事件处理
 */
void MainWindow::on_pushButton_clicked()
{
    ui_editShowClear();

    ui_editShowInfo("搜索设备中...\n");
    const QStringList &adbDevice = adb->searchDevice();
    ui_editShowInfo("搜索完成 ");
    if (adbDevice.isEmpty() == true) {
        ui_editShowInfo("未搜索到设备\n");
    } else {
        ui_editShowInfo("搜索到的设备 :\t");
        for (int i = 0; i < adbDevice.length(); i++) {
            ui_editShowInfo(adbDevice.at(i));
        }
    }

    ui->comboBoxDevice->clear();
    if (adbDevice.isEmpty() == true) {
        ui->comboBoxDevice->addItems(QStringList() << "no device");
    } else {
        ui->comboBoxDevice->addItems(adbDevice);
    }
}

void MainWindow::on_pushButtonRun_clicked()
{
    ui_editShowClear();

    if (ui->comboBoxDevice->currentText() == QString("no device")) {
        ui_editShowInfo("未搜索到adb设备，请重新搜索设备\n");
        return;
    }

    workUpdate->setADBDevice(ui->comboBoxDevice->currentText());
    workUpdate->start();

//    QString adbDevice = ui->comboBoxDevice->currentText();
//    ui_editShowInfo("adb进入bootloader模式...\n");
//    adb->entryBootloader(adbDevice);
//    QThread::sleep(6);
//    if (adb->adbError() != AndroidDebugBridge::NoError) {
//        ui_editShowInfo(QString("adb进入bootloader失败 错误代码 : %1\n").arg(adb->adbError()));
//        return;
//    }
//    ui_editShowInfo("adb进入bootloader模式成功\n");

//    const QStringList &fastbootDeviceList = fastboot->searchDevice();
//    if (fastbootDeviceList.isEmpty() == true) {
//        ui_editShowInfo("未搜索到fastboot设备，fastboot下载失败\n");
//        return;
//    }
//    QString fastbootDevice = fastbootDeviceList.at(0);

//    ui_editShowInfo("正在升级boot分区...\n");
//    fastboot->flashBootDownload(fastbootDevice,"E:\\Android\\SW_8953_android9_V01_20210120_HbSdk_factory\\SW_8953_android9_V01_20210120_HbSdk_factory\\boot.img");
//    if (fastboot->fsatbootError() != Fastboot::NoError) {
//        ui_editShowInfo(QString("fastboot升级boot分区失败 错误代码 : %1\n").arg(fastboot->fsatbootError()));
//    }
//    ui_editShowInfo(QString("fastboot升级boot分区成功\n"));

//    ui_editShowInfo("正在重启设备...\n");
//    fastboot->fsatbootReboot(fastbootDevice);
//    if (fastboot->fsatbootError() != Fastboot::NoError) {
//        ui_editShowInfo(QString("fastboot重启设备失败 错误代码 : %1\n").arg(fastboot->fsatbootError()));
//    }
//    ui_editShowInfo(QString("fastboot重启设备成功\n"));

//    ui_editShowInfo(QString("fastboot设备升级成功\n"));
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

