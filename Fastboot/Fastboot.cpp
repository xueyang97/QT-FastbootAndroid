#include "Fastboot/Fastboot.h"
#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QThread>

Fastboot::Fastboot(QProcess *parent) : QProcess(parent)
{
    executionPath = QDir::currentPath() + "/ADB/";  /* 工作目录+ADB */
    setWorkingDirectory(executionPath);
    devicesList = QStringList();
    fasterror = NoError;
}

Fastboot::~Fastboot()
{
}

/**
 * @brief Fastboot::isExecutable 测试 fastboot 命令是否可以被执行
 * @return
 */
bool Fastboot::isExecutable(void)
{
    setProgram(executionPath + "fastboot_executable.cmd");
    setArguments(QStringList());
    start();
    waitForStarted();
    waitForFinished(1000);

    fastbootDebug();
    errorAnalysis();

    if (fasterror != Fastboot::FailedToStart) {
        return true;
    }
    return false;
}

/**
 * @brief Fastboot::searchDevice 搜索 fastboot 设备
 * @return
 */
const QStringList &Fastboot::searchDevice(void)
{
    setProgram(executionPath + "fastboot_devices.cmd");
    setArguments(QStringList());
    start();
    waitForStarted();
    waitForFinished(5000);

    fastbootDebug();
    errorAnalysis();

    devicesList.clear();
    if (fasterror == Fastboot::NoError) {
        QStringList listTemp = QString(readAllOutput).split("\r\n");
        for (int i = 0; i < listTemp.length(); i++ ) {
            if (listTemp.at(i).indexOf("\tfastboot") >= 0) {
                devicesList.append(listTemp[i].remove("\tfastboot"));
            }
        }
    }

    return devicesList;
}

void Fastboot::Fastboot::fsatbootReboot(const QString &device)
{
    setProgram(executionPath + "fastboot_reboot.cmd");
    setArguments(QStringList() << device);
    start();
    waitForStarted();
    waitForFinished(120000);

    fastbootDebug();
    errorAnalysis();

    if ((fasterror == Fastboot::NoError) && (readAllError.indexOf("Finished") == -1)) {
        /* 执行成功，但是错误返回信息列表中未出现完成信号，设置为未知错误 */
        fasterror = Fastboot::UnknownError;
    }
}

void Fastboot::flashBootDownload(const QString &device, const QString &filename)
{
    setProgram(executionPath + "fastboot_boot.cmd");
    setArguments(QStringList() << device << filename);
    start();
    waitForStarted();
    waitForFinished(30000);

    fastbootDebug();
    errorAnalysis();
}

/**
 * @brief Fastboot::errorAnalysis fastboot 错误分析处理
 */
void Fastboot::errorAnalysis(void)
{
    if ((error() == QProcess::UnknownError) && (exitCode == 0) && (exitStatus == QProcess::NormalExit)) {
        fasterror = Fastboot::NoError;
    } else if (error() == QProcess::Timedout) {
        fasterror = Fastboot::Timedout;
    } else if (error() == QProcess::FailedToStart) {
        fasterror = Fastboot::FailedToStart;
    } else if (exitCode) {
        fasterror = Fastboot::UnknownCommand;
    } else {
        fasterror = Fastboot::UnknownError;
    }
}

void Fastboot::fastbootDebug(void)
{
    exitCode = QProcess::exitCode();
    exitStatus = QProcess::exitStatus();
    readAllOutput = readAllStandardOutput();
    readAllError  = readAllStandardError();
    qDebug() << "error = " << error();
    qDebug() << "exitCode = " << QString::number(exitCode);
    qDebug() << "exitStatus = " << exitStatus;
    qDebug() << "readAllOutput = " << readAllOutput;
    qDebug() << "readAllError = " << readAllError << endl << endl;
}

void Fastboot::flashDownload(Fastboot::FastbootCDM flash, QString filename)
{
//    if (!isFastRunning) {
//        isFastRunning = true;
//        switch ((int)flash) {
//        case FlashBoot:
//            ststus = FlashBoot;
//            setArguments(QStringList() << "boot" << "E:\\Android\\SW_8953_android9_V01_20210120_HbSdk_factory\\SW_8953_android9_V01_20210120_HbSdk_factory\\boot.img");
//        break;
//        default: isFastRunning = false; qDebug() << "不支持 fastboot flash 命令"; return;
//        }
//        QProcess::start();
//    } else {
//        qDebug() << "fastboot 程序正在运行";
//    }
}

//void Fastboot::on_finished()
//{
//    emit fastbootFinished(0,0);
//    QString readAllOutput = readAllStandardOutput();
//    QString readAllError  = readAllStandardError();
//    QStringList argum = arguments();

//    switch ((int)ststus) {
//    case SearchDevice: break;
//    case FsatbootReboot:
//        qDebug() << readAllOutput << readAllError << endl;
//        if (readAllOutput.indexOf("Finished")) {
//            fasterror = NoError;
//        } else {
//            fasterror = UnknownError;
//        }
//    break;
//    case FlashBoot:
//        qDebug() << readAllOutput << readAllError << endl;
//        if (readAllError.indexOf("error") != -1) {
//            fasterror = UnknownError;
//        } else {
//            fasterror = NoError;
//        }
//    break;
//    }

//    isFastRunning = false;
//    // ststus = NoCommand;
//    emit fastbootFinished(exitCode,exitStatus);
//}


//bool Fastboot::isRunning(void)
//{
//    return isFastRunning;
//}

Fastboot::FastbootError Fastboot::fsatbootError(void)
{
    return fasterror;
}

/**
 * @brief Fastboot::getProgram
 * @return 返回上一次执行的命令
 */
//Fastboot::FastbootCDM Fastboot::getProgram(void)
//{
//    return status;
//}

/**
 * @brief Fastboot::getProgram
 * @return 设置需要执行的命令
 */
//void Fastboot::setProgram(Fastboot::FastbootCDM cmd)
//{
//    status = cmd;
//}
