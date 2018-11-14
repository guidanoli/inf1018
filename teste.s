.text
.globl f1, f2, main

f1:
	pushq %rbp
	movq %rsp, %rbp
	subq $32, %rsp
	movl %edi, -28(%rbp)
	call f2
	movl -4(%rbp), %eax
	movl -8(%rbp), %eax
	movl -12(%rbp), %eax
	movl -16(%rbp), %eax
	movl -20(%rbp), %eax
	leave
	ret
	
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
