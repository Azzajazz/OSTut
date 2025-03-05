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
load_second_bootloader_msg: db "Bootloader stage 2 loaded", CR, LF, 0
stage_2_running_msg: db "Running bootloader stage 2", CR, LF, 0
stage_2_load_error_msg: db "Failed to load stage 2", CR, LF, 0

%include "io.asm"

boot:
    push welcome_msg
    call rm_print_fmt_16
    add sp, 2

    clc
    mov ah, 0x02
    mov al, 1
    mov ch, 0
    mov cl, 2
    mov dh, 0
    mov dl, 0
    mov bx, 0x7e00
    int 0x13
    jnc .stage_2_load_success
    push stage_2_load_error_msg
    call error

.stage_2_load_success:
    push load_second_bootloader_msg
    push OK
    call rm_log
    add sp, 4

    jmp 0x7e00

    cli
    hlt

; @Function
; Reporting for fatal errors
; Params:
;   bp + 4: Error message
; Returns:
;   None
error:
    push ERROR
    call rm_log
    cli
    hlt

; Boot signature
times 510 - ($ - $$) db 0
dw 0xaa55

; ------------------------------
; - Bootloader stage 2
; ------------------------------

push stage_2_running_msg
push INFO
call rm_log
add sp, 4

; Enumerate memory
