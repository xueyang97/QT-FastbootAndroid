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
    };
    Q_ENUM(HttpError)

    HttpCommunication();
    ~HttpCommunication();

    void httpDownload(QString urlSpec, QString fileName);
    HttpError getError();

private slots :
    void on_httpReadyRead();
    void on_httpFinished();
    void on_downloadProgress(qint64 progressMaximum, qint64 progressValue);
    void on_error(QNetworkReply::NetworkError networkError);

private:
    QNetworkAccessManager *manager;
    QNetworkReply *reply;
    QFile *file;

signals :
    void downloadProgress(qint64 progressMaximum, qint64 progressValue);
    void downloadFinished();
    void httpError(QNetworkReply::NetworkError networkError, HttpCommunication::HttpError http_error);
};

#endif // HTTPCOMMUNICATION_H
