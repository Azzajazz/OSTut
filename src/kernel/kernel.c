typedef char i8;
typedef unsigned char u8;
typedef short i16;
typedef unsigned short u16;
typedef int i32;
typedef unsigned int u32;

// TODO(Azzajazz): Move these to an x86 specific file later
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


// String code

typedef struct {
    char *data;
    u32 length;
} String;

u32 cstring_length(char *cstr) {
    u32 length = 0;
    for (char *p = cstr; *p; ++p) {
        length++;
    }
    return length;
}

String string_lit(const char *literal) {
    return (String){.data = literal, .length = cstring_length(literal)};
}


// VGA code

#define VGA_GFX_ADDR_REG 0x3ce
#define VGA_GFX_DATA_REG 0x3cf
#define VGA_GFX_SET_RESET_IDX 0x00
#define VGA_GFX_SET_RESET_ENABLE_IDX 0x01
#define VGA_GFX_COL_CMP_IDX 0x02
#define VGA_GFX_DATA_ROT_IDX 0x03
#define VGA_GFX_READ_MAP_SEL_IDX 0x04
#define VGA_GFX_MODE_IDX 0x05
#define VGA_GFX_MISC_IDX 0x06
#define VGA_GFX_COL_DONT_CARE_IDX 0x07
#define VGA_GFX_BIT_MASK_IDX 0x08

#define VGA_SEQ_ADDR_REG 0x3c4
#define VGA_SEQ_DATA_REG 0x3c5
#define VGA_SEQ_RESET_IDX 0x00
#define VGA_SEQ_CLOCK_MODE_IDX 0x01
#define VGA_SEQ_MAP_MASK_IDX 0x02
#define VGA_SEQ_CHAR_MAP_SEL_IDX 0x03
#define VGA_SEQ_MEM_MODE_IDX 0x04

#define VGA_ATTR_WRITE_REG 0x3c0
#define VGA_ATTR_READ_REG 0x3c1
#define VGA_ATTR_PALETTE1_IDX 0x00
#define VGA_ATTR_PALETTE2_IDX 0x01
#define VGA_ATTR_PALETTE3_IDX 0x02
#define VGA_ATTR_PALETTE4_IDX 0x03
#define VGA_ATTR_PALETTE5_IDX 0x04
#define VGA_ATTR_PALETTE6_IDX 0x05
#define VGA_ATTR_PALETTE7_IDX 0x06
#define VGA_ATTR_PALETTE8_IDX 0x07
#define VGA_ATTR_PALETTE9_IDX 0x08
#define VGA_ATTR_PALETTE10_IDX 0x09
#define VGA_ATTR_PALETTE11_IDX 0x0a
#define VGA_ATTR_PALETTE12_IDX 0x0b
#define VGA_ATTR_PALETTE13_IDX 0x0c
#define VGA_ATTR_PALETTE14_IDX 0x0d
#define VGA_ATTR_PALETTE15_IDX 0x0e
#define VGA_ATTR_PALETTE16_IDX 0x0f

#define VGA_ATTR_MODE_CTRL_IDX 0x10
#define VGA_ATTR_OVERSCAN_COL_IDX 0x11
#define VGA_ATTR_COL_PLANE_ENABLE_IDX 0x12
#define VGA_ATTR_HZ_PIXEL_PAN_IDX 0x13
#define VGA_ATTR_COL_SEL_IDX 0x14

#define VGA_CRTC_ADDR_REGB 0x3b4
#define VGA_CRTC_DATA_REGB 0x3b5
#define VGA_CRTC_ADDR_REGD 0x3d4
#define VGA_CRTC_DATA_REGD 0x3d5
#define VGA_CRTC_HZ_TOTAL_IDX 0x00
#define VGA_CRTC_END_HZ_DISP_IDX 0x01
#define VGA_CRTC_START_HZ_BLANK_IDX 0x02
#define VGA_CRTC_END_HZ_BLANK_IDX 0x03
#define VGA_CRTC_START_HZ_RETRC_IDX 0x04
#define VGA_CRTC_END_HZ_RETRC_IDX 0x05
#define VGA_CRTC_VERT_TOTAL_IDX 0x06
#define VGA_CRTC_OVERFLOW_IDX 0x07
#define VGA_CRTC_PRESET_ROW_IDX 0x08
#define VGA_CRTC_MAX_SCAN_LINE_IDX 0x09
#define VGA_CRTC_CURSOR_START_IDX 0x0a
#define VGA_CRTC_CURSOR_END_IDX 0x0b
#define VGA_CRTC_START_ADDR_HIGH_IDX 0x0c
#define VGA_CRTC_START_ADDR_LOW_IDX 0x0d
#define VGA_CRTC_CURSOR_LOC_HIGH_IDX 0x0e
#define VGA_CRTC_CURSOR_LOC_LOW_IDX 0x0f
#define VGA_CRTC_VERT_RETRC_START_IDX 0x10
#define VGA_CRTC_VERT_RETRC_END_IDX 0x11
#define VGA_CRTC_VERT_DISP_END_IDX 0x12
#define VGA_CRTC_OFFSET_IDX 0x13
#define VGA_CRTC_UNDERLINE_LOC_IDX 0x14
#define VGA_CRTC_START_VERT_BLANK_IDX 0x15
#define VGA_CRTC_END_VERT_BLANK_IDX 0x16
#define VGA_CRTC_MODE_CTRL_IDX 0x17
#define VGA_CRTC_LINE_CMP_IDX 0x18

#define VGA_DAC_ADDR_WRITE_MODE_REG 0x3c8
#define VGA_DAC_ADDR_READ_MODE_REG 0x3c7
#define VGA_DAC_DATA_REG 0x3c9
#define VGA_DAC_STATE_REG 0x3c7

#define VGA_MISC_OUT_READ_REG 0x3cc
#define VGA_MISC_OUT_WRITE_REG 0x3c2
#define VGA_FEAT_CTRL_READ_REG 0x3ca
#define VGA_FEAT_CTRL_WRITE_REG_MONO 0x3ba
#define VGA_FEAT_CTRL_WRITE_REG_COL 0x3da
#define VGA_INPUT_STATUS0_REG 0x3c2
#define VGA_INPUT_STATUS1_REG_MONO 0x3ba
#define VGA_INPUT_STATUS1_REG_COL 0x3da


volatile u16 *vga_buffer = (u16*)0xb8000;

void kmain() {
    *vga_buffer = 'a' | 0x7 << 8;
    for(;;);
}
