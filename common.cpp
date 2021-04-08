#include "common.h"

QString fastboot_enum2string(int enumration)
{
    QMetaEnum metaEnum = QMetaEnum::fromType<Fastboot::FastbootError>();
    return metaEnum.valueToKey(enumration);
}

QString adb_enum2string(int enumration)
{
    QMetaEnum metaEnum = QMetaEnum::fromType<AndroidDebugBridge::ADBError>();
    return metaEnum.valueToKey(enumration);
}

QString http_enum2string(int enumration)
{
    QMetaEnum metaEnum = QMetaEnum::fromType<QNetworkReply::NetworkError>();
    return metaEnum.valueToKey(enumration);
}



