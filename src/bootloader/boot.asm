; ------------------------------
; - Bootloader entry
; ------------------------------

%define CR 0x0d
%define LF 0x0a

bits 16

start:
    cli
    cld

    mov ax, 0
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
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
    jnc stage_2_load_success
    push stage_2_load_error_msg
    call error

stage_2_load_success:
    push load_second_bootloader_msg
    push OK
    call rm_log
    add sp, 4
    jmp stage_2


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

gdt_start:
    dq 0
gdt_kernel_code:
    dw 0xffff        ; Limit 0-15 = 0xffff
    dw 0             ; Base 0-15 = 0
    db 0             ; Base 16-23 = 0
    db 0b10011010    ; Access byte = P(1) DPL(00) S(1) E(1) DC(0) RW(1) A(0)
    db 0xcf          ; Flags = G(1) DB(1) L(0) Reserved(0), Limit 16-19 = 0xf
    db 0             ; Base 24-31 = 0
gdt_kernel_data:
    dw 0xffff        ; Limit 0-15 = 0xffff
    dw 0             ; Base 0-15 = 0
    db 0             ; Base 16-23 = 0
    db 0b10010010    ; Access byte = P(1) DPL(00) S(1) E(0) DC(0) RW(1) A(0)
    db 0xcf          ; Flags = G(1) DB(1) L(0) Reserved(0), Limit 16-19 = 0xf
    db 0             ; Base 24-31 = 0
gdt_user_code:
    dw 0xffff        ; Limit 0-15 = 0xffff
    dw 0             ; Base 0-15 = 0
    db 0             ; Base 16-23 = 0
    db 0b11111010    ; Access byte = P(1) DPL(11) S(1) E(1) DC(0) RW(1) A(0)
    db 0xcf          ; Flags = G(1) DB(1) L(0) Reserved(0), Limit 16-19 = 0xf
    db 0             ; Base 24-31 = 0
gdt_user_data:
    dw 0xffff        ; Limit 0-15 = 0xffff
    dw 0             ; Base 0-15 = 0
    db 0             ; Base 16-23 = 0
    db 0b11110010    ; Access byte = P(1) DPL(11) S(1) E(0) DC(0) RW(1) A(0)
    db 0xcf          ; Flags = G(1) DB(1) L(0) Reserved(0), Limit 16-19 = 0xf
    db 0             ; Base 24-31 = 0
gdt_tss:
    %define TSS_BASE 0x500
    %define TSS_LIMIT 0x6b
    dw (TSS_LIMIT & 0xffff)
    dw (TSS_BASE & 0xffff)
    db (TSS_BASE >> 16)
    db 0b10001001        ; Access byte = P(1) DPL(0) S(0) Type(1001)
    db (TSS_LIMIT >> 16) ; Flags = G(0) DB(0) L(0) Reserved(0)
    db (TSS_BASE >> 24)
gdt_end:

gdtr: 
    dw (gdt_end - gdt_start - 1)
    dd gdt_start

%define CODE_SEG (gdt_kernel_code - gdt_start)
%define DATA_SEG (gdt_kernel_data - gdt_start)

stage_2:
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
    clc
    mov ah, 0x02
    mov al, 5
    mov ch, 0
    mov cl, 3
    mov dh, 0
    mov dl, 0
    mov bx, 0x8000
    int 0x13
    jnc kernel_load_success
    push kernel_load_error_msg
    call error
kernel_load_success:
    push kernel_load_success_msg
    push OK
    call rm_log
    add sp, 4

    lgdt [gdtr]
    mov eax, cr0
    or al, 1
    mov cr0, eax

    ;TODO: qemu has A20 enabled by default. If we want more portability, we need to enable the A20 ourselves (and minimize risk)
    jmp CODE_SEG:pmode

enumerate_error_msg: db "Failed to enumerate memory", CR, LF, 0
enumerated_memory_msg: db "Mem region found. Start: %x%x, Size:%x%x, Type: %x", CR, LF, 0
kernel_load_error_msg: db "Failed to load kernel", CR, LF, 0
kernel_load_success_msg: db "Kernel loaded", CR, LF, 0

bits 32
pmode:
    mov ax, DATA_SEG
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    ;TODO: What do we do with esp/ebp? Do we need a bigger stack here? We have 30KB of stack. Not much.
    jmp 0x8000
