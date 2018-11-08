.text
.globl main

main:
	pushq %rbp
	movq %rsp,%rbp
	subq $32, %rbp

	movl %r10d, -4(%rbp)
	
	movl $0,%eax
	leave 
	ret
	

