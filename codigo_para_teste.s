.text
.globl main

main:
	pushq %rbp
	movq %rsp,%rbp
	subq $32,%rsp
	
	imull $128, %r10d
	
	leave
	ret
	
	
	
	
	
	
	
	
	
	

