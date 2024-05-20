import ctypes
import json
import platform


def get_library_filename(libname):
    system = platform.system()

    if system == "Windows":
        return f"{libname}.dll"
    elif system == "Linux":
        return f"lib{libname}.so"
    elif system == "Darwin":
        return f"lib{libname}.dylib"
    else:
        raise NotImplementedError(f"Unsupported OS: {system}")


libname = "QCloudMusicApi"
filename = get_library_filename(libname)
lib = ctypes.CDLL(filename)

lib.invoke.argtypes = [ ctypes.c_char_p, ctypes.c_char_p ]
lib.invoke.restype = ctypes.c_int

lib.get_result.argtypes = [ ctypes.c_int ]
lib.get_result.restype = ctypes.c_char_p

lib.free_result.argtypes = [ ctypes.c_int ]
lib.free_result.restype = ctypes.c_void_p

lib.freeApi.restype = ctypes.c_void_p
lib.memberName.restype = ctypes.c_char_p
lib.memberCount.restype = ctypes.c_int


lib.set_proxy.argtypes = [ ctypes.c_char_p ]
lib.set_proxy.restype = ctypes.c_void_p

proxy = ""
lib.set_proxy(ctypes.create_string_buffer(proxy.encode()))

def invoke(name, value):
    result_id = lib.invoke(ctypes.create_string_buffer(name.encode()),
                         ctypes.create_string_buffer(value.encode()))
    result = lib.get_result(result_id)
    lib.free_result(result_id)
    
    return result

if __name__ == '__main__':

    # funNames = []
    # for i in range(0, lib.memberCount()):
    #     funNames.append(lib.memberName(i))

    # print(sorted(funNames))
    result = invoke("lyric_new", "{\"id\": \"2058263032\"}")
    print("result", json.loads(result))
    
    lib.freeApi()
