use std::os::raw::c_char;
use std::ffi::{CStr, CString};

#[link(name="QCloudMusicApi", kind="dylib")]
extern "C" {
    fn invoke(memberName: *const c_char, value: *const c_char) -> *const c_char;
}

fn main() {
    let member_name = CString::new("lyric_new").expect("CString::new failed");
    let value = CString::new("{\"id\": \"2058263032\"}").expect("CString::new failed");

    unsafe {
        let result = invoke(member_name.as_ptr(), value.as_ptr());
        let result_c_str = CStr::from_ptr(result);
        let result_str = result_c_str.to_str().expect("to_str failed");
        println!("调用结果: {}", result_str);
    }
}

