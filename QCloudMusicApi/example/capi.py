import ctypes
import json
import os

# 设置环境变量，不输出Debug级别日志
os.putenv('QT_LOGGING_RULES', '*.debug=false')

lib = ctypes.CDLL("./QCloudMusicApi.dll")

lib.invoke.argtypes = [ ctypes.c_char_p, ctypes.c_char_p ]
lib.invoke.restype = ctypes.c_char_p

lib.invokeUrl.argtypes = [ ctypes.c_char_p ]
lib.invokeUrl.restype = ctypes.c_char_p

lib.freeApi.restype = ctypes.c_void_p
lib.memberName.restype = ctypes.c_char_p
lib.memberCount.restype = ctypes.c_int

lib.set_proxy.argtypes = [ ctypes.c_char_p ]
lib.set_proxy.restype = ctypes.c_void_p

lib.set_cookie.argtypes = [ ctypes.c_char_p ]
lib.set_cookie.restype = ctypes.c_void_p

# 反射调用API的成员函数
def invoke(name, value):
    result = lib.invoke(ctypes.create_string_buffer(name.encode()),
                         ctypes.create_string_buffer(value.encode()))
    return result

# 反射调用API的成员函数
def invokeUrl(url):
    result = lib.invokeUrl(ctypes.create_string_buffer(url.encode()))
    return result

# 设置全局cookie
def set_cookie(cookie):
    lib.set_cookie(ctypes.create_string_buffer(cookie.encode()))

# 获取cookie
def cookie():
    result = lib.cookie()
    return result

# 设置全局代理
def set_proxy(proxy):
    lib.set_proxy(ctypes.create_string_buffer(proxy.encode()))

# 获取代理
def proxy():
    result = lib.proxy()
    return result

# 获取API列表
def memberList():
    list = []
    for i in range(0, lib.memberCount()):
        list.append(lib.memberName(i))
    return list

if __name__ == '__main__':
    result = invoke("lyric_new", "{\"id\": \"2058263032\"}")
    print("result", json.dumps(json.loads(result), indent = 4, ensure_ascii = False))
    
    lib.freeApi()
