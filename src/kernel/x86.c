u8 x86_io_in8(u16 port) {
    u8 result;
    asm ("in %0, %1" : "=a" (result) : "d" (port));
    return result;
}

u16 x86_io_in16(u16 port) {
    u16 result;
    asm ("in %0, %1" : "=a" (result) : "d" (port));
    return result;
}

u32 x86_io_in32(u16 port) {
    u32 result;
    asm ("in %0, %1" : "=a" (result) : "d" (port));
    return result;
}

void x86_io_out8(u16 port, u8 value) {
    asm("out %0, %1" : : "d" (port), "a" (value));
}

void x86_io_out16(u16 port, u16 value) {
    asm("out %0, %1" : : "d" (port), "a" (value));
}

void x86_io_out32(u16 port, u32 value) {
    asm("out %0, %1" : : "d" (port), "a" (value));
}
