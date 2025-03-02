; @Function
; Params:
;   bp + 4: Character to print
; Returns:
;   None
rm_print_char:
    enter 0, 0
    mov ax, [bp + 4]
    mov ah, 0x0e
    mov bh, 0
    int 0x10
    leave
    ret

; @Function
; Params:
;   bp + 4: Address of string
; Returns:
;   None
rm_print_string:
    enter 0, 0
    mov di, [bp + 4]
.loop:
    movzx ax, byte [di]
    test al, al
    je .done
    push ax 
    call rm_print_char
    add sp, 2
    inc di 
    jmp .loop
.done:
    leave
    ret

; @Function
; Params:
;   bp + 4: Number to print
; Returns:
;   None
rm_print_hex_16:
    enter 0, 0
    push '0'
    call rm_print_char
    add sp, 2
    push 'x'
    call rm_print_char
    add sp, 2

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
    push dx
    call rm_print_char
    pop dx
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
    push ax
    call rm_print_char
    add sp, 2
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
    push si
    push di
    push ax
    call rm_print_char
    pop ax
    pop di
    pop si
    jmp .loop
.format:
    inc di
    movzx ax, byte [di]
    cmp al, 'x'
    je .hex
    cmp al, 'd'
    je .dec
    push si
    push di
    push ax
    call rm_print_char
    pop ax
    pop di
    pop si
    jmp .loop
.hex:
    mov ax, [bp + si]
    push si
    push di
    push ax
    call rm_print_hex_16
    pop ax
    pop di
    pop si
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
    leave
    ret
