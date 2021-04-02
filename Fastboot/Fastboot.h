#ifndef FASTBOOT_H
#define FASTBOOT_H

#include <QProcess>
#include <QString>
#include <QObject>

class Fastboot : public QProcess
{
    Q_OBJECT
public:
    enum FastbootCDM {
        NoCommand,
        SearchDevice,
        FsatbootReboot,

        FlashBoot,
    };
    Q_ENUM(FastbootCDM)

    enum FastbootError {
        NoError,
        UnknownError,   //未知错误
    };
    Q_ENUM(FastbootError)

    Fastboot(QProcess *parent = nullptr);
    ~Fastboot();

    void searchDevice(void);
    void fsatbootReboot(void);
    bool isRunning(void);
    void flashDownload(Fastboot::FastbootCDM, QString &);
    Fastboot::FastbootError fsatbootError(void);

private slots:
    void on_finished(int exitCode, QProcess::ExitStatus exitStatus);
    void on_readyReadStandardOutput();
    void on_readyReadStandardError();

private:
    QString     executionPath;      /*!< fsatboot.exe 程序执行路径 */
    bool        isFastRunning;      /*!< fsatboot 进程是否正在执行 */
    FastbootCDM ststus;
    QStringList devicesList;        /*!< fsatboot 设备列表 */
    FastbootError fasterror;


signals:
    void fastbootFinished(int exitCode, QProcess::ExitStatus exitStatus);
};

#endif // FASTBOOT_H
