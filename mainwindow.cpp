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
    cmd = new QProcess(0);


    adb = new AndroidDebugBridge();
    connect(adb, SIGNAL(adbFinished(int,QProcess::ExitStatus)),
            this,SLOT(on_adbFinished(int,QProcess::ExitStatus)));
    adbcmd = AndroidDebugBridge::NoCommand;
    /* 先对CDM命令做一次解析，判断对应的指令是否可以正常执行 */
    //    if (!adb->isAdbRun()) {
    //        QMessageBox::information(this,"错误","无法执行adb命令，请检查相应设备");
    //    }
    /* 上电更新一次设备列表 */
    // on_pushButton_clicked();

    fastboot = new Fastboot();
    connect(fastboot, SIGNAL(fastbootFinished(int,QProcess::ExitStatus)),
            this,SLOT(on_fastbootFinished(int,QProcess::ExitStatus)));




//    /* 下载进度条 */
//    connect(http,SIGNAL(downloadProgress(qint64,qint64)),
//            this,SLOT(updateDataReadProgress(qint64,qint64)));
//    /* 上传进度条 */
//    connect(http,SIGNAL(uploadProgress(qint64,qint64)),
//            this,SLOT(updateDataWritedProgress(qint64,qint64)));
//    /* 下载完成和上传完成 */
//    connect(http,SIGNAL(downloadFinished()),this,SLOT(httpFinished()));
//    connect(http,SIGNAL(uploadFinished()),this,SLOT(on_uploadFinished()));

//    connect(http,SIGNAL(httpError(QNetworkReply::NetworkError,HttpCommunication::HttpError)),
//            this,SLOT(on_httpError(QNetworkReply::NetworkError,HttpCommunication::HttpError)));


//    cmd->start("pwd");
//    cmd->waitForStarted();
//    cmd->waitForFinished();
//    ui->textEditInformation->setText(cmd->readAllStandardOutput());

    ui->progressBar->setValue(0);//将进度条的值设置为0
    ui->progressBar->show();//显示进度条

    // this->close();

}

MainWindow::~MainWindow()
{
    delete ui;
    delete http;
    delete cmd;
    delete adb;
}

void MainWindow::showPicture(QString fileName)
{
//    QMovie *mov = new QMovie(fileName);
//    mov->setScaledSize(ui->pushButtonExit->size());//自适应label的大小
//    ui->pushButtonExit->setMovie(mov);
//    mov->start();
}

void MainWindow::on_pushButtonLoad_clicked()
{
    // http->httpDownload("http://localhost/phpbin/upload", "123.jpg");
    // QProcess p(0);
//    cmd->start("ping 192.168.1.31");
//    connect(cmd,SIGNAL(readyReadStandardOutput()),this,SLOT(on_uploadFinished()));
//    p.waitForStarted();
//    p.waitForFinished();
//    qDebug()<<"error = " << QString::number(p.error());
//    ui->labelShow->setText(QString::fromLocal8Bit(p.readAllStandardOutput()));

//    fileName = QFileDialog::getExistingDirectory(this, tr("选择文件夹"));
//    ui->lineEditURL->setText(fileName);
    ui->textEditInformation->insertPlainText(QString::fromLocal8Bit(cmd->readAllStandardOutput()));
    ui->textEditInformation->insertPlainText(QString::fromLocal8Bit(cmd->readAllStandardError()));
}

void MainWindow::on_pushButtonExit_clicked()
{
    // http->httpUpload("123.jpg", "http://localhost/phpbin/upload");

//    cmd->start("pwd");
//    cmd->waitForStarted();
//    cmd->waitForFinished();
//    qDebug()<<"error = " << QString::number(cmd->error());
//    qDebug()<<QString::fromLocal8Bit(cmd->readAllStandardOutput());
    //ui->textEditInformation->setText(QString::fromLocal8Bit(p.readAllStandardOutput()));
//    a = 0;

    fileName = "E:\\Android\\SW_SD5000_V028_A01S\\SW_SD5000_V028_A01S";
    cmd->setWorkingDirectory(fileName);

    cmd->start("E:\\Android\\SW_SD5000_V028_A01S\\SW_SD5000_V028_A01S\\download_kernel.cmd");
//    a++;
    connect(cmd,SIGNAL(readyReadStandardOutput()),this,SLOT(on_uploadFinished()));
    connect(cmd,SIGNAL(readyReadStandardError()),this,SLOT(on_uploadFinished()));
    connect(cmd,SIGNAL(finished(int)),this,SLOT(on_CmdFinished(int)));
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
    showPicture("123.jpg");
}

void MainWindow::on_uploadFinished()
{
    // QMessageBox::information(this,"提示","文件已上传");
    ui->textEditInformation->insertPlainText(QString::fromLocal8Bit(cmd->readAllStandardOutput()));
    // ui->textEditInformation->insertPlainText(QString::fromLocal8Bit(cmd->readAllStandardError()));
}

void MainWindow::on_httpError(QNetworkReply::NetworkError networkError, HttpCommunication::HttpError http_error)
{
    qDebug() << "networkError : " << QString::number(networkError) << "\nhttpError : " << QString::number(http_error);
}

void MainWindow::on_textEditInformation_textChanged()
{
    ui->textEditInformation->moveCursor(QTextCursor::End);
}

void MainWindow::on_CmdFinished(int)
{
    qDebug() << QString("finished %1").arg(QString::number((a)));
}

void MainWindow::on_pushButton_clicked()
{
    ui->textEditInformation->clear();
    adbcmd = AndroidDebugBridge::SearchDevice;
    ui->textEditInformation->insertPlainText("正在扫描设备...\n");
    ui->pushButton->setEnabled(false);
    adb->searchDevice();
//    connect(adb, SIGNAL(adbFinished(int,QProcess::ExitStatus)),
//            this,SLOT(on_adbFinished(int,QProcess::ExitStatus)));
}

void MainWindow::on_pushButtonRun_clicked()
{
    ui->textEditInformation->clear();
    if (ui->comboBoxDevice->currentText() == QString("no device")) {
        ui->textEditInformation->insertPlainText("未检测出ADB设备，请重新扫描设备\n");
        return;
    }
    ui->textEditInformation->insertPlainText("设备正在进入boot loader模式...\n");
    adbcmd = AndroidDebugBridge::RebootBootloader;
    adb->entryBootloader(ui->comboBoxDevice->currentText());
//    connect(adb, SIGNAL(adbFinished(int,QProcess::ExitStatus)),
//            this,SLOT(on_adbFinished(int,QProcess::ExitStatus)));
}

void MainWindow::on_adbFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    switch ((int)adbcmd) {
    case AndroidDebugBridge::SearchDevice:
        ui->textEditInformation->insertPlainText("扫描设备完成\n");
        ui->pushButton->setEnabled(true);
        ui->comboBoxDevice->clear();
        ui->comboBoxDevice->addItems(adb->getDevicesList());
        if (ui->comboBoxDevice->count() == 0) {
            ui->comboBoxDevice->addItem("no device");
        }
    break;
    case AndroidDebugBridge::RebootBootloader:
        if ((exitCode == 0) && (exitStatus == QProcess::NormalExit)) {
            if (adb->adbError()) {
                ui->textEditInformation->insertPlainText(QString("设备进入boot loader模式失败 错误代码： %1\n").arg(adb->adbError()));
                break;
            }
            ui->textEditInformation->insertPlainText("设备进入boot loader成功\n");
            ui->textEditInformation->insertPlainText("正在重启设备...\n");
            fastboot->fsatbootReboot();
        } else {
            ui->textEditInformation->insertPlainText("adb 命令进程异常退出\n");
        }
    break;
    }
    adbcmd = AndroidDebugBridge::NoCommand;
}

void MainWindow::on_fastbootFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    if ((exitCode == 0) && (exitStatus == QProcess::NormalExit)) {
        if (!fastboot->fsatbootError()) {
            ui->textEditInformation->insertPlainText("设备重启成功\n");
        } else {
            ui->textEditInformation->insertPlainText("设备重启失败\n");
        }
        ui->textEditInformation->insertPlainText("fast boot 设备升级完成\n");
    } else {
        ui->textEditInformation->insertPlainText("fastboot 命令进程异常退出\n");
    }
}
