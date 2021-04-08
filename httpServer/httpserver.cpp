#include "httpServer/httpserver.h"

httpServer::httpServer(QObject *parent) : QObject(parent)
{
    _accessManager  = nullptr;
    _request        = nullptr;
    _reply          = nullptr;
    downloadFile    = nullptr;

    downloadFinished= true;
    networkError    = QNetworkReply::NoError;
}

void httpServer::httpDownloader(const QUrl &url, const QFile &download)
{
    downloadFile = new QFile(download.fileName());
    if (!downloadFile->exists()) {   /* 如果文件不存在 将需要下载的文件作为文件名并下载到当前目录 */
        downloadFile->setFileName(url.fileName());
    }
    if (downloadFile->isOpen()) {    /* 如果文件已经被打开 先关闭文件 */
        downloadFile->close();
    }
    downloadFile->open(QIODevice::WriteOnly | QIODevice::Truncate);

    downloadFinished = false;

    _accessManager = new QNetworkAccessManager();
    _accessManager->setNetworkAccessible(QNetworkAccessManager::Accessible);
    _request = new QNetworkRequest(url);
    _reply = _accessManager->get(*_request);

    connect(_reply, SIGNAL(readyRead()), this, SLOT(on_dataAvailable()));
    connect(_reply, SIGNAL(finished()),  this, SLOT(on_downloadfinished()));
//    connect(reply,SIGNAL(downloadProgress(qint64,qint64)),this,SLOT(on_downloadProgress(qint64,qint64)));//更新进度条
}
void httpServer::on_dataAvailable(void)
{
    if (downloadFile && _reply)
        downloadFile->write(_reply->readAll());
}
void httpServer::on_downloadfinished(void)
{
    networkError = _reply->error();
    qDebug() << networkError;

    downloadFile->flush();
    downloadFile->close();
    delete downloadFile;
    downloadFile = nullptr;

    _reply->deleteLater();
    _reply = nullptr;

    delete _accessManager;
    _accessManager = nullptr;

    delete _request;
    _request = nullptr;

    downloadFinished = true;
}


void httpServer::httpUploader(const QUrl &url, const QFile &upload, const QString &php) //http://localhost/phpbin/upload
{
//    QFile  uploadFile(urlSpec.fileName());
//    uploadFile.open(QIODevice::ReadOnly);
//    QByteArray byte_file = uploadFile.readAll();
//    uploadFile.close();

//    accessManagerUpload = new QNetworkAccessManager(this);    //往该目录中上传文件
//    accessManagerUpload->setNetworkAccessible(QNetworkAccessManager::Accessible);
//    QUrl url2(urlSpec.adjusted(QUrl::RemoveFilename).toString() + "upload.php?filename=" + urlSpec.fileName());
//    QNetworkRequest request2(url2);
//    request2.setHeader(QNetworkRequest::ContentTypeHeader, "application/octet-stream");
//    reply = accessManagerUpload->post(request2, byte_file);

//    connect(reply,SIGNAL(finished()),this,SLOT(on_uploadFinished()));
    // connect(reply,SIGNAL(uploadProgress(qint64,qint64)),this,SLOT(on_uploadProgress(qint64,qint64)));
    // connect(reply,SIGNAL(error(QNetworkReply::NetworkError)),this,SLOT(on_error(QNetworkReply::NetworkError)));
}
