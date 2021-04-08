#ifndef HTTPCOMMUNICATION_H
#define HTTPCOMMUNICATION_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QThread>
#include <QFile>
#include <QDebug>

class HttpServer : public QThread
{
    Q_OBJECT
public:
    enum HttpError {
        NoError = 0,
        DownloadFileNotOpen = 500,    //需要下载的文件未打开
        UploadFileNotOpen,    //上传的文件不存在
    };
    Q_ENUM(HttpError)

    HttpServer(QThread *parent = nullptr);
    ~HttpServer();
    void httpDownload(const QUrl &urlSpec);
    void httpUpload(const QUrl &urlSpec);
//    void waitDownloadFinished(void);
//    void waitUploadFinished(void);
    QNetworkReply::NetworkError error(void);

    void setUploaderFile(const QString &fileName);
    void setUrlSpec(const QString &urName);

protected:
    void run(void) Q_DECL_OVERRIDE;

private slots :
    void on_readyRead();
    void on_downloadFinished();
    void on_uploadFinished();
    void on_downloadProgress(qint64 progressMaximum, qint64 progressValue);
    void on_uploadProgress(qint64 progressMaximum, qint64 progressValue);
    void on_error(QNetworkReply::NetworkError networkError);

private:
    QNetworkAccessManager *accessManager;
    QNetworkAccessManager *accessManagerUpload;
    QNetworkRequest *request;
    QNetworkReply *reply;
    QFile *file;

    QFile logFile;
    QUrl  qurl;
    QNetworkReply::NetworkError networkError;

    qint64 downloadProgressMaximum;
    qint64 downloadProgressValue;
    qint64 uploadProgressMaximum;
    qint64 uploadProgressValue;


    bool isDownloadRunning;
    bool isUploadRunning;

signals :
    void downloadProgress(qint64 progressMaximum, qint64 progressValue);
    void uploadProgress(qint64 progressValue, qint64 progressMaximum);
    void downloadFinished();
    void uploadFinished();
    // void httpError(QNetworkReply::NetworkError networkError, HttpCommunication::HttpError http_error);
};

#endif // HTTPCOMMUNICATION_H
