#include <stdarg.h>

typedef char i8;
typedef unsigned char u8;
typedef short i16;
typedef unsigned short u16;
typedef int i32;
typedef unsigned int u32;

typedef int bool;
#define true 1
#define false 0

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

String from_cstr(char *literal) {
    return (String){.data = literal, .length = cstring_length(literal)};
}

// TODO: We want to assert that start_index < str.length, but we don't have an assert.
String string_after_index(String str, u32 start_index) {
    return (String){str.data + start_index, str.length - start_index};
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

void io_print_string(String str) {
    for (u32 i = 0; i < str.length; ++i) {
        io_print_char(str.data[i]);
    }
}

void io_print_u32(u32 value) {
    if (value == 0) {
        io_print_char('0');
        return;
    }

    char digits[10]; // u32 is at most 10 digits in decimal
    int digit_count = 0;
    while (value > 0) {
        digits[digit_count++] = value % 10 + '0';
        value /= 10;
    }
    for (; digit_count > 0; --digit_count) {
        io_print_char(digits[digit_count - 1]);
    }
}

void io_print_u16(u16 value) {
    io_print_u32((u32)value);
}

void io_print_u8(u8 value) {
    io_print_u32((u32)value);
}

void io_print_i32(i32 value) {
    if (value < 0) {
        io_print_char('-');
        value = -value;
    }
    io_print_u32(value);
}

void io_print_i16(i16 value) {
    io_print_i32((i32)value);
}

void io_print_i8(i8 value) {
    io_print_i32((i32)value);
}

void io_print_x32(u32 value) {
    if (value == 0) {
        io_print_char('0');
        return;
    }

    char digits[8]; // u32 is at most 8 digits in hexadecimal
    int digit_count = 0;
    while (value > 0) {
        int digit = value & 0xf;
        if (digit < 10) {
            digits[digit_count++] = digit + '0';
        }
        else {
            digits[digit_count++] = digit - 10 + 'a';
        }

        value >>= 4;
    }
    for (; digit_count > 0; --digit_count) {
        io_print_char(digits[digit_count - 1]);
    }
}

void io_print_x16(u16 value) {
    io_print_x32((u32)value);
}

void io_print_x8(u8 value) {
    io_print_x32((u32)value);
}

typedef enum {
    FMT_REGULAR,
    FMT_FORMAT,
    FMT_UNSIGNED,
    FMT_SIGNED,
    FMT_HEX,
} Print_Fmt_State;

bool is_digit(char c) {
    return c >= '0' && c <= '9';
}

// @TODO: Allow failures here
int read_int_prefix(String str) {
    int value = 0;

    for (u32 i = 0; i < str.length; ++i) {
        if (!is_digit(str.data[i])) break;

        value *= 10;
        value += str.data[i] - '0';
    }

    return value;
}

void io_print_fmt(String str, ...) {
    Print_Fmt_State state = FMT_REGULAR;
    va_list args;
    va_start(args, str);

    for (u32 i = 0; i < str.length; ++i) {
        switch (state) {
            case FMT_REGULAR: {
                if (str.data[i] == '%') {
                    state = FMT_FORMAT;
                }
                else {
                    io_print_char(str.data[i]);
                }
            } break;

            case FMT_FORMAT: {
                switch (str.data[i]) {
                    case '%':
                        io_print_char('%');
                        state = FMT_REGULAR;
                        break;

                    case 's':
                        io_print_string(va_arg(args, String));
                        state = FMT_REGULAR;
                        break;

                    case 'u':
                        state = FMT_UNSIGNED;
                        break;

                    case 'i':
                        state = FMT_SIGNED;
                        break;

                    case 'x':
                        state = FMT_HEX;
                        break;

                    default:
                        io_print_char(str.data[i]);
                        state = FMT_REGULAR;
                        break;
                }
            } break;

            case FMT_UNSIGNED: {
                int size = read_int_prefix(string_after_index(str, i));

                switch (size) {
                    // TODO: I don't think this works. We should be able to print u32s as u8s, probably truncated?
                    case 8:
                        io_print_u8(va_arg(args, u32));
                        break;

                    case 16:
                        io_print_u16(va_arg(args, u32));
                        i++; // Skip over the 16
                        break;

                    case 32:
                        io_print_u32(va_arg(args, u32));
                        i++; // Skip over the 32
                        break;

                    default:
                        io_print_char('u');
                        io_print_char(str.data[i]);
                        break;
                }
                state = FMT_REGULAR;
            } break;

            case FMT_SIGNED: {
                int size = read_int_prefix(string_after_index(str, i));

                switch (size) {
                    case 8:
                        io_print_i8(va_arg(args, i32));
                        break;

                    case 16:
                        io_print_i16(va_arg(args, i32));
                        i++; // Skip over the 16
                        break;

                    case 32:
                        // TODO: Printing the maximum i32 doesn't seem to work for some reason...
                        io_print_i32(va_arg(args, i32));
                        i++; // Skip over the 32
                        break;

                    default:
                        io_print_char('s');
                        io_print_char(str.data[i]);
                        break;
                }
                state = FMT_REGULAR;
            } break;

            case FMT_HEX: {
                int size = read_int_prefix(string_after_index(str, i));

                switch (size) {
                    case 8:
                        io_print_x8(va_arg(args, u32));
                        break;

                    case 16:
                        io_print_x16(va_arg(args, u32));
                        i++; // Skip over the 16
                        break;

                    case 32:
                        io_print_x32(va_arg(args, u32));
                        i++; // Skip over the 32
                        break;

                    default:
                        io_print_char('x');
                        io_print_char(str.data[i]);
                        break;
                }
                state = FMT_REGULAR;
            } break;
        }
    }

    va_end(args);
}


void kmain() {
    vga_set_mode(VGA_MODE_TEXT);
    io_clear_screen();
    io_print_string(from_cstr("------------------------------\n"));
    io_print_string(from_cstr("FORMAT PRINTING TEST\n"));
    io_print_string(from_cstr("------------------------------\n"));
    io_print_string(from_cstr("Strings:\n"));
    io_print_fmt(from_cstr("  String 1 is %s, string 2 is %s\n"), from_cstr("Hello"), from_cstr("World"));
    io_print_string(from_cstr("u8:\n"));
    io_print_fmt(from_cstr("  u8 min is %u8, u8 max is %u8, u8 overflow is %u8\n"), 0, 255, 256);
    io_print_string(from_cstr("u16:\n"));
    io_print_fmt(from_cstr("  u16 min is %u16, u16 max is %u16, u16 overflow is %u16\n"), 0, 65535, 65536);
    io_print_string(from_cstr("u32:\n"));
    io_print_fmt(from_cstr("  u32 min is %u32, u32 max is %u32, u32 overflow is %u32\n"), 0, 4294967295, 4294967296);
    io_print_string(from_cstr("x8:\n"));
    io_print_fmt(from_cstr("  x8 min is %x8, x8 max is %x8, x8 overflow is %x8\n"), 0, 255, 256);
    io_print_string(from_cstr("x16:\n"));
    io_print_fmt(from_cstr("  x16 min is %x16, x16 max is %x16, x16 overflow is %x16\n"), 0, 65535, 65536);
    io_print_string(from_cstr("x32:\n"));
    io_print_fmt(from_cstr("  x32 min is %x32, x32 max is %x32, x32 overflow is %x32\n"), 0, 4294967295, 4294967296);
    io_print_string(from_cstr("i8:\n"));
    io_print_fmt(from_cstr("  i8 min is %i8, i8 max is %i8\n"), -128, 127);
    io_print_string(from_cstr("i16:\n"));
    io_print_fmt(from_cstr("  i16 min is %i16, i16 max is %i16\n"), -32768, 32767);
    io_print_string(from_cstr("i32:\n"));
    io_print_fmt(from_cstr("  i32 min is %i32, i32 max is %i32\n"), -2147483648, 2147483647);
    for(;;);
}
