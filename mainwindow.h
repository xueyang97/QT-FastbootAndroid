#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QUrl>
#include <QFileInfo>
#include <QDebug>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QMovie>
#include <QThread>
#include <QMessageBox>
#include "httpcommunication.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void startRequest(QUrl url);
    void showPicture(QString fileName);

private slots:
    void updateDataReadProgress(qint64 a,qint64 b);
    void on_pushButtonLoad_clicked();
    void httpFinished();
    void on_httpError(QNetworkReply::NetworkError, HttpCommunication::HttpError);

private:
    Ui::MainWindow *ui;
    HttpCommunication *http;
//    QFile *file;
//    QNetworkAccessManager *manager;
//    QNetworkReply *reply;
//    QByteArray temp;
//    QUrl url;
};
#endif // MAINWINDOW_H
