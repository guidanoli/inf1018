.text
.globl f1, f2, main

f1:
	ret

main:
	movl $32, %edi
	call f1
	movl %eax, -10(%rbp)
	movl $0, %eax
	ret
