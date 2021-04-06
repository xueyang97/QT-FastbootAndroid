#include "workupdate.h"

WorkUpdate::WorkUpdate() : QThread(0)
{
    standardOutput.clear();
}

void WorkUpdate::run(void)
{
    AndroidDebugBridge *adb = new AndroidDebugBridge(0);
    Fastboot *fastboot = new Fastboot(0);

    standardOutput.clear();

    setStandardOutput("adb进入bootloader模式...\n");
    adb->entryBootloader(adbDevice);
    QThread::sleep(6);
    if (adb->adbError() != AndroidDebugBridge::NoError) {
        setStandardOutput(QString("adb进入bootloader失败 错误代码 : %1\n").arg(adb->adbError()));
        quit();
    }
    setStandardOutput("adb进入bootloader模式成功\n");

    const QStringList &fastbootDeviceList = fastboot->searchDevice();
    if (fastbootDeviceList.isEmpty() == true) {
        setStandardOutput("未搜索到fastboot设备，fastboot下载失败\n");
        quit();
    }
    QString fastbootDevice = fastbootDeviceList.at(0);

    setStandardOutput("正在升级boot分区...\n");
    fastboot->flashBootDownload(fastbootDevice,"E:\\Android\\SW_8953_android9_V01_20210120_HbSdk_factory\\SW_8953_android9_V01_20210120_HbSdk_factory\\boot.img");
    if (fastboot->fsatbootError() != Fastboot::NoError) {
        setStandardOutput(QString("fastboot升级boot分区失败 错误代码 : %1\n").arg(fastboot->fsatbootError()));
        quit();
    }
    setStandardOutput(QString("fastboot升级boot分区成功\n"));

    setStandardOutput("正在重启设备...\n");
    fastboot->fsatbootReboot(fastbootDevice);
    if (fastboot->fsatbootError() != Fastboot::NoError) {
        setStandardOutput(QString("fastboot重启设备失败 错误代码 : %1\n").arg(fastboot->fsatbootError()));
        quit();
    }
    setStandardOutput(QString("fastboot重启设备成功\n"));
    setStandardOutput(QString("fastboot设备升级成功\n"));

    quit();
}

void WorkUpdate::setADBDevice(const QString &device)
{
    adbDevice = device;
}

QString WorkUpdate::readAllStandardOutput(void)
{
    QString OutputTemp = standardOutput;
    standardOutput.clear();
    return OutputTemp;
}

void WorkUpdate::setStandardOutput(const QString &string)
{
    standardOutput.append(string);
    emit readyReadStandardOutput();
}
