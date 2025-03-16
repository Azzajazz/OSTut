typedef char i8;
typedef unsigned char u8;
typedef short i16;
typedef unsigned short u16;
typedef int i32;
typedef unsigned int u32;

// TODO: Move these to an x86 specific file later
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

u32 cstring_length(const char *cstr) {
    u32 length = 0;
    for (const char *p = cstr; *p; ++p) {
        length++;
    }
    return length;
}

String string_lit(char *literal) {
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

#define VGA_COL_BLACK 0x00
#define VGA_COL_BLUE 0x01
#define VGA_COL_GREEN 0x02
#define VGA_COL_CYAN 0x03
#define VGA_COL_RED 0x04
#define VGA_COL_MAGENTA 0x05
#define VGA_COL_BROWN 0x06
#define VGA_COL_LIGHTGREY 0x07
#define VGA_COL_DARKGREY 0x08
#define VGA_COL_LIGHTBLUE 0x09
#define VGA_COL_LIGHTGREEN 0x10
#define VGA_COL_LIGHTCYAN 0x11
#define VGA_COL_LIGHTRED 0x12
#define VGA_COL_LIGHTMAGENTA 0x13
#define VGA_COL_YELLOW 0x14
#define VGA_COL_WHITE 0x15

// TODO: This is not the only possible width and height
#define VGA_TEXT_BUFFER_WIDTH 80
#define VGA_TEXT_BUFFER_HEIGHT 25
volatile u16 *vga_buffer = (u16*)0xb8000;

typedef enum {
    VGA_MODE_TEXT,
} Vga_Mode;

void vga_init_cursor(void);

void vga_set_mode(Vga_Mode mode) {
    switch (mode) {
        case VGA_MODE_TEXT:
            vga_init_cursor();
            // TODO: How do you enable text mode?
            return;
    }
}

// TODO: Should these register read and writes save addresses and load them back?
u8 vga_read_indexed_register(u16 addr_port, u16 data_port, u8 reg_idx) {
    x86_io_out8(addr_port, reg_idx);
    // TODO: I/O delay
    x86_io_out8(0x80, 0x00);
    return x86_io_in8(data_port);
}

void vga_write_indexed_register(u16 addr_port, u16 data_port, u8 reg_idx, u8 data) {
    x86_io_out8(addr_port, reg_idx);
    // TODO: I/O delay
    x86_io_out8(data_port, data);
}

u8 vga_read_crtc_register(u8 reg_idx) {
    // TODO: Check if we should use REGD or REGB
    return vga_read_indexed_register(
        VGA_CRTC_ADDR_REGD,
        VGA_CRTC_DATA_REGD,
        reg_idx
    );
}

void vga_write_crtc_register(u8 reg_idx, u8 value) {
    return vga_write_indexed_register(
        VGA_CRTC_ADDR_REGD,
        VGA_CRTC_DATA_REGD,
        reg_idx, value
    );
}

// NOTE: Only valid in text mode
u16 vga_get_cursor_index() {
    u8 pos_high = vga_read_indexed_register(
        VGA_CRTC_ADDR_REGD,
        VGA_CRTC_DATA_REGD,
        VGA_CRTC_CURSOR_LOC_HIGH_IDX
    );
    u8 pos_low = vga_read_indexed_register(
        VGA_CRTC_ADDR_REGD,
        VGA_CRTC_DATA_REGD,
        VGA_CRTC_CURSOR_LOC_LOW_IDX
    );
    return pos_high << 8 | pos_low;
}

// NOTE: Only valid in text mode
void vga_set_cursor_index(u16 index) {
    vga_write_indexed_register(
        VGA_CRTC_ADDR_REGD,
        VGA_CRTC_DATA_REGD,
        VGA_CRTC_CURSOR_LOC_HIGH_IDX,
        index >> 8
    );
    vga_write_indexed_register(
        VGA_CRTC_ADDR_REGD,
        VGA_CRTC_DATA_REGD,
        VGA_CRTC_CURSOR_LOC_LOW_IDX,
        index & 0xff
    );
}

// NOTE: Only valid in text mode
void vga_init_cursor() {
    vga_set_cursor_index(0);
    u8 max_scan_line = vga_read_crtc_register(VGA_CRTC_MAX_SCAN_LINE_IDX) & 0x1f;
    // TODO: Preserve reserved fields
    vga_write_crtc_register(VGA_CRTC_CURSOR_START_IDX, max_scan_line - 3);
    vga_write_crtc_register(VGA_CRTC_CURSOR_END_IDX, max_scan_line);
    // Set the end scan line to the maximum
}

// I/O code
// NOTE: Only supports VGA hardware

void io_clear_screen() {
    for (int i = 0; i < VGA_TEXT_BUFFER_WIDTH * VGA_TEXT_BUFFER_HEIGHT; ++i) {
        // NOTE: The foreground colour is set to light grey because the cursor uses this.
        vga_buffer[i] = ' ' | (VGA_COL_BLACK << 12) | (VGA_COL_LIGHTGREY << 8);
    }
}

void io_scroll_screen() {
    for (int i = VGA_TEXT_BUFFER_WIDTH; i < VGA_TEXT_BUFFER_WIDTH * VGA_TEXT_BUFFER_HEIGHT; ++i) {
        vga_buffer[i - VGA_TEXT_BUFFER_WIDTH] = vga_buffer[i];
    }
    for (int i = 0; i < VGA_TEXT_BUFFER_WIDTH; ++i) {
        vga_buffer[VGA_TEXT_BUFFER_WIDTH * (VGA_TEXT_BUFFER_HEIGHT - 1) + i] = ' ' | (VGA_COL_BLACK << 12) | (VGA_COL_LIGHTGREY << 8);
    }
    u16 cursor_index = vga_get_cursor_index();
    vga_set_cursor_index(VGA_TEXT_BUFFER_WIDTH * (VGA_TEXT_BUFFER_HEIGHT - 1));
}

void io_print_char(char c) {
    //TODO: Allow custom background and foreground colours
    //TODO: Screen scrolling
    u16 cursor_index = vga_get_cursor_index();
    if (c == '\n') {
        if (cursor_index < VGA_TEXT_BUFFER_WIDTH * (VGA_TEXT_BUFFER_HEIGHT - 1)) {
            cursor_index += VGA_TEXT_BUFFER_WIDTH - (cursor_index % VGA_TEXT_BUFFER_WIDTH);
            vga_set_cursor_index(cursor_index);
        }
        else {
            io_scroll_screen();
        }
    }
    else {
        vga_buffer[cursor_index] = c | (VGA_COL_BLACK << 12) | (VGA_COL_LIGHTGREY << 8);
        if (cursor_index < VGA_TEXT_BUFFER_WIDTH * VGA_TEXT_BUFFER_HEIGHT - 1) {
            cursor_index += 1;
            vga_set_cursor_index(cursor_index);
        }
        else {
            io_scroll_screen();
        }
    }
}


void kmain() {
    vga_set_mode(VGA_MODE_TEXT);
    io_clear_screen();
    for (int i = 0; i < VGA_TEXT_BUFFER_HEIGHT - 1; ++i) {
        io_print_char('a' + i);
        io_print_char('\n');
    }
    for (int i = 0; i < VGA_TEXT_BUFFER_WIDTH; ++i) {
        io_print_char('a');
    }
    for(;;);
}
