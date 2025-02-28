; @Function
; Params:
;   ax: Character to print
; Returns:
;   None
print_char:
    mov ah, 0x0e
    mov bh, 0
    int 0x10
    ret

; @Function
; Params:
;   di: Address of string
; Returns:
;   None
print:
.loop:
    mov al, [di]
    test al, al
    je .done
    call print_char
    inc di 
    jmp .loop
.done:
    ret

; @Function
; Params:
;   ax: Number to print
; Returns:
;   None
print_16:
    push ax
    mov ax, '0'
    call print_char
    mov ax, 'x'
    call print_char
    pop ax

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
    mov ax, dx
    call print_char
    pop bx
    pop ax

    shr bx, 4
    sub cx, 4
    jmp .loop
.done:
    ret
