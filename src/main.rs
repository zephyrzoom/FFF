#![feature(asm)]

fn add(a: i32, b: i32) -> i32 {
    let c: i32;
    unsafe {
        asm!("add $2, $0"
             : "=r"(c)
             : "0"(a), "r"(b)
             );
    }
    c
}

fn main() {
    assert_eq!(add(3, 14159), 14162)
}
