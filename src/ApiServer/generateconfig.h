#ifndef GENERATECONFIG_H
#define GENERATECONFIG_H

#include "../QCloudMusicApi/apihelper.h"
#include "../QCloudMusicApi/util/index.h"

#include <QDir>
#include <QFile>
#include <QStandardPaths>

static QString tmpPath = QStandardPaths::writableLocation(QStandardPaths::TempLocation);

void generateConfig() {
    // global.cnIp = generateRandomChineseIP()

    const auto res = ApiHelper().invoke("register_anonimous", {});
    const auto cookie = res["body"].toMap()["cookie"].toString();
    if (!cookie.isEmpty()) {
        const auto cookieObj = QCloudMusicApi::Index::cookieToJson(cookie);
        QFile file(QDir(tmpPath).absoluteFilePath("anonymous_token"));
        file.open(QIODevice::WriteOnly | QIODevice::Text);
        file.write(cookieObj["MUSIC_A"].toString().toUtf8());
    }
}

#endif // GENERATECONFIG_H
