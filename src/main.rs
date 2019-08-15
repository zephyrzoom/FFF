#![feature(asm)]

fn main() {
    unsafe {
        asm!("loop: jmp loop"::);
    }
}
