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
        DaemonError,    // cannot connect to daemon
        NotFoundDevices, //没有找到设备
    };
    Q_ENUM(ADBError);

    explicit AndroidDebugBridge(QObject *parent = nullptr);
    ~AndroidDebugBridge();

    QString path(void);
    void setPath(QString &path);
    bool isAdbRun(void);
    void searchDevice(void);
    QStringList waitSearchDevice(void);
    QStringList getDevicesList(void);

    bool entryBootloader(QString device);
    AndroidDebugBridge::ADBError adbError(void);


private slots:
    void on_finshed(int exitCode, QProcess::ExitStatus exitStatus);

private:
    QString     executionPath;      /*!< adb.exe 程序执行路径 */
    QProcess    *adbcmd;
    QStringList devicesList;        /*!< adb 设备列表 */
    ADBError    adberror;
    ADBCDM      ststus;

signals:
    void adbFinished(int exitCode, QProcess::ExitStatus exitStatus);                        /* 搜索设备完成信号 */

};

#endif // ANDROIDDEBUGBRIDGE_H
