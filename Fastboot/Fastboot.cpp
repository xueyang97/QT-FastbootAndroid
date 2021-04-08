#include "Fastboot/Fastboot.h"
#include <QDebug>
#include <QDir>

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

void Fastboot::fsatbootReboot(const QString &device)
{
    setProgram(executionPath + "fastboot_reboot.cmd");
    setArguments(QStringList() << device);
    start();
    waitForStarted();
    waitForFinished(FASTBOOT_REBOOT_TIMEDOUT);

    fastbootDebug();
    errorAnalysis();

    if ((fasterror == Fastboot::NoError) && (readAllError.indexOf("Finished") == -1)) {
        /* 执行成功，但是错误返回信息列表中未出现完成信号，设置为未知错误 */
        fasterror = Fastboot::UnknownError;
    }
}

void Fastboot::flashCacheDownload(const QString &device, const QString &filename)
{
    setProgram(executionPath + "fastboot_cache.cmd");
    setArguments(QStringList() << device << filename);
    start();
    waitForStarted();
    waitForFinished(FASTBOOT_CACHE_TIMEDOUT);

    fastbootDebug();
    errorAnalysis();
}
void Fastboot::flashAbootDownload(const QString &device, const QString &filename)
{
    setProgram(executionPath + "fastboot_aboot.cmd");
    setArguments(QStringList() << device << filename);
    start();
    waitForStarted();
    waitForFinished(FASTBOOT_ABOOT_TIMEDOUT);

    fastbootDebug();
    errorAnalysis();
}
void Fastboot::flashBootDownload(const QString &device, const QString &filename)
{
    setProgram(executionPath + "fastboot_boot.cmd");
    setArguments(QStringList() << device << filename);
    start();
    waitForStarted();
    waitForFinished(FASTBOOT_BOOT_TIMEDOUT);

    fastbootDebug();
    errorAnalysis();
}
void Fastboot::flashPersistDownload(const QString &device, const QString &filename)
{
    setProgram(executionPath + "fastboot_persist.cmd");
    setArguments(QStringList() << device << filename);
    start();
    waitForStarted();
    waitForFinished(FASTBOOT_PERSIST_TIMEDOUT);

    fastbootDebug();
    errorAnalysis();
}
void Fastboot::flashRecoveryDownload(const QString &device, const QString &filename)
{
    setProgram(executionPath + "fastboot_recovery.cmd");
    setArguments(QStringList() << device << filename);
    start();
    waitForStarted();
    waitForFinished(FASTBOOT_RECOVERY_TIMEDOUT);

    fastbootDebug();
    errorAnalysis();
}
void Fastboot::flashSystemDownload(const QString &device, const QString &filename)
{
    setProgram(executionPath + "fastboot_system.cmd");
    setArguments(QStringList() << device << filename);
    start();
    waitForStarted();
    waitForFinished(FASTBOOT_SYSTEM_TIMEDOUT);

    fastbootDebug();
    errorAnalysis();
}
void Fastboot::flashUserdataDownload(const QString &device, const QString &filename)
{
    setProgram(executionPath + "fastboot_userdata.cmd");
    setArguments(QStringList() << device << filename);
    start();
    waitForStarted();
    waitForFinished(FASTBOOT_USERDATA_TIMEDOUT);

    fastbootDebug();
    errorAnalysis();
}
void Fastboot::flashDDRDownload(const QString &device, const QString &filename)
{
    setProgram(executionPath + "fastboot_ddr.cmd");
    setArguments(QStringList() << device << filename);
    start();
    waitForStarted();
    waitForFinished(FASTBOOT_DDR_TIMEDOUT);

    fastbootDebug();
    errorAnalysis();
}
void Fastboot::flashSplashDownload(const QString &device, const QString &filename)
{
    setProgram(executionPath + "fastboot_splash.cmd");
    setArguments(QStringList() << device << filename);
    start();
    waitForStarted();
    waitForFinished(FASTBOOT_DDR_TIMEDOUT);

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

/**
 * @brief Fastboot::fsatbootError
 * @return 返回fastboot命令错误信息
 */
Fastboot::FastbootError Fastboot::fastbootError(void)
{
    return fasterror;
}

