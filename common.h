#ifndef COMMON_H
#define COMMON_H

#include <QString>
#include <QMetaEnum>
#include "httpcommunication.h"
#include "AndroidDebugBridge/AndroidDebugBridge.h"
#include "Fastboot/Fastboot.h"


#define UPDATA_CACHE_DEFAULT_FILENAME     "cache.img"
#define UPDATA_ABOOT_DEFAULT_FILENAME     "emmc_appsboot.mbn"
#define UPDATA_BOOT_DEFAULT_FILENAME      "boot.img"
#define UPDATA_PERSIST_DEFAULT_FILENAME   "persist.img"
#define UPDATA_RECOVERY_DEFAULT_FILENAME  "recovery.img"
#define UPDATA_SYSTEM_DEFAULT_FILENAME    "system.img"
#define UPDATA_USERDATA_DEFAULT_FILENAME  "userdata.img"
#define UPDATA_DDR_DEFAULT_FILENAME       "param_id.txt"
#define UPDATA_SPLASH_DEFAULT_FILENAME    "splash.img"

QString fastboot_enum2string(int enumration);
QString adb_enum2string(int enumration);
QString http_enum2string(int enumration);

#endif // COMMON_H
