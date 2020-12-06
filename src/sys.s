.intel_syntax noprefix

.globl read
read:
  mov rdx, rsi
  mov rsi, rdi
  mov rdi, 0
  mov rax, 0
  syscall
  ret

.globl write
write:
  mov rdx, rsi
  mov rsi, rdi
  mov rdi, 1
  mov rax, 1
  syscall
  ret

.globl eWrite
eWrite:
  mov rdx, rsi
  mov rsi, rdi
  mov rdi, 2
  mov rax, 1
  syscall
  ret

.globl sysExit
sysExit:
  mov rax, 60
  syscall
  ret

.globl _start
_start:
  call main
  mov rdi, rax
  mov rax, 60
  syscall
