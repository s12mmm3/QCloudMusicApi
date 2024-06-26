﻿#ifndef CONFIG_H
#define CONFIG_H

#include <QVariantMap>

namespace QCloudMusicApi {
    namespace Config {

        const static QVariantMap resourceTypeMap{
            { "0", "R_SO_4_" },
            { "1", "R_MV_5_" },
            { "2", "A_PL_0_" },
            { "3", "R_AL_3_" },
            { "4", "A_DJ_1_" },
            { "5", "R_VI_62_" },
            { "6", "A_EV_2_" },
            { "7", "A_DR_14_" }
        };
        static QVariantMap APP_CONF{
            { "apiDomain", "https://interface.music.163.com" },
            { "domain", "https://music.163.com" },
            { "encrypt", false },
            { "encryptResponse", false },
            { "newApiDomain", true },
        };
    }
}
#endif // CONFIG_H
