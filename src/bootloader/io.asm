%macro print_char 1
    mov al, %1
    mov ah, 0x0e
    mov bh, 0
    int 0x10
%endmacro

; @Function
; Params:
;   bp + 4: Number to print
; Returns:
;   None
rm_print_hex_16:
    enter 0, 0
    print_char '0'
    print_char 'x'

    mov ax, [bp + 4]
    mov bx, 0xf000 ; Mask
    mov cx, 12 ; Shift
.loop:
    test bx, bx
    je .done
    mov dx, ax
    and dx, bx
    shr dx, cl
    cmp dx, 10
    jl .number
    add dx, 'a' - 10 - '0'
.number:
    add dx, '0'

.print:
    push ax
    push bx
    print_char dl
    pop bx
    pop ax

    shr bx, 4
    sub cx, 4
    jmp .loop
.done:
    leave
    ret

dec_print: db "     "

; @Function
; Params:
;   bp + 4: Number to print
; Returns:
;   None
rm_print_dec_16:
    enter 0, 0
    mov ax, [bp + 4]
    mov bx, dec_print
    mov di, 4 ; Index into dec_print
    mov cx, 10 ; Dividend
.store_loop:
    test ax, ax
    je .print_loop
    mov dx, 0 ; Top of dividend
    div cx ; dx = remainder, ax = quotient
    add dl, '0'
    mov [bx + di], dl
    dec di
    jmp .store_loop
.print_loop:
    inc di
    cmp di, 5
    je .done
    movzx ax, byte [bx + di]
    push bx
    print_char al
    pop bx
    jmp .print_loop
.done:
    leave
    ret

; @Function
; Real mode format printing. Supports the following formats:
;   - %x: print hexadecimal format
;   - %d: print decimal format
; Params:
;   bp + 4: Format string
;   bp + 4 * (n + 1): nth format argument 
rm_print_fmt_16:
    enter 0, 0
    call rm_vprint_fmt_16
    leave
    ret

; @Function
; varargs version of rm_print_fmt_16. This does not reset bp
rm_vprint_fmt_16:
    mov di, [bp + 4]
    mov si, 6 ; Index into format arguments
    dec di
.loop:
    inc di
    movzx ax, byte [di]
    test al, al
    je .done
    cmp al, '%'
    je .format
    print_char al
    jmp .loop
.format:
    inc di
    movzx ax, byte [di]
    cmp al, 'x'
    je .hex
    cmp al, 'd'
    je .dec
    print_char al
    jmp .loop
.hex:
    mov ax, [bp + si]
    print_char al
    add si, 2
    jmp .loop
.dec:
    mov ax, [bp + si]
    push si
    push di
    push ax
    call rm_print_dec_16
    pop ax
    pop di
    pop si
    add si, 2
    jmp .loop
.done:
    ret

info_pref: db "INFO", 0
ok_pref: db "OK", 0
warn_pref: db "WARN", 0
error_pref: db "ERROR", 0

%define INFO 0
%define OK 1
%define WARN 2
%define ERROR 3
; @Function
; Logging to console in real mode
; Params:
;   bp + 4: Log level (0 = info, 1 = OK, 2 = warn, 3 = error)
;   bp + 6: Format string
;   bp + 8...: Format string arguments
rm_log:
    enter 0, 0
    mov al, '['
    mov ah, 0x0e
    mov bh, 0
    int 0x10
    mov ax, [bp + 4]
    cmp ax, INFO 
    je .info
    cmp ax, OK
    je .ok
    cmp ax, WARN
    je .warn
    ; Error
    push error_pref
    jmp .print_pref
.info:
    push info_pref
    jmp .print_pref
.ok:
    push ok_pref
    jmp .print_pref
.warn:
    push warn_pref
.print_pref:
    call rm_print_fmt_16
    add sp, 2

    mov al, ']'
    mov ah, 0x0e
    mov bh, 0
    int 0x10

    mov al, ' '
    mov ah, 0x0e
    mov bh, 0
    int 0x10
    
    add bp, 2
    call rm_vprint_fmt_16
    sub bp, 2

    leave
    ret
