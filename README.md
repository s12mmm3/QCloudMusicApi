# QCloudMusicApi
Qt版 网易云音乐 API

## 简介
本项目翻译自Node.js项目[Binaryify/NeteaseCloudMusicApi](https://github.com/Binaryify/NeteaseCloudMusicApi)

更新尽量与原项目同步，实现了大部分接口；支持跨语言调用，支持跨平台编译

使用方式参考[原项目文档](https://binaryify.github.io/NeteaseCloudMusicApi)，参数与返回结果与原项目完全一致；支持Http服务器（需要Qt6.4及以上），运行ApiServer即可启动本地Http服务器

## 需求和依赖
本项目需要以下库：

- Qt >= 5.12

## 使用说明
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

#### 使用ApiHelper类
该类实现了API的cookie管理

代码中包含头文件apihelper.h
```C++
#include "apihelper.h"
```
通过对应方法字符串调用
```C++
ApiHelper helper;
qDebug() << helper.invoke("lyric", { { "id", 1408586353 } });
```
直接调用API的方法
```C++
ApiHelper helper;
qDebug() << helper.invoke(&NeteaseCloudMusicApi::lyric, { { "id", 1408586353 } });
```

调用ApiHelper::set_cookie设置cookie，支持QVariantMap和QString

如果在传递参数时传递了 `"do-cache": 1` （1可以为任意值），结果会被缓存 5 分钟，下次调用时会直接返回缓存的结果。修改了参数，如将 `1` 改为其他值，会进行重新请求。

#### 使用NeteaseCloudMusicApi类
若无需保存cookie，可直接使用该类

代码中包含头文件module.h
```C++
#include "module.h"
```

可直接调用类函数
```C++
NeteaseCloudMusicApi api;
qDebug() << api.lyric({ { "id", 1408586353 } });
```

也可以通过Qt的反射调用
```C++
NeteaseCloudMusicApi api;
QVariantMap ret;
QMetaObject::invokeMethod(&api, "lyric"
                              , Qt::DirectConnection
                              , Q_RETURN_ARG(QVariantMap, ret)
                              , Q_ARG(QVariantMap, QVariantMap({ {"id", 1408586353} }));
qDebug() << ret;
```

### 跨语言调用
动态库提供C接口，开放给Python、Java和C#等支持与C语言交互的语言调用
```C
/**
 * @brief 通过反射调用API的成员函数。
 *
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
import os

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

### 在项目中引用CMake项目
在CMakeLists.txt文件中加上
```CMake
add_subdirectory(QCloudMusicApi)

add_executable(YourProjectName
    ./QCloudMusicApi/QCloudMusicApi/module.h
  main.cpp
)

target_compile_definitions(YourProjectName PRIVATE QCLOUDMUSICAPI_LIBRARY)
# 添加需要链接的库
target_link_libraries(YourProjectName  QCloudMusicApi)
```

C++代码中加上
```C++
#include "QCloudMusicApi/QCloudMusicApi/module.h"
```

### 手动编译
手动安装好Qt库后
```Shell
git clone --recursive https://github.com/s12mmm3/QCloudMusicApi.git
cmake -B build
cmake --build build -j
```
## License

[The MIT License (MIT)](https://github.com/s12mmm3/QCloudMusicApi/blob/master/LICENSE)
