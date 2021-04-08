#ifndef WORKUPDATE_H
#define WORKUPDATE_H

#include <QThread>
#include <QObject>

#include "httpcommunication.h"
#include "AndroidDebugBridge/AndroidDebugBridge.h"
#include "Fastboot/Fastboot.h"

class WorkUpdate : public QThread
{
    Q_OBJECT
public:
    enum WorkMode {
        NotMode         = 0,
        CacheMode       = (unsigned int)0x00000001,
        AbootMode       = (unsigned int)0x00000002,
        BootMode        = (unsigned int)0x00000004,
        PersistMode     = (unsigned int)0x00000008,
        RecoveryMode    = (unsigned int)0x00000010,
        SystemMode      = (unsigned int)0x00000020,
        UserdataMode    = (unsigned int)0x00000040,
        DDRMode         = (unsigned int)0x00000080,
        SplashMode      = (unsigned int)0x00000100,
        RebootMode      = (unsigned int)0x00000200,
    };
    Q_ENUM(WorkMode)
    WorkUpdate(QThread *parent = nullptr);

    QString readAllStandardOutput(void);
    void setADBDevice(const QString &device);
    void setWorkMode(unsigned int mode);
    void setWorkFileName(WorkUpdate::WorkMode mode, const QString &fileName);
    bool isSuccess(void);
    bool isRunning(void);

protected:
    void run(void) Q_DECL_OVERRIDE;

private:
    void setStandardOutput(const QString &string);
    QString adbDevice;          /*!< 升级指定的ADB设备 */
    QString standardOutput;     /*!< 升级过程中的信息输出列表 */
    unsigned int workMode;      /*!< 设置fast boot工作模式 */
    bool    isUpdataSuccess;    /*!< 程序升级是否成功 */
    bool    isWorkRunning;      /*!< 程序升级是否正在进行 */

    /*!< 升级文件名称 */
    QString cacheFilename    ;
    QString abootFilename    ;
    QString bootFilename     ;
    QString persistFilename  ;
    QString recoveryFilename ;
    QString systemFilename   ;
    QString userdataFilename ;
    QString DDRFilename      ;
    QString splashFilename   ;

signals:
    void workingFinished();
    void readyReadStandardOutput();
};


class SearchDevice : public QThread
{
    Q_OBJECT
public:
    SearchDevice(QThread *parent = nullptr);
    const QStringList &getDevices(void);
private:
    QStringList devices;
protected:
    void run(void) Q_DECL_OVERRIDE;
signals:
    void searchFinished(void);
};


#endif // WORKUPDATE_H
