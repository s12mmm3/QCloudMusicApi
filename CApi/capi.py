import ctypes
import json

lib = ctypes.CDLL("./CApi.dll")

def invoke(name, value):
    lib.invoke.restype = ctypes.c_char_p
    return lib.invoke(ctypes.create_string_buffer(name.encode())
                      , ctypes.create_string_buffer(value.encode()))

# lib.getFunNames.restype = ctypes.POINTER(ctypes.c_char_p)

# lib.getFunName.restype = ctypes.c_char_p
# funNames = []
# for i in range(0, lib.getFunCount()):
#     funNames.append(lib.getFunName(i))

# print(sorted(funNames))
result = invoke("lyric", "{\"id\": \"2058263032\"}")
print(json.loads(result))
