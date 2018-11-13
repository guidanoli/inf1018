.text
.globl main

main:
	/* zret $cte $cte */
	movl $12313, %r10d
	cmpl $0, %r10d
	jne label
	movl $998, %eax
	leave
	ret
	
	/* zret $cte pilha */
	movl $999, %r10d
	cmpl $0, %r10d
	jne label
	movl -28(%rbp), %eax
	leave
	ret
	
	/* zret pilha $cte */
	cmpl $0, -28(%rbp)
	jne label
	movl $999, %eax
	leave
	ret
	
	/* zret pilha pilha */
	cmpl $0, -28(%rbp)
	jne label
	movl -28(%rbp), %eax
	leave
	ret
	label:
