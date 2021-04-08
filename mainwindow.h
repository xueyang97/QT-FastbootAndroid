#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QMessageBox>
#include <QFile>
#include <QFileDialog>
#include <QCoreApplication>
#include <QDir>

#include "httpcommunication.h"
#include "AndroidDebugBridge/AndroidDebugBridge.h"
#include "Fastboot/Fastboot.h"
#include "workupdate.h"
#include "httpServer/httpserver.h"
#include "common.h"


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
    void on_workReadyReadStandardOutput(void);
    void on_workingFinished(void);
    void on_searchFinished(void);
    void on_httpFinished(void);

    void on_textEditInformation_textChanged();
    void on_pushButton_clicked();
    void on_pushButtonRun_clicked();
    void on_pushButtonFolder_clicked();
    void on_comboBoxFastbootMode_currentIndexChanged(int);

    void on_checkBoxSplash_stateChanged(int arg1);

    void on_checkBoxIsUser_stateChanged(int arg1);

private:
    Ui::MainWindow      *ui;
    HttpServer          *http;
    WorkUpdate          *workUpdate;
    SearchDevice        *adbDevices;
    QFile               *logFile;
    QString             folderPath;
    unsigned int        workMode;

    QStringList cacheFilename    ;
    QStringList abootFilename    ;
    QStringList bootFilename     ;
    QStringList persistFilename  ;
    QStringList recoveryFilename ;
    QStringList systemFilename   ;
    QStringList userdataFilename ;
    QStringList DDRFilename      ;
    QStringList splashFilename   ;

    void ui_editShowInfo(const QString & info);
    void ui_editShowClear(void);
    void setFastbootModeCheckBoxEnable(bool enable);
    void setWorkFileName(void);

    httpServer *https;
};

QStringList searchFileName(const QString &folderPath, const QString &files);

#endif // MAINWINDOW_H
