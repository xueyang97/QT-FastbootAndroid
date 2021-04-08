#include "httpcommunication.h"

HttpServer::HttpServer(QThread *parent) : QThread(parent)
{
    moveToThread(this);
    isDownloadRunning = false;
    isUploadRunning = false;
}

HttpServer::~HttpServer()
{
}

void HttpServer::setUploaderFile(const QString &fileName)
{
    logFile.setFileName(fileName);
}
void HttpServer::setUrlSpec(const QString &urName)
{
    qurl.setUrl(urName);
}

void HttpServer::run(void)
{
    QFile downloadFile;
    downloadFile.setFileName(qurl.fileName());

    networkError = QNetworkReply::NoError;
    httpDownload(qurl);
    this->exec();
    if (networkError != QNetworkReply::NoError) {
        downloadFile.remove();
        qDebug() << networkError;
        return;
    }

    logFile.open(QIODevice::ReadOnly);
    downloadFile.open(QIODevice::Append);
    downloadFile.write(logFile.readAll());
    downloadFile.close();
    logFile.close();

    httpUpload(qurl);
    this->exec();
    downloadFile.remove();
    if (networkError != QNetworkReply::NoError) {
        qDebug() << networkError;
        return;
    }
}

void HttpServer::httpDownload(const QUrl &urlSpec)
{
    file = new QFile(urlSpec.fileName());
    file->remove();
    if (!file->open(QIODevice::WriteOnly)) {
        delete file;
        return;
    }

    accessManager = new QNetworkAccessManager(this);
    accessManager->setNetworkAccessible(QNetworkAccessManager::Accessible);

    request = new QNetworkRequest(urlSpec);
    reply = accessManager->get(*request);//当服务器接收到请求后

    connect(reply,SIGNAL(readyRead()),this,SLOT(on_readyRead()));//服务器响应请求后，开始下载内容
    connect(reply,SIGNAL(finished()),this,SLOT(on_downloadFinished()));//下载完成后，关闭文件
    // connect(reply,SIGNAL(downloadProgress(qint64,qint64)),this,SLOT(on_downloadProgress(qint64,qint64)));//更新进度条
    // connect(reply,SIGNAL(error(QNetworkReply::NetworkError)),this,SLOT(on_error(QNetworkReply::NetworkError)));
}

void HttpServer::httpUpload(const QUrl &urlSpec) //http://localhost/phpbin/upload
{
    QFile  uploadFile(urlSpec.fileName());
    uploadFile.open(QIODevice::ReadOnly);
    QByteArray byte_file = uploadFile.readAll();
    uploadFile.close();

    accessManagerUpload = new QNetworkAccessManager(this);    //往该目录中上传文件
    accessManagerUpload->setNetworkAccessible(QNetworkAccessManager::Accessible);
    QUrl url2(urlSpec.adjusted(QUrl::RemoveFilename).toString() + "upload.php?filename=" + urlSpec.fileName());
    QNetworkRequest request2(url2);
    request2.setHeader(QNetworkRequest::ContentTypeHeader, "application/octet-stream");
    reply = accessManagerUpload->post(request2, byte_file);

    connect(reply,SIGNAL(finished()),this,SLOT(on_uploadFinished()));
    // connect(reply,SIGNAL(uploadProgress(qint64,qint64)),this,SLOT(on_uploadProgress(qint64,qint64)));
    // connect(reply,SIGNAL(error(QNetworkReply::NetworkError)),this,SLOT(on_error(QNetworkReply::NetworkError)));
}

QNetworkReply::NetworkError HttpServer::error(void)
{
    return networkError;
}

void HttpServer::on_readyRead()
{
    if(file) {
        file->write(reply->readAll());//读取文件
    }
}

void HttpServer::on_downloadFinished()
{
    networkError = reply->error();
    file->flush();
    file->close();
    reply->deleteLater();

    delete accessManager;
    delete request;
    delete file;
    file  = 0;
    request = 0;
    accessManager = 0;

    quit();
}

void HttpServer::on_uploadFinished()
{
    reply->deleteLater();
    delete accessManagerUpload;
    accessManagerUpload = 0;
    quit();
}

void HttpServer::on_downloadProgress(qint64 progressMaximum, qint64 progressValue)
{
    downloadProgressMaximum = progressMaximum;
    downloadProgressValue = progressValue;
    qDebug()<<QString::number(progressValue)<< " / "<<QString::number(progressMaximum);
    emit downloadProgress(progressMaximum, progressValue);
}

void HttpServer::on_uploadProgress(qint64 progressValue, qint64 progressMaximum)
{
    uploadProgressMaximum = progressMaximum;
    uploadProgressValue = progressValue;
    qDebug()<<QString::number(progressValue)<< " / "<<QString::number(progressMaximum);
    emit uploadProgress(progressMaximum, progressValue);
}

void HttpServer::on_error(QNetworkReply::NetworkError networkError)
{
    this->networkError = networkError;
    qDebug()<<"networkError: "<< QString::number(networkError);
    qDebug()<<"HttpError: "<< QString::number(HttpServer::NoError);
}

