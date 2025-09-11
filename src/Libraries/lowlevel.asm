global printchar
global scanf
section .data
section .bss
section .text
printchar:
    mov rsi, rdi
    mov rax, 1
    mov rdi, 1
    mov rdx, 1
    syscall
    ret
scanf:
    mov rsi, rdi
    mov rax, 0
    mov rdi, 0
    mov rdx, 64
    syscall
    mov byte [rsi+rax-1], 0
    ret