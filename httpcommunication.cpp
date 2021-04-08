#include "httpcommunication.h"

HttpCommunication::HttpCommunication()
{
    isDownloadRunning = false;
    isUploadRunning = false;
}

HttpCommunication::~HttpCommunication()
{
}

void HttpCommunication::httpDownload(QString urlSpec, QString fileName)
{

    file = new QFile(fileName);
    file->remove();
    if (!file->open(QIODevice::WriteOnly)) {
        delete file;
        emit downloadFinished();
        return;
    }

    isDownloadRunning = true;
    networkError = QNetworkReply::NoError;

    QUrl url(urlSpec + '/' + fileName);
    accessManager = new QNetworkAccessManager(this);
    accessManager->setNetworkAccessible(QNetworkAccessManager::Accessible);
    reply = accessManager->get(QNetworkRequest(url));//当服务器接收到请求后

    connect(reply,SIGNAL(readyRead()),this,SLOT(on_httpReadyRead()));//服务器响应请求后，开始下载内容
    connect(reply,SIGNAL(finished()),this,SLOT(on_downloadFinished()));//下载完成后，关闭文件
    connect(reply,SIGNAL(downloadProgress(qint64,qint64)),this,SLOT(on_downloadProgress(qint64,qint64)));//更新进度条
    connect(reply,SIGNAL(error(QNetworkReply::NetworkError)),this,SLOT(on_error(QNetworkReply::NetworkError)));
}

void HttpCommunication::httpUpload(QString fileName, QString urlSpec) //http://localhost/phpbin/upload
{
    QFile  uploadFile(fileName);
    if (!uploadFile.exists()) {
        qDebug() << "上传的文件不存在";
        emit uploadFinished();
        // emit httpError(QNetworkReply::NoError, UploadFileNotOpen);
        return;
    }
    uploadFile.open(QIODevice::ReadOnly);
    QByteArray byte_file = uploadFile.readAll();
    uploadFile.close();

    isUploadRunning = true;
    networkError = QNetworkReply::NoError;

    accessManagerUpload = new QNetworkAccessManager(this);    //往该目录中上传文件
    accessManagerUpload->setNetworkAccessible(QNetworkAccessManager::Accessible);
    QUrl url2(urlSpec + '/' + "upload.php?filename=" + fileName);
    // QUrl url2 = QUrl(urlSpec).resolved(QUrl("upload.php"));
    // url2.setQuery(QString("foldername=%1&filename=%2").arg(QUrl(urlSpec).fileName(), fileName));
    QNetworkRequest request2(url2);
    request2.setHeader(QNetworkRequest::ContentTypeHeader, "application/octet-stream");
    reply = accessManagerUpload->post(request2, byte_file);

    connect(reply,SIGNAL(finished()),this,SLOT(on_uploadFinished()));
    connect(reply,SIGNAL(uploadProgress(qint64,qint64)),this,SLOT(on_uploadProgress(qint64,qint64)));
    connect(reply,SIGNAL(error(QNetworkReply::NetworkError)),this,SLOT(on_error(QNetworkReply::NetworkError)));
}

void HttpCommunication::waitDownloadFinished(void)
{
    while(isDownloadRunning);
}
void HttpCommunication::waitUploadFinished(void)
{
    while(isUploadRunning);
}

QNetworkReply::NetworkError HttpCommunication::error(void)
{
    return networkError;
}

void HttpCommunication::on_httpReadyRead()
{
    if(file) {
        file->write(reply->readAll());//读取文件
    }
}

void HttpCommunication::on_downloadFinished()
{
    file->flush();
    file->close();
    reply->deleteLater();
    delete accessManager;
    delete file;
    file  = 0;
    accessManager = 0;
    isDownloadRunning = false;
    emit downloadFinished();
}

void HttpCommunication::on_uploadFinished()
{
    reply->deleteLater();
    delete accessManagerUpload;
    accessManagerUpload = 0;
    isUploadRunning = false;
    emit uploadFinished();
}

void HttpCommunication::on_downloadProgress(qint64 progressMaximum, qint64 progressValue)
{
    downloadProgressMaximum = progressMaximum;
    downloadProgressValue = progressValue;
    qDebug()<<QString::number(progressValue)<< " / "<<QString::number(progressMaximum);
    emit downloadProgress(progressMaximum, progressValue);
}

void HttpCommunication::on_uploadProgress(qint64 progressValue, qint64 progressMaximum)
{
    uploadProgressMaximum = progressMaximum;
    uploadProgressValue = progressValue;
    qDebug()<<QString::number(progressValue)<< " / "<<QString::number(progressMaximum);
    emit uploadProgress(progressMaximum, progressValue);
}

void HttpCommunication::on_error(QNetworkReply::NetworkError networkError)
{
    this->networkError = networkError;
    qDebug()<<"networkError: "<< QString::number(networkError);
    qDebug()<<"HttpError: "<< QString::number(HttpCommunication::NoError);
}

