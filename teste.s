.text
.globl f1, f2, main

f1:
	pushq %rbp
	movq %rsp, %rbp
	subq $32, %rsp
	movl %edi, -28(%rbp)
	call f2
	movl -28(%rbp), %r10d
	cmpl $0, %r10d
	movl %r10d, %eax
	jne label
	leave
	ret
label:
	
f2:
	pushq %rbp
	movq %rsp, %rbp
	subq $32, %rsp
	movl %edi, -28(%rbp)
	movl $1024, %eax
	leave
	ret
	
main:
	call f1
	movl $0, %eax
	ret
