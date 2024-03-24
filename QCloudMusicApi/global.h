#ifndef GLOBAL_H
#define GLOBAL_H

#if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) \
|| defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#define Q_DECL_EXPORT __declspec(dllexport)
#define Q_DECL_IMPORT __declspec(dllimport)
#else
#define Q_DECL_EXPORT __attribute__((visibility("default")))
#define Q_DECL_IMPORT __attribute__((visibility("default")))
#endif

#if defined(BUILD_SHARED_LIBS)
#if defined(QCLOUDMUSICAPI_LIBRARY)
#  define QCLOUDMUSICAPI_EXPORT Q_DECL_EXPORT
#else
#  define QCLOUDMUSICAPI_EXPORT Q_DECL_IMPORT
#endif
#else
#  define QCLOUDMUSICAPI_EXPORT
#endif


//读函数名
#define READ_NAME(valueName) valueName

//写函数名
#define WRITE_NAME(valueName) \
set_##valueName

//变化函数名
#define NOTIFY_NAME(valueName) \
    valueName##Changed

//定义变量
#define DEFINE_VALUE(type, valueName, defaultValue) \
    Q_PROPERTY(type valueName READ READ_NAME(valueName) WRITE WRITE_NAME(valueName) NOTIFY NOTIFY_NAME(valueName)) \
    public: \
    type READ_NAME(valueName)() const { return m_##valueName; } \
    void WRITE_NAME(valueName)(type valueName) { \
        if(valueName == m_##valueName) { \
            return; \
    } \
        m_##valueName = valueName; \
        emit NOTIFY_NAME(valueName)(); \
} \
    Q_SIGNALS: \
    void NOTIFY_NAME(valueName)(); \
    private: \
    type m_##valueName = defaultValue; \
    public: \

#endif // GLOBAL_H
