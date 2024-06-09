# QCloudMusicApi - Qt版 网易云音乐 API

[![GitHub Actions CI Status](https://github.com/s12mmm3/QCloudMusicApi/actions/workflows/windows.yml/badge.svg)](https://github.com/s12mmm3/QCloudMusicApi/actions/workflows/windows.yml)
[![GitHub Actions CI Status](https://github.com/s12mmm3/QCloudMusicApi/actions/workflows/macos.yml/badge.svg)](https://github.com/s12mmm3/QCloudMusicApi/actions/workflows/macos.yml)
[![GitHub Actions CI Status](https://github.com/s12mmm3/QCloudMusicApi/actions/workflows/ubuntu.yml/badge.svg)](https://github.com/s12mmm3/QCloudMusicApi/actions/workflows/ubuntu.yml)

![C++ version](https://img.shields.io/badge/C++-11-00599C?logo=++)
[![Qt version](https://img.shields.io/badge/Qt-5.12+-41CD52?logo=qt)](https://www.qt.io)
[![Qt version](https://img.shields.io/badge/Qt-6.0+-41CD52?logo=qt)](https://www.qt.io)
![GitHub license](https://img.shields.io/github/license/s12mmm3/QCloudMusicApi)
********************************

## 简介

本项目翻译自Node.js项目[Binaryify/NeteaseCloudMusicApi](https://github.com/Binaryify/NeteaseCloudMusicApi)

更新尽量与原项目同步，实现了大部分接口；支持跨语言调用，支持跨平台编译

使用方式参考[原项目文档](https://binaryify.github.io/NeteaseCloudMusicApi)，参数与返回结果与原项目完全一致；支持设置Http服务器（需要Qt6.4及以上）

### 目录

- [需求和依赖](#需求和依赖)
- [使用说明](#使用说明)
- [编译方式](#编译方式)
- [License](#License)

---

## 需求和依赖

- [Qt 5.12+](https://www.qt.io/download-qt-installer)

## 使用说明

#### 使用ApiHelper类

该类实现了API的cookie管理；支持Qt反射调用

通过对应方法字符串调用
```C++
#include "apihelper.h"

ApiHelper helper;
qDebug() << helper.invoke("lyric", { { "id", 1408586353 } });
```
可通过传入Url调用
```C++
ApiHelper helper;
qDebug() << helper.invokeUrl("/song/url/v1?id=2058263032, 2057797340&level=exhigh");
qDebug() << helper.invokeUrl("http://localhost:3000/activate/init/profile");
```

调用ApiHelper::set_cookie设置cookie，支持QVariantMap和QString

#### 使用NeteaseCloudMusicApi类

若无需保存cookie，可直接使用该类；支持Qt反射调用

可直接调用类函数
```C++
#include "module.h"

NeteaseCloudMusicApi api;
qDebug() << api.lyric({ { "id", 1408586353 } });
```

### 跨语言调用

动态库提供C接口，开放给Python、Java和C#等支持与C语言交互的语言调用
```C++
/**
 * @brief 通过反射调用API的成员函数。
 * @param memberName 调用成员函数的名称
 * @param value 参数的JSON格式字符串
 * @return 调用结果的JSON格式字符串
 */
const char* invoke(char* memberName, char* value);
```
其他例子可以在```QCloudMusicApi/example```路径下查看

#### Python调用

```Python
import ctypes
import json

lib = ctypes.CDLL("./QCloudMusicApi.dll")
lib.invoke.argtypes = [ ctypes.c_char_p, ctypes.c_char_p ]
lib.invoke.restype = ctypes.c_char_p

def invoke(name, value):
    result = lib.invoke(ctypes.create_string_buffer(name.encode()),
                         ctypes.create_string_buffer(value.encode()))
    return result

if __name__ == '__main__':
    result = invoke("lyric_new", "{\"id\": \"2058263032\"}")
    print("result", json.loads(result))
```

### 引用动态库文件

在CMakeLists.txt文件中加上
```CMake
# 指定第三方库的路径
set(LIBRARY_DIR "${PROJECT_SOURCE_DIR}/QCloudMusicApi")

# 添加头文件路径
include_directories(${LIBRARY_DIR}/include)

# 添加库文件路径
link_directories(${LIBRARY_DIR}/bin)
```

### 在项目中引用CMake项目

在CMakeLists.txt文件中加上
```CMake
add_subdirectory(QCloudMusicApi)

add_executable(${PROJECT_NAME}
    ./QCloudMusicApi/QCloudMusicApi/apihelper.h
  main.cpp
)

target_compile_definitions(${PROJECT_NAME} PRIVATE QCLOUDMUSICAPI_LIBRARY)
target_link_libraries(${PROJECT_NAME}  QCloudMusicApi)
```
引用头文件
```C++
#include "QCloudMusicApi/QCloudMusicApi/apihelper.h"
```

## 编译方式

```Shell
git clone --recursive https://github.com/s12mmm3/QCloudMusicApi.git
cd QCloudMusicApi
cmake -B build
cmake --build build -j
```

## License

[The MIT License (MIT)](https://github.com/s12mmm3/QCloudMusicApi/blob/master/LICENSE)
