use std::os::raw::c_char;
use std::ffi::{CStr, CString};

#[link(name="QCloudMusicApi", kind="dylib")]
extern "C" {
    fn invoke(memberName: *const c_char, value: *const c_char) -> *const c_char;
}

fn main() {
    unsafe {
        let result = invoke(CString::new("lyric_new").as_ptr(), CString::new("{\"id\": \"2058263032\"}").as_ptr());
        println!(CStr::from_ptr(result).to_str());
    }
}

