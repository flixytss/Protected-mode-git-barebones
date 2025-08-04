[org 0x7C00]
bits 16

jmp start
nop

start:
    cli
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00

    ; Modo texto 80x25
    mov ax, 0x0003
    int 0x10

    mov ah, 0x02
    mov al, 14
    mov ch, 0
    mov cl, 2
    mov dh, 0
    mov dl, 0x00
    mov bx, 0x1000
    mov es, bx
    xor bx, bx
    int 0x13
    jc disk_error


    cli

    lgdt [gdt_descriptor]

    ; Activar modo protegido
    mov eax, cr0
    or eax, 1
    mov cr0, eax

    ; Far jump para limpiar prefetch y cargar CS con segmento código (0x08)
    jmp 0x08:protected_mode

disk_error:
    mov si, err_msg
.print_err:
    lodsb
    or al, al
    jz .done
    mov ah, 0x0E
    int 0x10
    jmp .print_err
.done:
    jmp $

err_msg db 'Disk Error!', 0

gdt_start:
    dq 0                      ; Null descriptor

    dw 0xFFFF                 ; Código segmento (base 0, límite 4GB)
    dw 0x0000
    db 0x00
    db 10011010b              ; Code segment, executable, readable, accessed=0, DPL=0, Present=1
    db 11001111b              ; 4KiB granularity, 32-bit protected mode
    db 0x00

    dw 0xFFFF                 ; Datos segmento (base 0, límite 4GB)
    dw 0x0000
    db 0x00
    db 10010010b              ; Data segment, writable, accessed=0, DPL=0, Present=1
    db 11001111b
    db 0x00

gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start -1
    dd gdt_start

[bits 32]
protected_mode:
    ; Cargar segmentos de datos
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov esp, 0x90000

    ; Salto far al kernel en offset 0 (base 0)
    call dword 0x10000

    hlt
    jmp $

times 510-($-$$) db 0
dw 0xAA55
