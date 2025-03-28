#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>

typedef int8_t i8;
typedef uint8_t u8;
typedef int16_t i16;
typedef uint16_t u16;
typedef int32_t i32;
typedef uint32_t u32;
typedef memory_index u32;

#define INTERRUPT __attribute__((interrupt))
#define PACKED __attribute__((packed))

// NOTE: This is a unity build, so we're including C files directly
#include "x86.c"
#include "vga.c"

// String code

typedef struct {
    char *data;
    memory_index length;
} String;

memory_index cstring_length(const char *cstr) {
    memory_index length = 0;
    for (const char *p = cstr; *p; ++p) {
        length++;
    }
    return length;
}

String from_cstr(char *literal) {
    return (String){.data = literal, .length = cstring_length(literal)};
}

// TODO: We want to assert that start_index < str.length, but we don't have an assert.
String string_after_index(String str, memory_index start_index) {
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
    for (memory_index i = 0; i < str.length; ++i) {
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

    for (memory_index i = 0; i < str.length; ++i) {
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

    for (memory_index i = 0; i < str.length; ++i) {
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


// PIC code
#define PIC_MASTER_COMMAND 0x20
#define PIC_MASTER_DATA 0x21
#define PIC_SLAVE_COMMAND 0xa0
#define PIC_SLAVE_DATA 0xa1

#define PIC_ICW1_ID 0x10
#define PIC_ICW1_ICW4 0x1

#define PIC_MASTER_ICW3_SLAVE_CONN 0x4
#define PIC_SLAVE_ICW3_ID 0x2

#define PIC_ICW4_FULLY_NESTED 0x10
#define PIC_ICW4_AUTO_EOI 0x2
#define PIC_ICW4_MICRO 0x1

#define PIC_OCW2_NONSPEC_EOI 0x20

void init_pic(u8 master_base, u8 slave_base) {
    x86_io_out8(PIC_MASTER_COMMAND, PIC_ICW1_ID | PIC_ICW1_ICW4);
    x86_io_out8(PIC_SLAVE_COMMAND, PIC_ICW1_ID | PIC_ICW1_ICW4);

    x86_io_out8(PIC_MASTER_DATA, master_base << 3);
    x86_io_out8(PIC_SLAVE_DATA, slave_base << 3);

    x86_io_out8(PIC_MASTER_DATA, PIC_MASTER_ICW3_SLAVE_CONN);
    x86_io_out8(PIC_SLAVE_DATA, PIC_SLAVE_ICW3_ID);

    x86_io_out8(PIC_MASTER_DATA, PIC_ICW4_MICRO);
    x86_io_out8(PIC_SLAVE_DATA, PIC_ICW4_MICRO);
}

void pic_send_eoi(u8 irq) {
    if (irq > 8) {
        x86_io_out8(PIC_SLAVE_COMMAND, PIC_OCW2_NONSPEC_EOI);
    }
    x86_io_out8(PIC_MASTER_COMMAND, PIC_OCW2_NONSPEC_EOI);
}


// Interrupt initialisation code
typedef struct {
    u16 offset_low;
    u16 selector;
    u8 word_count;
    u8 flags;
    u16 offset_high;
} Gate_Descriptor;

typedef struct PACKED {
    u16 size;
    u32 offset;
} Idtr;

volatile Gate_Descriptor *g_idt_base = (Gate_Descriptor*)0xbc00;
volatile Idtr *g_idtr_base = (Idtr*)0xc400;
#define DESC_PRES (1 << 7)
#define DESC_PRIV_LVL3 (3 << 4)
#define DESC_PRIV_LVL0 (0 << 4)
#define DESC_INT_TYPE 0xe
#define DESC_TRAP_TYPE 0xf

typedef struct {
    u32 eip;
    u32 eflags;
} Handler_Stack_Frame;

#define INTERRUPT_STUB(ivt_index)                                       \
INTERRUPT void interrupt_stub_##ivt_index(Handler_Stack_Frame *frame) { \
    io_print_fmt(from_cstr("Index: %u32"), ivt_index);                  \
    x86_halt();                                                         \
}

#define EXCEPTION_STUB(ivt_index)                                                          \
INTERRUPT void exception_stub_##ivt_index(Handler_Stack_Frame *frame, unsigned int code) { \
    io_print_fmt(from_cstr("Index: %u32"), ivt_index);                                     \
    x86_halt();                                                                            \
}

INTERRUPT void pit_interrupt_handler(Handler_Stack_Frame *frame) {
    pic_send_eoi(0);
    return;
}

EXCEPTION_STUB(1);
EXCEPTION_STUB(2);
EXCEPTION_STUB(3);
EXCEPTION_STUB(4);
EXCEPTION_STUB(5);
EXCEPTION_STUB(6);
EXCEPTION_STUB(7);
EXCEPTION_STUB(8);
EXCEPTION_STUB(9);
EXCEPTION_STUB(10);
EXCEPTION_STUB(11);
EXCEPTION_STUB(12);
EXCEPTION_STUB(13);
EXCEPTION_STUB(14);
EXCEPTION_STUB(15);
EXCEPTION_STUB(16);
EXCEPTION_STUB(17);
EXCEPTION_STUB(18);
EXCEPTION_STUB(19);
EXCEPTION_STUB(20);
EXCEPTION_STUB(21);
EXCEPTION_STUB(22);
EXCEPTION_STUB(23);
EXCEPTION_STUB(24);
EXCEPTION_STUB(25);
EXCEPTION_STUB(26);
EXCEPTION_STUB(27);
EXCEPTION_STUB(28);
EXCEPTION_STUB(29);
EXCEPTION_STUB(30);
EXCEPTION_STUB(31);
INTERRUPT_STUB(32);
INTERRUPT_STUB(33);
INTERRUPT_STUB(34);
INTERRUPT_STUB(35);
INTERRUPT_STUB(36);
INTERRUPT_STUB(37);
INTERRUPT_STUB(38);
INTERRUPT_STUB(39);
INTERRUPT_STUB(40);
INTERRUPT_STUB(41);
INTERRUPT_STUB(42);
INTERRUPT_STUB(43);
INTERRUPT_STUB(44);
INTERRUPT_STUB(45);
INTERRUPT_STUB(46);
INTERRUPT_STUB(47);
INTERRUPT_STUB(48);
INTERRUPT_STUB(49);
INTERRUPT_STUB(50);
INTERRUPT_STUB(51);
INTERRUPT_STUB(52);
INTERRUPT_STUB(53);
INTERRUPT_STUB(54);
INTERRUPT_STUB(55);
INTERRUPT_STUB(56);
INTERRUPT_STUB(57);
INTERRUPT_STUB(58);
INTERRUPT_STUB(59);
INTERRUPT_STUB(60);
INTERRUPT_STUB(61);
INTERRUPT_STUB(62);
INTERRUPT_STUB(63);
INTERRUPT_STUB(64);

u32 handler_table[] = {
    (u32)pit_interrupt_handler,
    (u32)exception_stub_1,
    (u32)exception_stub_2,
    (u32)exception_stub_3,
    (u32)exception_stub_4,
    (u32)exception_stub_5,
    (u32)exception_stub_6,
    (u32)exception_stub_7,
    (u32)exception_stub_8,
    (u32)exception_stub_9,
    (u32)exception_stub_10,
    (u32)exception_stub_11,
    (u32)exception_stub_12,
    (u32)exception_stub_13,
    (u32)exception_stub_14,
    (u32)exception_stub_15,
    (u32)exception_stub_16,
    (u32)exception_stub_17,
    (u32)exception_stub_18,
    (u32)exception_stub_19,
    (u32)exception_stub_20,
    (u32)exception_stub_21,
    (u32)exception_stub_22,
    (u32)exception_stub_23,
    (u32)exception_stub_24,
    (u32)exception_stub_25,
    (u32)exception_stub_26,
    (u32)exception_stub_27,
    (u32)exception_stub_28,
    (u32)exception_stub_29,
    (u32)exception_stub_30,
    (u32)exception_stub_31,
    (u32)interrupt_stub_32,
    (u32)interrupt_stub_33,
    (u32)interrupt_stub_34,
    (u32)interrupt_stub_35,
    (u32)interrupt_stub_36,
    (u32)interrupt_stub_37,
    (u32)interrupt_stub_38,
    (u32)interrupt_stub_39,
    (u32)interrupt_stub_40,
    (u32)interrupt_stub_41,
    (u32)interrupt_stub_42,
    (u32)interrupt_stub_43,
    (u32)interrupt_stub_44,
    (u32)interrupt_stub_45,
    (u32)interrupt_stub_46,
    (u32)interrupt_stub_47,
    (u32)interrupt_stub_48,
    (u32)interrupt_stub_49,
    (u32)interrupt_stub_50,
    (u32)interrupt_stub_51,
    (u32)interrupt_stub_52,
    (u32)interrupt_stub_53,
    (u32)interrupt_stub_54,
    (u32)interrupt_stub_55,
    (u32)interrupt_stub_56,
    (u32)interrupt_stub_57,
    (u32)interrupt_stub_58,
    (u32)interrupt_stub_59,
    (u32)interrupt_stub_60,
    (u32)interrupt_stub_61,
    (u32)interrupt_stub_62,
    (u32)interrupt_stub_63,
    (u32)interrupt_stub_64,
};

void fill_idt_entry(volatile Gate_Descriptor *entry, u32 offset, u16 selector, u8 flags) {
    entry->offset_low = offset & 0xffff;
    entry->selector = selector;
    entry->word_count = 0;
    entry->flags = flags;
    entry->offset_high = offset >> 16;
}

void init_idt() {
    // Trap gates for exceptions
    for (int i = 0; i < 32; ++i) {
        fill_idt_entry(
            &g_idt_base[i], handler_table[i],
            0x08, DESC_PRES | DESC_PRIV_LVL0 | DESC_TRAP_TYPE
        );
    }

    for (int i = 32; i < 64; ++i) {
        fill_idt_entry(
            &g_idt_base[i], handler_table[i],
            0x08, DESC_PRES | DESC_PRIV_LVL0 | DESC_INT_TYPE
        );
    }

    g_idtr_base->offset = (u32)g_idt_base;
    g_idtr_base->size = 256 * 8; // 256 8-byte entries

    asm (
        "lidt [0xc400]\n\t"
        "sti"
    );
}

void init_interrupts() {
    init_pic(0x20, 0x30);
    init_idt();
}


void kmain() {
    init_interrupts();
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
