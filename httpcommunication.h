#ifndef HTTPCOMMUNICATION_H
#define HTTPCOMMUNICATION_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QFile>
#include <QDebug>

class HttpCommunication : public QObject
{
    Q_OBJECT
public:
    enum HttpError {
        NoError = 0,
        DownloadFileNotOpen = 500,    //需要下载的文件未打开
        UploadFileNotOpen,    //上传的文件不存在
    };
    Q_ENUM(HttpError)

    HttpCommunication();
    ~HttpCommunication();
    void httpDownload(QString urlSpec, QString fileName);
    void httpUpload(QString fileName, QString urlSpec);
    void waitDownloadFinished(void);
    void waitUploadFinished(void);
    QNetworkReply::NetworkError error(void);

private slots :
    void on_httpReadyRead();
    void on_downloadFinished();
    void on_uploadFinished();
    void on_downloadProgress(qint64 progressMaximum, qint64 progressValue);
    void on_uploadProgress(qint64 progressMaximum, qint64 progressValue);
    void on_error(QNetworkReply::NetworkError networkError);

private:
//    QNetworkAccessManager *accessManager1;
//    QNetworkAccessManager *accessManager2;
    QNetworkAccessManager *accessManager;
    QNetworkAccessManager *accessManagerUpload;
    QNetworkReply *reply;
    QFile *file;

    qint64 downloadProgressMaximum;
    qint64 downloadProgressValue;
    qint64 uploadProgressMaximum;
    qint64 uploadProgressValue;
    QNetworkReply::NetworkError networkError;

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
