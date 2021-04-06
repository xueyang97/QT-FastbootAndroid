#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QMessageBox>
#include <QFile>
#include <QFileDialog>
#include <QCoreApplication>

#include "httpcommunication.h"
#include "AndroidDebugBridge/AndroidDebugBridge.h"
#include "Fastboot/Fastboot.h"
#include "workupdate.h"
#include <QThread>


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
    void closeEvent( QCloseEvent * event );

private slots:
    void updateDataReadProgress(qint64 a,qint64 b);
    void updateDataWritedProgress(qint64 a,qint64 b);
    void httpFinished();
    void on_uploadFinished();
    void on_httpError(QNetworkReply::NetworkError, HttpCommunication::HttpError);

    void on_workReadyReadStandardOutput(void);

    void on_pushButtonLoad_clicked();
    void on_pushButtonExit_clicked();
    void on_textEditInformation_textChanged();
    void on_pushButton_clicked();
    void on_pushButtonRun_clicked();

private:
    Ui::MainWindow *ui;
    HttpCommunication *http;
    QString fileName;

    AndroidDebugBridge *adb;
    Fastboot           *fastboot;

    WorkUpdate          *workUpdate;

    void ui_editShowInfo(const QString & info);
    void ui_editShowClear(void);
};
#endif // MAINWINDOW_H
