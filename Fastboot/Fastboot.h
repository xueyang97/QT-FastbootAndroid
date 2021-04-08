#ifndef FASTBOOT_H
#define FASTBOOT_H

/*! fast boot升级命令超时时间设定 */
#define FASTBOOT_REBOOT_TIMEDOUT        (10 * 1000)
#define FASTBOOT_CACHE_TIMEDOUT         (10 * 1000)
#define FASTBOOT_ABOOT_TIMEDOUT         (10 * 1000)
#define FASTBOOT_BOOT_TIMEDOUT          (10 * 1000)
#define FASTBOOT_PERSIST_TIMEDOUT       (10 * 1000)
#define FASTBOOT_RECOVERY_TIMEDOUT      (10 * 1000)
#define FASTBOOT_SYSTEM_TIMEDOUT        (500 * 1000)
#define FASTBOOT_USERDATA_TIMEDOUT      (10 * 1000)
#define FASTBOOT_DDR_TIMEDOUT           (10 * 1000)
#define FASTBOOT_SPLASH_TIMEDOUT        (10 * 1000)

#include <QProcess>

class Fastboot : public QProcess
{
    Q_OBJECT
public:
    enum FastbootError {
        NoError,
        FailedToStart,  //命令未开始，未找到 fastboot 指令
        UnknownCommand, //未知的 fastboot 命令
        Timedout,       //命令执行超时
        UnknownError,   //未知错误
    };
    Q_ENUM(FastbootError)
    Fastboot(QProcess *parent = nullptr);
    ~Fastboot();

    bool isExecutable(void);
    const QStringList &searchDevice(void);

    void fsatbootReboot(const QString &device);
    void flashCacheDownload(const QString &device, const QString &filename);
    void flashAbootDownload(const QString &device, const QString &filename);
    void flashBootDownload(const QString &device, const QString &filename);
    void flashPersistDownload(const QString &device, const QString &filename);
    void flashRecoveryDownload(const QString &device, const QString &filename);
    void flashSystemDownload(const QString &device, const QString &filename);
    void flashUserdataDownload(const QString &device, const QString &filename);
    void flashDDRDownload(const QString &device, const QString &filename);
    void flashSplashDownload(const QString &device, const QString &filename);

    Fastboot::FastbootError fastbootError(void);

private:
    QString     executionPath;      /*!< fsatboot.exe 程序执行路径 */
    QStringList devicesList;        /*!< fsatboot 设备列表 */
    FastbootError fasterror;        /*!< fsatboot 执行错误信息 */

    void fastbootDebug(void);
    void errorAnalysis(void);

    int exitCode;
    QProcess::ExitStatus exitStatus;
    QString readAllOutput;
    QString readAllError;

};

#endif // FASTBOOT_H
