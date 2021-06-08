.intel_syntax noprefix

.text
.global syscall2
.global syscall3
.global syscall4
.global syscall_str4

/* rdi, rsi */
syscall2:
    mov rax, rdi
    mov rdi, rsi
    syscall
    ret
    
/* rdi, rsi, rdx */
syscall3:
    mov rax, rdi
    mov rdi, rsi
    mov rsi, rdx
    syscall
    ret
    
/* rdi, rsi, rdx, rcx */
syscall4:
    mov rax, rdi
    mov rdi, rsi
    mov rsi, rdx
    mov rdx, rcx
    syscall
    ret
    
syscall_str4:
    call syscall4
    ret
    
