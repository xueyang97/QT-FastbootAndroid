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
    file = new QFile(fileName);

    if (!file->open(QIODevice::ReadOnly)) {
        qDebug() << "上传的文件不存在";
        emit httpError(QNetworkReply::NoError, UploadFileNotOpen);
        return;
    }

    QByteArray byte_file = file->readAll();
    file->flush();
    file->close();
    delete file;
    file  = 0;

    accessManager1 = new QNetworkAccessManager(this);  //在服务器上创建目录
    accessManager1->setNetworkAccessible(QNetworkAccessManager::Accessible);
    QByteArray data;
    QUrl url1 = QUrl(urlSpec).resolved(QUrl("createFolder.php"));
    url1.setQuery(QString("foldername=%1").arg(QUrl(urlSpec).fileName()));
    QNetworkRequest request1(url1);
    request1.setHeader(QNetworkRequest::ContentTypeHeader, QLatin1String("application/octet-stream"));
    accessManager1->post(request1, data);

    accessManager2 = new QNetworkAccessManager(this);    //往该目录中上传文件
    accessManager2->setNetworkAccessible(QNetworkAccessManager::Accessible);
    QUrl url2 = QUrl(urlSpec).resolved(QUrl("upload.php"));
    url2.setQuery(QString("foldername=%1&filename=%2").arg(QUrl(urlSpec).fileName(), fileName));
    QNetworkRequest request2(url2);
    request2.setHeader(QNetworkRequest::ContentTypeHeader, "application/octet-stream");
    reply = accessManager2->post(request2, byte_file);

    connect(reply,SIGNAL(finished()),this,SLOT(on_uploadFinished()));
    connect(reply,SIGNAL(uploadProgress(qint64,qint64)),this,SLOT(on_uploadProgress(qint64,qint64)));
    connect(reply,SIGNAL(error(QNetworkReply::NetworkError)),this,SLOT(on_error(QNetworkReply::NetworkError)));
}

void HttpCommunication::on_httpReadyRead()
{
    if(file) {
        file->write(reply->readAll());//读取文件
    }
}

void HttpCommunication::on_downloadFinished()
{
    qDebug() << "文件下载结束";

    file->flush();
    file->close();
    reply->deleteLater();
    delete accessManager;
    delete file;
    file  = 0;
    accessManager = 0;

    emit downloadFinished();
}

void HttpCommunication::on_uploadFinished()
{
    qDebug() << "文件上传完成";
    qDebug() << QString::number(reply->error());

    reply->deleteLater();
    delete accessManager1;
    delete accessManager2;
    accessManager1 = 0;
    accessManager2 = 0;

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
    qDebug()<<"networkError: "<< QString::number(networkError);
    qDebug()<<"HttpError: "<< QString::number(HttpCommunication::NoError);
    emit httpError(networkError, HttpCommunication::NoError);
}

qint64 HttpCommunication::uploadMaximum(void)
{
    return uploadProgressMaximum;
}
qint64 HttpCommunication::uploadValue(void)
{
    return uploadProgressValue;
}
