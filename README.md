# QCloudMusicApi
Qt版 网易云音乐 API

## 简介
本项目翻译自Node.js项目[Binaryify/NeteaseCloudMusicApi](https://github.com/Binaryify/NeteaseCloudMusicApi)

更新尽量与原项目同步

使用方式请参考[原项目文档](https://binaryify.github.io/NeteaseCloudMusicApi)，参数与返回结果与原项目完全一致

## 需求和依赖
本项目需要以下库和工具：

- Qt >= 5.10
- OpenSSL >= 1.1.1
- conan2

conan 是一个开源的 C/C++ 包管理器，可以自动下载、编译和链接依赖库。可以使用以下命令来安装 conan（以 Ubuntu 为例）：

```bash
sudo apt install python3-pip
pip3 install conan
```

在Qt Creator装上conan插件，帮助->关于插件->conan打勾重启，打开项目重新CMake，自动下载/编译依赖的库

## 使用说明
### 引用动态库文件
在CMakeLists.txt文件中加上
```
# 指定第三方库的路径
set(LIBRARY_DIR "${PROJECT_SOURCE_DIR}/QCloudMusicApi")

# 添加头文件路径
include_directories(${LIBRARY_DIR}/include)

# 添加库文件路径
link_directories(${LIBRARY_DIR}/bin)
```

代码中包含头文件module.h
```
#include "module.h"
```

可直接调用类函数
```
NeteaseCloudMusicApi api;
qDebug() << api.song_url_v1(QVariantMap({ {"id", 1408586353} }));
```

也可以通过Qt的反射调用
```
NeteaseCloudMusicApi api;
QByteArray ret;
QMetaObject::invokeMethod(&api, "song_url_v1"
                              , Qt::DirectConnection
                              , Q_RETURN_ARG(QByteArray, ret)
                              , Q_ARG(QVariantMap, QVariantMap({ {"id", 1408586353} }));
qDebug() << ret;
```

### 在项目中引用CMake项目
将conanfile.txt移动至项目根目录
在CMakeLists.txt文件中加上
```
add_subdirectory(QCloudMusicApi)

add_executable(MusicApiTest
    ./QCloudMusicApi/QCloudMusicApi/module.h
  main.cpp
)

target_compile_definitions(YourProjectName PRIVATE QCLOUDMUSICAPI_LIBRARY)
# 添加需要链接的库
target_link_libraries(YourProjectName Qt${QT_VERSION_MAJOR}::Core QCloudMusicApi)
```

C++代码中加上
```
#include "QCloudMusicApi/QCloudMusicApi/module.h"
```

若CMake时出现类似错误
```
error: Syntax error in cmake code at ... when parsing string ... Invalid character escape '\q'.
```
进入出错的CMake文件，将"\\"全部替换为"/"，重新CMake即可

## License

[The MIT License (MIT)](https://github.com/s12mmm3/QCloudMusicApi/blob/master/LICENSE)
