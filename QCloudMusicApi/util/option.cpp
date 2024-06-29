#include "option.h"

using namespace QCloudMusicApi;
QVariantMap Option::createOption(QVariantMap query, QString crypto)
{
    return {
            { "crypto", query.value("crypto", crypto) },
            { "cookie", query["cookie"] },
            { "ua", query.value("ua", "") },
            { "proxy", query["proxy"] },
            { "realIP", query["realIP"] },
            };
}
