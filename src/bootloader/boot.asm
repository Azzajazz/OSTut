; ------------------------------
; - Bootloader entry
; ------------------------------

%define CR 0x0d
%define LF 0x0a

bits 16

start:
    mov ax, 0
    mov es, ax
    mov bp, 0x7c00
    mov sp, bp
    jmp boot

welcome_msg: db "Bootloading OSTut", CR, LF, 0
memory_enumerate_msg: db "Found %dKB before EBDA", CR, LF, 0
tst_msg: db "%d %x %d", CR, LF, 0
newline_msg: db CR, LF, 0

%include "io.asm"

boot:
    push 123 
    push 0xfff
    push 65535
    push tst_msg
    call rm_print_fmt_16
    add sp, 8
    push 'a'
    call rm_print_char
    add sp, 2
    push welcome_msg
    call rm_print_string
    add sp, 2
    push 0x4555
    call rm_print_hex_16
    add sp, 2
    push newline_msg
    call rm_print_string
    add sp, 2
    push 123
    call rm_print_dec_16
    add sp, 2
    push newline_msg
    call rm_print_string
    add sp, 2

    ;mov di, welcome_msg
    ;call rm_print_fmt_16

    ;int 0x12
    ;push ax
    ;mov di, memory_enumerate_msg
    ;call rm_print_fmt_16
    ;add sp, 2

    ;mov ah, 0x02
    ;mov al, 1
    ;mov ch, 0
    ;mov cl, 2
    ;mov dh, 0
    ;mov dl, 0
    ;mov bx, 0x50
    ;int 0x13
    ;jmp 0x500

    cli
    hlt

; Boot signature
times 510 - ($ - $$) db 0
dw 0xaa55
