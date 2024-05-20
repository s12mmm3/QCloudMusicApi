use std::os::raw::c_char;
use std::ffi::{CStr, CString, Int, c_int, c_void};

#[link(name="QCloudMusicApi", kind="dylib")]
extern "C" {
    fn invoke(memberName: *const c_char, value: *const c_char) -> c_int;
    fn get_result(key: c_int) -> *const c_char;
    fn free_result(key: c_int);
}

fn main() {
    let member_name = CString::new("lyric_new").expect("CString::new failed");
    let value = CString::new("{\"id\": \"2058263032\"}").expect("CString::new failed");

    unsafe {
        let result_id = invoke(member_name.as_ptr(), value.as_ptr());
        let result = get_result(result_id);
        let result_c_str = CStr::from_ptr(result);
        let result_str = result_c_str.to_str().expect("to_str failed");
        free_result(result_id);
        println!("调用结果: {}", result_str);
    }
}

