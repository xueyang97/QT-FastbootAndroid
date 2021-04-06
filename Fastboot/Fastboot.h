#ifndef FASTBOOT_H
#define FASTBOOT_H

#include <QProcess>
//#include <QString>
//#include <QThread>

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
    void fsatbootReboot(void);
    void fsatbootReboot(const QString &device);
    void flashBootDownload(const QString &device, const QString &filename = QString("boot.img"));


    void flashDownload(Fastboot::FastbootCDM, QString filename = QString("boot.img"));

    Fastboot::FastbootError fsatbootError(void);
//    Fastboot::FastbootCDM getProgram(void);
//    void setProgram(Fastboot::FastbootCDM);

    void fastbootStart(void);
    void fastbootStart(Fastboot::FastbootCDM);

private:
    QString     executionPath;      /*!< fsatboot.exe 程序执行路径 */
    QStringList devicesList ;        /*!< fsatboot 设备列表 */
    FastbootError fasterror;

    void fastbootDebug(void);
    void errorAnalysis(void);

    int exitCode;
    QProcess::ExitStatus exitStatus;
    QString readAllOutput;
    QString readAllError;

};

#endif // FASTBOOT_H
