import ctypes
import json

def invoke(name, value):
    lib = ctypes.CDLL("./QCloudMusicApi.dll")
    lib.invoke.restype = ctypes.c_char_p
    return lib.invoke(ctypes.create_string_buffer(name.encode())
                      , ctypes.create_string_buffer(value.encode()))

if __name__ == '__main__':

    # lib.memberName.restype = ctypes.c_char_p
    # funNames = []
    # for i in range(0, lib.memberCount()):
    #     funNames.append(lib.memberName(i))

    # print(sorted(funNames))
    result = invoke("lyric_new", "{\"id\": \"2058263032\"}")
    print("result", json.loads(result))
