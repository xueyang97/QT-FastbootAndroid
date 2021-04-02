#include "AndroidDebugBridge.h"
#include <QCoreApplication>
#include <QDebug>
#include <QDir>

AndroidDebugBridge::AndroidDebugBridge(QObject *parent) : QProcess(parent)
{
    adbcmd = new QProcess(nullptr);
    executionPath = QDir::currentPath() + "/ADB";  /* 工作目录+ADB */
    adbcmd->setProgram(executionPath + "/adb.exe");
    devicesList = QStringList();

    connect(adbcmd,SIGNAL(finished(int,QProcess::ExitStatus)), this,SLOT(on_finshed(int,QProcess::ExitStatus)));
    adberror = UnknownError;
    ststus = NoCommand;
}
AndroidDebugBridge::~AndroidDebugBridge()
{
    delete adbcmd;
}





/**
 * @brief AndroidDebugBridge::path 获取ADB命令执行的绝对路径
 * @return ADB命令执行的绝对路径
 */
QString AndroidDebugBridge::path(void)
{
    return executionPath;
}

/**
 * @brief AndroidDebugBridge::setPath 设置ADB命令执行的绝对路径
 * @param path ADB命令执行的绝对路径
 */
void AndroidDebugBridge::setPath(QString &path)
{
    executionPath = path;
    adbcmd->setProgram(executionPath + "/adb.exe");
//    adbcmd->setWorkingDirectory(executionPath);
}

/**
 * @brief AndroidDebugBridge::isAdbRun 测试adb命令是否可以被执行
 * @return true/false 错误代码在error中返回
 */
bool AndroidDebugBridge::isAdbRun()
{
    adbcmd->setArguments(QStringList() << "version");
    ststus = Version;
    adbcmd->start();
    adbcmd->waitForStarted();
    adbcmd->waitForFinished();
    // adbError = (AndroidDebugBridge::ADBError)adbcmd->error();
    if (adbcmd->error() != QProcess::UnknownError) {
        return false;
    }
    return true;
}

/**
 * @brief AndroidDebugBridge::searchDevice 搜索ADB设备，为了防止搜索过程中出现阻塞，使用信号槽更新设备列表
 */
void AndroidDebugBridge::searchDevice(void)
{
    adbcmd->setArguments(QStringList() << "devices");
    ststus = SearchDevice;
    adbcmd->start();
}

/**
 * @brief AndroidDebugBridge::waitSearchDeviceFinshed
 *        使用阻塞等待的方式搜索ADB设备 阻塞等待方式可以直接获取设备列表
 */
QStringList AndroidDebugBridge::waitSearchDevice(void)
{
    adbcmd->setArguments(QStringList() << "devices");
    ststus = SearchDevice;
    adbcmd->start();
    adbcmd->waitForFinished();
    return devicesList;
}

/**
 * @brief AndroidDebugBridge::getDevicesList
 * @return 返回ADB设备列表
 *      注意：返回的设备列表是上一次搜索完成后的列表，
 *           如果需要最新的设备列表，请重新搜索设备列表
 */
QStringList AndroidDebugBridge::getDevicesList(void)
{
    return devicesList;
}

/**
 * @brief AndroidDebugBridge::entryBootloader 使设备进入boot loader模式 非阻塞模式
 * @param device 设备号
 * @return true/false
 */
bool AndroidDebugBridge::entryBootloader(QString device)
{
    QStringList Arguments = QStringList();
    Arguments.clear();
    Arguments << "-s" << device << "reboot" << "bootloader";
    adbcmd->setArguments(Arguments);
    ststus = RebootBootloader;
    adbcmd->start();
    return true;
}

/**
 * @brief AndroidDebugBridge::on_finshed
 */
void AndroidDebugBridge::on_finshed(int exitCode, QProcess::ExitStatus exitStatus)
{
    QString readAllOutput = adbcmd->readAllStandardOutput();
    QString readAllError  = adbcmd->readAllStandardError();
    adberror = NoError;
    QStringList argum = adbcmd->arguments();

    if (readAllError.indexOf("error: cannot connect to daemon") != -1) {
        adberror = DaemonError;
        goto out;
    }

    switch ((int)ststus) {
    case Version: break;
    case SearchDevice:
        devicesList.clear();
        if ((exitCode == 0) && (exitStatus == QProcess::NormalExit)) {   /* 进程执行成功 */
            QStringList listTemp = QString(readAllOutput).split("\r\n");
            for (int i = 0; i < listTemp.length(); i++ ) {
                if (listTemp.at(i).indexOf("\tdevice") >= 0) {
                    devicesList.append(listTemp[i].remove("\tdevice"));
                }
            }
        } else {
            adberror = NotFoundDevices; //未搜寻到设备
        }
    break;
    case RebootBootloader:
        if (readAllError.indexOf("error") != -1) {
            adberror = NotFoundDevices;
        }
    break;
    }

out:
    ststus = NoCommand;
    emit adbFinished(exitCode, exitStatus);
}

AndroidDebugBridge::ADBError AndroidDebugBridge::adbError(void)
{
    return adberror;
}
