.text
.globl main

main:
	pushq %rbp
	movq %rsp,%rbp
	subq $32,%rsp
	
	subl $127, %r10d
	
	leave
	ret
	
	
	
	
	
	
	
	
	
	

