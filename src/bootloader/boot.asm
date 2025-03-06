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
mov ebx, 0
mov di, 0x500
enumerate_loop:
    clc
    mov eax, 0xe820
    mov edx, 0x534d4150
    mov ecx, 20
    int 0x15
    ; TODO: Implement fallback mechanisms
    jnc enumerate_success
    push enumerate_error_msg
    call error
enumerate_success:
    push bx ; Save for later
    push di ; Save for later
    ; Log the enumerated memory
    ; First push the type of memory
    mov ax, [di + 16]
    push ax
    ; Then push the length in four parts
    ; TODO: QWORD decimal printing
    mov ax, [di + 8]
    push ax
    mov ax, [di + 10]
    push ax
    ; Then push the address in four parts
    mov ax, [di]
    push ax
    mov ax, [di + 2]
    push ax
    push enumerated_memory_msg
    push INFO
    call rm_log
    add sp, 14 
    pop di ; Restore
    pop bx ; Restore

    test ebx, ebx
    je enumerate_done
    add di, cx
    jmp enumerate_loop
enumerate_done:
    cli
    hlt
    
enumerate_error_msg: db "Failed to enumerate memory", CR, LF, 0
enumerated_memory_msg: db "Mem region found. Start: %x%x, Size:%x%x, Type: %x", CR, LF, 0
