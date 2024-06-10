use std::os::raw::c_char;
use std::ffi::{CStr, CString};

#[link(name = "QCloudMusicApi", kind = "dylib")]
extern "C" {
    fn invoke(memberName: *const c_char, value: *const c_char) -> *const c_char;
    fn invokeUrl(url: *const c_char) -> *const c_char;
    fn memberName(index: std::ffi::c_int) -> *const c_char;
    fn memberCount() -> std::ffi::c_int;
    fn set_cookie(cookie: *const c_char);
    fn cookie() -> *const c_char;
    fn set_proxy(proxy: *const c_char);
    fn proxy() -> *const c_char;
    fn setFilterRules(rules: *const c_char);
    fn loadPlugin(fileName: *const c_char);
    fn unloadPlugin(fileName: *const c_char);
}

pub struct ApiHelper;

impl ApiHelper {

    // 获取API列表
    pub fn memberList(&self) -> Vec<String> {
        let mut list = Vec::new();
        unsafe {
            for i in 0..memberCount() as i32 {
                let name_cstr = CStr::from_ptr(memberName(i as i32));
                list.push(name_cstr.to_str().unwrap().to_owned());
            }
        }
        list
    }

    // 反射调用API的成员函数
    pub fn invoke(&self, member_name: &str, value: &str) -> String {
        let mut result = String::new();
        unsafe {
            result = CStr::from_ptr(invoke(CString::new(member_name).expect("").as_ptr(),
             CString::new(value).expect("").as_ptr()))
             .to_str().expect("").to_string()
        }
        result
    }

    // 反射调用API的成员函数
    pub fn invokeUrl(&self, url: &str) -> String {
        let mut result = String::new();
        unsafe {
            result = CStr::from_ptr(invokeUrl(CString::new(url).expect("").as_ptr()))
             .to_str().expect("").to_string()
        }
        result
    }

    // 设置全局cookie
    pub fn set_cookie(&self, cookie: &str) {
        unsafe {
            set_cookie(CString::new(cookie).expect("").as_ptr())
        }
    }

    // 获取cookie
    pub fn cookie(&self) -> String {
        let mut result = String::new();
        unsafe {
            result = CStr::from_ptr(cookie())
            .to_str().expect("").to_string()
        }
        result
    }

    // 设置全局代理
    pub fn set_proxy(&self, proxy: &str) {
        unsafe {
            set_proxy(CString::new(proxy).expect("").as_ptr())
        }
    }

    // 获取代理
    pub fn proxy(&self) -> String {
        let mut result = String::new();
        unsafe {
            result = CStr::from_ptr(proxy())
            .to_str().expect("").to_string()
        }
        result
    }

    // 设置log规则
    pub fn setFilterRules(&self, rules: &str) {
        unsafe {
            setFilterRules(CString::new(rules).expect("").as_ptr())
        }
    }

    // 加载插件
    pub fn loadPlugin(&self, fileName: &str) {
        unsafe {
            loadPlugin(CString::new(fileName).expect("").as_ptr())
        }
    }

    // 卸载插件
    pub fn unloadPlugin(&self, fileName: &str) {
        unsafe {
            unloadPlugin(CString::new(fileName).expect("").as_ptr())
        }
    }
}

fn main() {
    let helper = ApiHelper;
    helper.setFilterRules("*.debug=false");
    for method in helper.memberList() {
        println!("{}", method);
    }

    println!("Result: {}", helper.invoke("artist_detail", "{\"id\": \"15396\"}"));
    
    // println!("Result: {}", helper.invokeUrl("/song/url/v1?id=2058263032, 2057797340&level=exhigh"));

}
