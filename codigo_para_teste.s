.text
.globl main

main:
	pushq %rbp
	movq %rsp,%rbp
	subq $32, %rbp
	
	movl $54654, %eax
	movl -4(%eax), %eax
	
	cmpl $0,%eax
	jne foo
	
	leave 
	ret
	

