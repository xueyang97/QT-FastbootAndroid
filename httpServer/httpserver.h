#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrl>
#include <QFile>

class httpServer : public QObject
{
    Q_OBJECT
public:
    explicit httpServer(QObject *parent = nullptr);
    void httpDownloader(const QUrl &url, const QFile &download);
    void httpUploader(const QUrl &url, const QFile &upload, const QString &php);

private slots:
    void on_dataAvailable(void);
    void on_downloadfinished(void);

private:
    QNetworkAccessManager   *_accessManager;
    QNetworkRequest         *_request;
    QNetworkReply           *_reply;

    QFile                   *downloadFile;
    bool                    downloadFinished;

    QNetworkReply::NetworkError networkError;



signals:

};

#endif // HTTPSERVER_H
