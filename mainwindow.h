#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QMovie>
#include <QMessageBox>
#include "httpcommunication.h"
#include <QProcess>
#include <QFile>
#include <QFileDialog>
#include <QCoreApplication>
#include "AndroidDebugBridge/AndroidDebugBridge.h"
#include "Fastboot/Fastboot.h"


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
    void closeEvent( QCloseEvent * event );

private slots:
    void updateDataReadProgress(qint64 a,qint64 b);
    void updateDataWritedProgress(qint64 a,qint64 b);
    void on_pushButtonLoad_clicked();
    void httpFinished();
    void on_uploadFinished();
    void on_httpError(QNetworkReply::NetworkError, HttpCommunication::HttpError);

    void on_CmdFinished(int);
//    void on_CmdReadyReadStandardOutput();
//    void on_CmdReadyReadStandardError();

    void on_pushButtonExit_clicked();
    void on_textEditInformation_textChanged();
    void on_pushButton_clicked();
    void on_pushButtonRun_clicked();

    void on_fastbootFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void on_adbFinished(int exitCode, QProcess::ExitStatus exitStatus);

private:
    Ui::MainWindow *ui;
    HttpCommunication *http;
    QProcess *cmd;
    QString fileName;
    int a;

    AndroidDebugBridge *adb;
    AndroidDebugBridge::ADBCDM adbcmd;
    Fastboot           *fastboot;
};
#endif // MAINWINDOW_H
