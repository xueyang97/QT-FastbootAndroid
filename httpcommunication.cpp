#include "httpcommunication.h"

HttpCommunication::HttpCommunication()
{
}

HttpCommunication::~HttpCommunication()
{
}

void HttpCommunication::httpDownload(QString urlSpec, QString fileName)
{
    file = new QFile(fileName);

    file->remove();
    if (!file->open(QIODevice::WriteOnly)) {
        emit httpError(QNetworkReply::NoError, DownloadFileNotOpen);
        return;
    }

    QUrl url(urlSpec);
    manager = new QNetworkAccessManager(this);
    reply = manager->get(QNetworkRequest(url));//当服务器接收到请求后

    connect(reply,SIGNAL(readyRead()),this,SLOT(on_httpReadyRead()));//服务器响应请求后，开始下载内容
    connect(reply,SIGNAL(finished()),this,SLOT(on_httpFinished()));//下载完成后，关闭文件
    connect(reply,SIGNAL(downloadProgress(qint64,qint64)),this,SLOT(on_downloadProgress(qint64,qint64)));//更新进度条
    connect(reply,SIGNAL(error(QNetworkReply::NetworkError)),this,SLOT(on_error(QNetworkReply::NetworkError)));
}

void HttpCommunication::on_httpReadyRead()
{
    if(file) {
        file->write(reply->readAll());//读取文件
    }
}

void HttpCommunication::on_httpFinished()
{
    qDebug() << "文件下载结束";

    file->flush();
    file->close();
    reply->deleteLater();
    delete manager;
    delete file;
    file  = 0;
    manager = 0;

    emit downloadFinished();
}

void HttpCommunication::on_downloadProgress(qint64 progressMaximum, qint64 progressValue)
{
    emit downloadProgress(progressMaximum, progressValue);
}

void HttpCommunication::on_error(QNetworkReply::NetworkError networkError)
{
    qDebug()<<"networkError: "<< QString::number(networkError);
    qDebug()<<"HttpError: "<< QString::number(HttpCommunication::NoError);
    emit httpError(networkError, HttpCommunication::NoError);
}
