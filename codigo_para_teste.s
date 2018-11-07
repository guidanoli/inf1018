.text
.globl main

main:
	pushq %rbp
	movq %rsp,%rbp
	subq $32,%rsp
	movl $0, -4(%rbp)
	
	cmpl $0, -4(%rbp)
	je algo
	
algo:
	leave
	ret
