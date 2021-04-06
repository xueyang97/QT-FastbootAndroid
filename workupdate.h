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
    WorkUpdate();

    QString readAllStandardOutput(void);
    void setADBDevice(const QString &device);

protected:
    void run(void) Q_DECL_OVERRIDE;
private:
    void setStandardOutput(const QString &string);
    QString adbDevice;
    QString standardOutput;

signals:
    void workingFinished(int);
    void readyReadStandardOutput();
};

#endif // WORKUPDATE_H
