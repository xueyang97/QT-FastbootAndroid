#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    http = new HttpCommunication();

    connect(http,SIGNAL(downloadProgress(qint64,qint64)),
            this,SLOT(updateDataReadProgress(qint64,qint64)));

    connect(http,SIGNAL(uploadProgress(qint64,qint64)),
            this,SLOT(updateDataWritedProgress(qint64,qint64)));

    connect(http,SIGNAL(downloadFinished()),this,SLOT(httpFinished()));
    connect(http,SIGNAL(uploadFinished()),this,SLOT(on_uploadFinished()));

    connect(http,SIGNAL(httpError(QNetworkReply::NetworkError,HttpCommunication::HttpError)),
            this,SLOT(on_httpError(QNetworkReply::NetworkError,HttpCommunication::HttpError)));

    ui->progressBar->setValue(0);//将进度条的值设置为0
    ui->progressBar->show();//显示进度条
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showPicture(QString fileName)
{
    QMovie *mov = new QMovie(fileName);
    mov->setScaledSize(ui->labelShow->size());//自适应label的大小
    ui->labelShow->setMovie(mov);
    mov->start();
}

void MainWindow::on_pushButtonLoad_clicked()
{
    http->httpDownload("http://localhost/phpbin/upload", "123.jpg");


//    QString urlSpec = ui->lineEditURL->text();
//    url.setUrl(urlSpec);
//    QFileInfo info(url.path());//获取文件夹的绝对路径
//    qDebug() << url.path();
//    QString fn(info.fileName());//获取需要下载文件的文件名
//    if (fn.isEmpty()) {
//        qDebug() << QMessageBox::information(this,"错误","请指定需要下载的文件URL");
//        return;
//    }
//    file = new QFile(fn);//生成一个相同文件名的文件夹，用来存放下载的内容
//    if (file->exists()) {   //文件已经存在删除文件
//        QMessageBox::StandardButton MessageBoReturn;
//        MessageBoReturn = QMessageBox::information(this,"提示","下载文件已存在，是否覆盖文件", QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
//        if (MessageBoReturn == QMessageBox::Yes) {
//            file->remove();
//        }
//        else {
//            /* 显示图片 */
//            return;
//        }

//    }

//    if (!file->open(QIODevice::ReadWrite)) {
//        qDebug() << "error";
//    }
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
    ui->progressBar->setMaximum(1);
    ui->progressBar->setValue(1);
    QMessageBox::information(this,"提示","文件已上传");
    // showPicture("123.jpg");
}

void MainWindow::on_httpError(QNetworkReply::NetworkError networkError, HttpCommunication::HttpError http_error)
{
    qDebug() << "networkError : " << QString::number(networkError) << "\nhttpError : " << QString::number(http_error);
}

void MainWindow::on_pushButtonExit_clicked()
{
    http->httpUpload("123.jpg", "http://localhost/phpbin/upload");
}
