import ctypes
import json
import os

lib = ctypes.CDLL("./QCloudMusicApi.dll")

lib.invoke.argtypes = [ ctypes.c_char_p, ctypes.c_char_p ]
lib.invoke.restype = ctypes.c_char_p

lib.freeApi.restype = ctypes.c_void_p
lib.memberName.restype = ctypes.c_char_p
lib.memberCount.restype = ctypes.c_int


lib.set_proxy.argtypes = [ ctypes.c_char_p ]
lib.set_proxy.restype = ctypes.c_void_p

proxy = ""
lib.set_proxy(ctypes.create_string_buffer(proxy.encode()))

def invoke(name, value):
    result = lib.invoke(ctypes.create_string_buffer(name.encode()),
                         ctypes.create_string_buffer(value.encode()))
    return result

if __name__ == '__main__':

    # funNames = []
    # for i in range(0, lib.memberCount()):
    #     funNames.append(lib.memberName(i))

    # print(sorted(funNames))
    result = invoke("lyric_new", "{\"id\": \"2058263032\"}")
    print("result", json.loads(result))
    
    lib.freeApi()
