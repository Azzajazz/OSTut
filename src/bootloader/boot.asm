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
newline_msg: db CR, LF, 0

%include "io.asm"

boot:
    push welcome_msg
    call rm_print_fmt_16
    add sp, 2

    int 0x12
    push ax
    push memory_enumerate_msg
    call rm_print_fmt_16
    add sp, 4

    mov ah, 0x02
    mov al, 1
    mov ch, 0
    mov cl, 2
    mov dh, 0
    mov dl, 0
    mov bx, 0x50
    int 0x13
    jmp 0x500

    cli
    hlt

; Boot signature
times 510 - ($ - $$) db 0
dw 0xaa55
