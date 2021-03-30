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

private slots:
    void updateDataReadProgress(qint64 a,qint64 b);
    void on_pushButtonLoad_clicked();
    void httpReadyRead();
    void httpFinished();
    void replyFinished(QNetworkReply *reply);

private:
    Ui::MainWindow *ui;
    QNetworkAccessManager *manager;
    QNetworkReply *reply;
    QByteArray temp;
    QUrl url;
    QFile *file;
};
#endif // MAINWINDOW_H
