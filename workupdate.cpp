#include "workupdate.h"
#include "common.h"
#include <QMetaEnum>
#include <QNetworkReply>

QString printErrorCode(const QString &info, int enumration)
{
    return info + QString("\t错误代码为 : %1").arg(fastboot_enum2string(enumration)) + "\n";
}

QString adbPrintErrorCode(const QString &info, int enumration)
{
    return info + QString("\t错误代码为 : %1").arg(adb_enum2string(enumration)) + "\n";
}

WorkUpdate::WorkUpdate(QThread *parent) : QThread(parent)
{
    standardOutput.clear();
    workMode = WorkUpdate::NotMode;

    cacheFilename     = UPDATA_CACHE_DEFAULT_FILENAME   ;
    abootFilename     = UPDATA_ABOOT_DEFAULT_FILENAME   ;
    bootFilename      = UPDATA_BOOT_DEFAULT_FILENAME    ;
    persistFilename   = UPDATA_PERSIST_DEFAULT_FILENAME ;
    recoveryFilename  = UPDATA_RECOVERY_DEFAULT_FILENAME;
    systemFilename    = UPDATA_SYSTEM_DEFAULT_FILENAME  ;
    userdataFilename  = UPDATA_USERDATA_DEFAULT_FILENAME;
    DDRFilename       = UPDATA_DDR_DEFAULT_FILENAME     ;
    splashFilename    = UPDATA_SPLASH_DEFAULT_FILENAME  ;

    isUpdataSuccess = false;
    isWorkRunning = false;
}

void WorkUpdate::run(void)
{
    AndroidDebugBridge *adb = new AndroidDebugBridge(0);
    Fastboot *fastboot = new Fastboot(0);

    QString fastbootDevice;
    QStringList fastbootDeviceList;

    standardOutput.clear();
    isUpdataSuccess = true;
    isWorkRunning = true;

    /* Android设备进入bootloader模式  不论成功 */
    setStandardOutput("adb进入bootloader模式...\n");
    adb->entryBootloader(adbDevice);
    QThread::sleep(6);  //需要等待设备已经进入boot loader模式
    if (adb->adbError() != AndroidDebugBridge::NoError) {
        setStandardOutput(adbPrintErrorCode("adb进入bootloader失败", adb->adbError()));
    } else {
        setStandardOutput("adb进入bootloader模式成功\n");
    }

    /* 搜索fastboot设备 */
    fastbootDeviceList = fastboot->searchDevice();
    if (fastbootDeviceList.isEmpty() == true) {
        setStandardOutput("未搜索到fastboot设备，fastboot下载失败\n");
        isUpdataSuccess = false;
        goto out;
    }
    fastbootDevice = fastbootDeviceList.at(0);

    /* fast boot升级cache分区 */
    if ((workMode & CacheMode) == CacheMode) {
        setStandardOutput("Android设备正在升级cache分区...\n");
        fastboot->flashCacheDownload(fastbootDevice, cacheFilename);
        if (fastboot->fastbootError() != Fastboot::NoError) {
            setStandardOutput(printErrorCode("fastboot升级cache分区失败", fastboot->fastbootError()));
            isUpdataSuccess = false;
            // goto out;
        } else {
            setStandardOutput(QString("fastboot升级cache分区成功\n"));
        }
    }

    /* fast boot升级aboot分区 */
    if ((workMode & AbootMode) == AbootMode) {
        setStandardOutput("Android设备正在升级aboot分区...\n");
        fastboot->flashAbootDownload(fastbootDevice, abootFilename);
        if (fastboot->fastbootError() != Fastboot::NoError) {
            setStandardOutput(printErrorCode("fastboot升级aboot分区失败", fastboot->fastbootError()));
            isUpdataSuccess = false;
            // goto out;
        } else {
            setStandardOutput(QString("fastboot升级aboot分区成功\n"));
        }
    }

    /* fast boot升级boot分区 */
    if ((workMode & BootMode) == BootMode) {
        setStandardOutput("Android设备正在升级boot分区...\n");
        fastboot->flashBootDownload(fastbootDevice,bootFilename);
        if (fastboot->fastbootError() != Fastboot::NoError) {
            setStandardOutput(printErrorCode("fastboot升级boot分区失败", fastboot->fastbootError()));
            isUpdataSuccess = false;
            // goto out;
        } else {
            setStandardOutput(QString("fastboot升级boot分区成功\n"));
        }
    }

    /* fast boot升级persist分区 */
    if ((workMode & PersistMode) == PersistMode) {
        setStandardOutput("Android设备正在升级persist分区...\n");
        fastboot->flashPersistDownload(fastbootDevice,persistFilename);
        if (fastboot->fastbootError() != Fastboot::NoError) {
            setStandardOutput(printErrorCode("fastboot升级persist分区失败", fastboot->fastbootError()));
            isUpdataSuccess = false;
            // goto out;
        } else {
            setStandardOutput(QString("fastboot升级persist分区成功\n"));
        }
    }

    /* fast boot升级Recovery分区 */
    if ((workMode & RecoveryMode) == RecoveryMode) {
        setStandardOutput("Android设备正在升级recovery分区...\n");
        fastboot->flashRecoveryDownload(fastbootDevice,recoveryFilename);
        if (fastboot->fastbootError() != Fastboot::NoError) {
            setStandardOutput(printErrorCode("fastboot升级recovery分区失败", fastboot->fastbootError()));
            isUpdataSuccess = false;
            // goto out;
        } else {
            setStandardOutput(QString("fastboot升级recovery分区成功\n"));
        }
    }

    /* fast boot升级System分区 */
    if ((workMode & SystemMode) == SystemMode) {
        setStandardOutput("Android设备正在升级system分区...\n");
        fastboot->flashSystemDownload(fastbootDevice,systemFilename);
        if (fastboot->fastbootError() != Fastboot::NoError) {
            setStandardOutput(printErrorCode("fastboot升级system分区失败", fastboot->fastbootError()));
            isUpdataSuccess = false;
            // goto out;
        } else {
            setStandardOutput(QString("fastboot升级system分区成功\n"));
        }
    }

    /* fast boot升级Userdata分区 */
    if ((workMode & UserdataMode) == UserdataMode) {
        setStandardOutput("Android设备正在升级userdata分区...\n");
        fastboot->flashUserdataDownload(fastbootDevice,userdataFilename);
        if (fastboot->fastbootError() != Fastboot::NoError) {
            setStandardOutput(printErrorCode("fastboot升级userdata分区失败", fastboot->fastbootError()));
            isUpdataSuccess = false;
            // goto out;
        } else {
            setStandardOutput(QString("fastboot升级userdata分区成功\n"));
        }
    }

    /* fast boot升级DDR分区 */
    if ((workMode & DDRMode) == DDRMode) {
        setStandardOutput("Android设备正在升级DDR分区...\n\n");
        fastboot->flashDDRDownload(fastbootDevice,DDRFilename);
        if (fastboot->fastbootError() != Fastboot::NoError) {
            setStandardOutput(printErrorCode("fastboot升级DDR分区失败", fastboot->fastbootError()));
            isUpdataSuccess = false;
            // goto out;
        } else {
            setStandardOutput(QString("fastboot升级DDR分区成功\n"));
        }
    }

    /* fast boot升级splash分区 */
    if ((workMode & SplashMode) == SplashMode) {
        setStandardOutput("Android设备正在升级splash分区...\n\n");
        fastboot->flashSplashDownload(fastbootDevice,splashFilename);
        if (fastboot->fastbootError() != Fastboot::NoError) {
            setStandardOutput(printErrorCode("fastboot升级splash分区失败", fastboot->fastbootError()));
            isUpdataSuccess = false;
            // goto out;
        } else {
            setStandardOutput(QString("fastboot升级splash分区成功\n"));
        }
    }

    /* fast boot重启设备  此命令必须最后执行 */
    if ((workMode & RebootMode) == RebootMode) {
        setStandardOutput("Android设备正在重启设备...\n");
        fastboot->fsatbootReboot(fastbootDevice);
        if (fastboot->fastbootError() != Fastboot::NoError) {
            setStandardOutput(printErrorCode("fastboot重启设备失败", fastboot->fastbootError()));
            isUpdataSuccess = false;
            // goto out;
        } else {
            setStandardOutput(QString("fastboot重启设备成功\n"));
        }
    }


out:
    if (isUpdataSuccess) {
        setStandardOutput(QString("fastboot设备升级成功\n"));
    } else {
        setStandardOutput(QString("fastboot设备升级失败\n"));
//        /* 如果可以搜索到fast boot设备，就将设备重启退出 */
//        fastbootDeviceList = fastboot->searchDevice();
//        if (fastbootDeviceList.isEmpty() != true) {
//            fastboot->fsatbootReboot(fastboot->searchDevice().at(0));
//        }
//        setStandardOutput(QString("fastboot设备已重新启动\n"));
    }
    setStandardOutput(QString("\n\n"));

    delete adb;
    delete fastboot;
    isWorkRunning = false;
    emit workingFinished();
    quit();
}

/**
 * @brief WorkUpdate::setADBDevice
 * @param device 设置需要升级的Android设备
 */
void WorkUpdate::setADBDevice(const QString &device)
{
    adbDevice = device;
}

/**
 * @brief WorkUpdate::setWorkMode
 * @param mode 设置升级使用的工作模式
 */
void WorkUpdate::setWorkMode(unsigned int mode)
{
    workMode = mode;
}

void WorkUpdate::setWorkFileName(WorkUpdate::WorkMode mode, const QString &fileName)
{
    switch (int(mode)) {
    case CacheMode    : cacheFilename     = fileName;   break;
    case AbootMode    : abootFilename     = fileName;   break;
    case BootMode     : bootFilename      = fileName;   break;
    case PersistMode  : persistFilename   = fileName;   break;
    case RecoveryMode : recoveryFilename  = fileName;   break;
    case SystemMode   : systemFilename    = fileName;   break;
    case UserdataMode : userdataFilename  = fileName;   break;
    case DDRMode      : DDRFilename       = fileName;   break;
    case SplashMode   : splashFilename    = fileName;   break;
    }
}

QString WorkUpdate::readAllStandardOutput(void)
{
    QString OutputTemp = standardOutput;
    standardOutput.clear();
    return OutputTemp;
}

bool WorkUpdate::isSuccess(void)
{
    return isUpdataSuccess;
}
bool WorkUpdate::isRunning(void)
{
    return isWorkRunning;
}


/**
 * ===================================================================================
 * @brief private function
 * =====================================================================================
 */


void WorkUpdate::setStandardOutput(const QString &string)
{
    standardOutput.append(string);
    emit readyReadStandardOutput();
}






















SearchDevice::SearchDevice(QThread *parent) : QThread(parent)
{
    devices = QStringList();
}

void SearchDevice::run(void)
{
    AndroidDebugBridge *adb = new AndroidDebugBridge(0);
    qDebug() << "搜索设备中...";
    devices = adb->searchDevice();
    if (devices.isEmpty() == true) {
        qDebug() << "未搜索到设备";
    } else {
        qDebug() << "搜索到的设备 :\t" << devices;
    }

    emit searchFinished();
    quit();
}

const QStringList &SearchDevice::getDevices(void)
{
    return devices;
}






