#ifndef ANDROIDDEBUGBRIDGE_H
#define ANDROIDDEBUGBRIDGE_H

#include <QProcess>
#include <QCoreApplication>

class AndroidDebugBridge : public QProcess
{
    Q_OBJECT
public:
    enum ADBCDM {
        NoCommand,
        SearchDevice,
        RebootBootloader,
        Version,
    };
    Q_ENUM(ADBCDM);

    enum ADBError {
        NoError,
        UnknownError,
        Timedout,           // 命令执行超时
        FailedToStart,      // 命令未启动 可能没有找到 ADB 命令
        UnknownCommand,     // 未知的 ADB 命令
        DaemonError,        // cannot connect to daemon
        NotFoundDevices,    // 没有找到设备
    };
    Q_ENUM(ADBError);

    explicit AndroidDebugBridge(QProcess *parent = nullptr);
    ~AndroidDebugBridge();

    const QString &path(void);
    void setPath(const QString &path);

    bool isExecutable(void);

    const QStringList &searchDevice(void);
    const QStringList &getDevicesList(void);

    void entryBootloader(const QString &device);

    AndroidDebugBridge::ADBError adbError(void);

private:
    QString     executionPath;      /*!< adb.exe 程序执行路径 */
    QStringList devicesList;        /*!< adb 设备列表 */
    ADBError    adberror;

    int exitCode;
    QProcess::ExitStatus exitStatus;
    QString readAllOutput;
    QString readAllError;

    void adbDebug(void);
    void errorAnalysis(void);

};

#endif // ANDROIDDEBUGBRIDGE_H
