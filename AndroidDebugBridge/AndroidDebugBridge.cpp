#include "AndroidDebugBridge.h"
#include <QCoreApplication>
#include <QDebug>
#include <QDir>

AndroidDebugBridge::AndroidDebugBridge(QProcess *parent) : QProcess(parent)
{
    executionPath = QDir::currentPath() + "/ADB/";  /* 工作目录+ADB */
    setWorkingDirectory(executionPath);
    devicesList = QStringList();
    adberror = UnknownError;
}
AndroidDebugBridge::~AndroidDebugBridge()
{
}

/**
 * @brief AndroidDebugBridge::path 获取ADB命令执行的绝对路径
 * @return ADB命令执行的绝对路径
 */
const QString &AndroidDebugBridge::path(void)
{
    return executionPath;
}

/**
 * @brief AndroidDebugBridge::setPath 设置ADB命令执行的绝对路径
 * @param path ADB命令执行的绝对路径
 */
void AndroidDebugBridge::setPath(const QString &path)
{
    executionPath = path;
    setWorkingDirectory(executionPath);
}

/**
 * @brief AndroidDebugBridge::isExecutable 测试adb命令是否可以被执行
 * @return true/false 错误代码在error中返回
 */
bool AndroidDebugBridge::isExecutable()
{
    setProgram(executionPath + "adb_executable.cmd");
    setArguments(QStringList());
    start();
    waitForStarted();
    waitForFinished(1000);

    adbDebug();
    errorAnalysis();

    if (adberror != AndroidDebugBridge::FailedToStart) {
        return true;
    }
    return false;
}

/**
 * @brief AndroidDebugBridge::searchDevice 搜索ADB设备
 */
const QStringList &AndroidDebugBridge::searchDevice(void)
{
    setProgram(executionPath + "adb_devices.cmd");
    setArguments(QStringList());
    start();
    waitForStarted();
    waitForFinished(20000);

    adbDebug();
    errorAnalysis();

    devicesList.clear();
    if (adberror == AndroidDebugBridge::NoError) {
        QStringList listTemp = QString(readAllOutput).split("\r\n");
        for (int i = 0; i < listTemp.length(); i++ ) {
            if (listTemp.at(i).indexOf("\tdevice") >= 0) {
                devicesList.append(listTemp[i].remove("\tdevice"));
            }
        }
    }

    return devicesList;
}

/**
 * @brief AndroidDebugBridge::getDevicesList
 * @return 返回ADB设备列表
 *      注意：返回的设备列表是上一次搜索完成后的列表，
 *           如果需要最新的设备列表，请重新搜索设备列表
 */
const QStringList &AndroidDebugBridge::getDevicesList(void)
{
    return devicesList;
}

/**
 * @brief AndroidDebugBridge::entryBootloader 使设备进入boot loader模式 非阻塞模式
 * @param device 设备号
 * @return true/false
 */
void AndroidDebugBridge::entryBootloader(const QString &device)
{
    setProgram(executionPath + "adb_reboot_bootloader.cmd");
    setArguments(QStringList() << device);
    start();
    waitForStarted();
    waitForFinished(20000);

    adbDebug();
    errorAnalysis();
}

void AndroidDebugBridge::errorAnalysis(void)
{
    if ((error() == QProcess::UnknownError) && (exitCode == 0) && (exitStatus == QProcess::NormalExit)) {
        adberror = AndroidDebugBridge::NoError;
    } else if (error() == QProcess::Timedout) {
        adberror = AndroidDebugBridge::Timedout;
    } else if (error() == QProcess::FailedToStart) {
        adberror = AndroidDebugBridge::FailedToStart;
    } else if (exitCode) {
        adberror = AndroidDebugBridge::UnknownCommand;
    } else {
        adberror = AndroidDebugBridge::UnknownError;
    }

    qDebug() << "adberror = " << adberror << endl << endl;
}

void AndroidDebugBridge::adbDebug(void)
{
    exitCode = QProcess::exitCode();
    exitStatus = QProcess::exitStatus();
    readAllOutput = readAllStandardOutput();
    readAllError  = readAllStandardError();
    qDebug() << "error = " << error();
    qDebug() << "exitCode = " << QString::number(exitCode);
    qDebug() << "exitStatus = " << exitStatus;
    qDebug() << "readAllOutput = " << readAllOutput;
    qDebug() << "readAllError = " << readAllError;
}

AndroidDebugBridge::ADBError AndroidDebugBridge::adbError(void)
{
    return adberror;
}
