.text
.globl f1, f2, main

f1:
	ret

main:
	movl -28(%rbp), %r10d
	addl -28(%rbp), %r10d
	subl -28(%rbp), %r10d
	imull -28(%rbp), %r10d
	movl $0, %eax
	ret
