#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>

typedef int8_t i8;
typedef uint8_t u8;
typedef int16_t i16;
typedef uint16_t u16;
typedef int32_t i32;
typedef uint32_t u32;

// NOTE: This is a unity build, so we're including C files directly
#include "x86.c"
#include "vga.c"

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
    u32 to_print = *(u32*)&value;
    if (value < 0) {
        io_print_char('-');
        to_print = ~to_print + 1;
    }
    io_print_u32(to_print);
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
    // NOTE: -2147483648 is not a literal. It is the unary negation applied to the literal 2147483648.
    // This literal is too big to fit in an i32 and results in UB when used in this context. Hence we use
    // -2147483647 - 1 instead.
    // TODO: Add I32_MIN, I32_MAX, etc.
    io_print_fmt(from_cstr("  i32 min is %i32, i32 max is %i32\n"), -2147483647 - 1, 2147483647);
    for(;;);
}
