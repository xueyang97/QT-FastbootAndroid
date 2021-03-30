#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    manager = new QNetworkAccessManager(this);
    ui->progressBar->setValue(0);//将进度条的值设置为0
    ui->progressBar->show();//显示进度条
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::startRequest(QUrl url)
{
    reply = manager->get(QNetworkRequest(url));//当服务器接收到请求后
    // connect(reply,SIGNAL(finished()),this,SLOT(replyFinished(QNetworkReplty *)));//显示图片
    connect(reply,SIGNAL(readyRead()),this,SLOT(httpReadyRead()));//服务器响应请求后，开始下载内容
    connect(reply,SIGNAL(finished()),this,SLOT(httpFinished()));//下载完成后，关闭文件
    connect(reply,SIGNAL(downloadProgress(qint64,qint64)),this,SLOT(updateDataReadProgress(qint64,qint64)));//更新进度条
}






void MainWindow::on_pushButtonLoad_clicked()
{
    QString urlSpec = ui->lineEditURL->text();
    url.setUrl(urlSpec);
    QFileInfo info(url.path());//获取文件夹的绝对路径
    qDebug() << url.path();
    QString fn(info.fileName());//获取需要下载文件的文件名
    if (fn.isEmpty()) {
        qDebug() << QMessageBox::information(this,"错误","请指定需要下载的文件URL");
        return;
    }
    file = new QFile(fn);//生成一个相同文件名的文件夹，用来存放下载的内容
    if (file->exists()) {   //文件已经存在删除文件
        QMessageBox::StandardButton MessageBoReturn;
        MessageBoReturn = QMessageBox::information(this,"提示","下载文件已存在，是否覆盖文件", QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
        if (MessageBoReturn == QMessageBox::Yes) {
            file->remove();
        }
        else {
            /* 显示图片 */
//            QMovie *mov = new QMovie(file->fileName());
//            mov->setScaledSize(ui->labelShow->size());//自适应label的大小
//            ui->labelShow->setMovie(mov);
//            mov->start();
//            delete mov;
            return;
        }

    }

    if (!file->open(QIODevice::ReadWrite)) {
        qDebug() << "error";
    }

    startRequest(url);//发送请求

    ui->progressBar->setValue(0);//将进度条的值设置为0
//    ui->progressBar->show();//显示进度条
}

void MainWindow::updateDataReadProgress(qint64 a,qint64 b)
{//更新进度条
    ui->progressBar->setMaximum(b);
    ui->progressBar->setValue(a);
}

void MainWindow::httpReadyRead(){
    temp = reply->readAll();
    if(file)
        file->write(temp);//读取文件
}

void MainWindow::httpFinished()
{
    qDebug() << "文件下载结束";
    /* 显示图片 */
    QMovie *mov = new QMovie(file->fileName());
    mov->setScaledSize(ui->labelShow->size());//自适应label的大小
    ui->labelShow->setMovie(mov);
    mov->start();

    /* 关闭文件 */
    file->flush();
    file->close();
    reply->deleteLater();
    delete file;
    file  =0;
}

void MainWindow::replyFinished(QNetworkReply *reply)
{
    QMovie *mov = new QMovie(reply);
    mov->setScaledSize(ui->labelShow->size());//自适应label的大小
    ui->labelShow->setMovie(mov);
    mov->start();
}
