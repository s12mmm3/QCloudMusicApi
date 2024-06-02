#include "api.h"

Api::Api(QObject *parent)
    : QObject{parent}
{}

QVariantMap Api::aaaaaaaaaaaaaaaaaaaa(QVariantMap query)
{
    return {
        { "A method from plugin", query },
    };
}

QVariantMap Api::bbbbbbbbbbbbbbbbbbbb(QVariantMap query)
{
    qDebug() << "bbbbbbbbbbbbbbbbbbbb";
    return {
        { "bbbbbbbbbbbbbbbbbbbb", query },
    };
}
