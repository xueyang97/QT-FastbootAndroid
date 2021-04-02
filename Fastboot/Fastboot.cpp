#include "Fastboot/Fastboot.h"
#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QThread>

Fastboot::Fastboot(QProcess *parent) : QProcess(parent)
{
    executionPath = QDir::currentPath() + "/ADB";  /* 工作目录+ADB */
    setProgram(executionPath + "/fastboot.exe");
    isFastRunning = false;
    ststus = NoCommand;
    devicesList = QStringList();
    fasterror = NoError;

    connect(this,SIGNAL(finished(int,QProcess::ExitStatus)),this,SLOT(on_finished(int,QProcess::ExitStatus)));
    connect(this,SIGNAL(readyReadStandardOutput()),this,SLOT(on_readyReadStandardOutput()));
    connect(this,SIGNAL(readyReadStandardError()),this,SLOT(on_readyReadStandardError()));
}

Fastboot::~Fastboot()
{
}

void Fastboot::searchDevice(void)
{
    if (!isFastRunning) {
        isFastRunning = true;
        ststus = SearchDevice;
        setArguments(QStringList() << "devices");
        start();
    } else {
        qDebug() << "fastboot 程序正在运行";
    }
}

void Fastboot::fsatbootReboot(void)
{
    if (!isFastRunning) {
        isFastRunning = true;
        ststus = FsatbootReboot;
        setArguments(QStringList() << "reboot");
        start();
    } else {
        qDebug() << "fastboot 程序正在运行";
    }
}

bool Fastboot::isRunning(void)
{
    return isFastRunning;
}

void flashDownload(Fastboot::FastbootCDM flash, QString &filename)
{

}

void Fastboot::on_readyReadStandardOutput()
{
//    qDebug() << readAllStandardOutput();
//    qDebug() << readAll();
}
void Fastboot::on_readyReadStandardError()
{
//    qDebug() << readAllStandardError();
}

void Fastboot::on_finished(int exitCode, QProcess::ExitStatus exitStatus)
{
    QString readAllOutput = readAllStandardOutput();
    QString readAllError  = readAllStandardError();
    QStringList argum = arguments();

    switch ((int)ststus) {
    case SearchDevice: break;
    case FsatbootReboot:
        if (readAllOutput.indexOf("Finished")) {
            fasterror = NoError;
        } else {
            fasterror = UnknownError;
        }
    break;
    case FlashBoot:
        qDebug() << readAllOutput << readAllError << endl;
    break;
    }

    isFastRunning = false;
    ststus = NoCommand;
    emit fastbootFinished(exitCode,exitStatus);
}

Fastboot::FastbootError Fastboot::fsatbootError(void)
{
    return fasterror;
}
