; ------------------------------
; - Bootloader entry
; ------------------------------

%define CR 0x0d
%define LF 0x0a

bits 16

start:
    mov ax, 0
    mov es, ax
    jmp boot

msg: db "Welcome to OSTut!", CR, LF, 0

%include "io.asm"

boot:
    mov di, msg
    call print

    int 0x12
    ; AX = # of KB before EBDA
    call print_16

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
    cld
    hlt

; Boot signature
times 510 - ($ - $$) db 0
dw 0xaa55
