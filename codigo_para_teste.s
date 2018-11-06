.text
.globl main

main:
	pushq %rbp
	movq %rsp,%rbp
	subq $32,%rsp
	movl $5555,-4(%rbp)
	imull $5,%eax
	imull $35,%edi
	imull $35,%esi
	
	leave
	ret
	
	
	
	
	
	
	
	
	
	

