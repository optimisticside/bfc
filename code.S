.section .data
	buf: .zero 3000

.section .text
_start:
	pushq %rbp
	movq %rsp, %rbp
	movq $buf, %rax
	incq %rax
	incq (%rax)
	incq (%rax)
	incq (%rax)
	incq (%rax)
	incq (%rax)
	incq (%rax)
	incq (%rax)
	incq (%rax)
loop1:
	decq %rax
	incq (%rax)
	incq (%rax)
	incq (%rax)
	incq (%rax)
	incq (%rax)
	incq (%rax)
	incq (%rax)
	incq (%rax)
	incq (%rax)
	incq %rax
	decq (%rax)
	cmp 0, %eax
	jle loop1
	decq %rax
	pushq %rax
	call _getchar
	movq %rax, %rbx
	popq %rax
	movq %rbx, (%rax)
	incq %rax
	incq (%rax)
	incq (%rax)
	incq (%rax)
	incq (%rax)
loop2:
	decq %rax
	incq (%rax)
	incq (%rax)
	incq (%rax)
	incq (%rax)
	incq (%rax)
	incq (%rax)
	incq (%rax)
	incq %rax
	decq (%rax)
	cmp 0, %eax
	jle loop2
	decq %rax
	incq (%rax)
	pushq %rax
	call _getchar
	movq %rax, %rbx
	popq %rax
	movq %rbx, (%rax)
	incq (%rax)
	incq (%rax)
	incq (%rax)
	incq (%rax)
	incq (%rax)
	incq (%rax)
	incq (%rax)
	pushq %rax
	call _getchar
	movq %rax, %rbx
	popq %rax
	movq %rbx, (%rax)
	pushq %rax
	call _getchar
	movq %rax, %rbx
	popq %rax
	movq %rbx, (%rax)
	incq (%rax)
	incq (%rax)
	incq (%rax)
	pushq %rax
	call _getchar
	movq %rax, %rbx
	popq %rax
	movq %rbx, (%rax)
	incq %rax
	incq %rax
	incq (%rax)
	incq (%rax)
	incq (%rax)
	incq (%rax)
	incq (%rax)
	incq (%rax)
loop3:
	decq %rax
	incq (%rax)
	incq (%rax)
	incq (%rax)
	incq (%rax)
	incq (%rax)
	incq (%rax)
	incq (%rax)
	incq %rax
	decq (%rax)
	cmp 0, %eax
	jle loop3
	decq %rax
	incq (%rax)
	incq (%rax)
	pushq %rax
	call _getchar
	movq %rax, %rbx
	popq %rax
	movq %rbx, (%rax)
	decq (%rax)
	decq (%rax)
	decq (%rax)
	decq (%rax)
	decq (%rax)
	decq (%rax)
	decq (%rax)
	decq (%rax)
	decq (%rax)
	decq (%rax)
	decq (%rax)
	decq (%rax)
	pushq %rax
	call _getchar
	movq %rax, %rbx
	popq %rax
	movq %rbx, (%rax)
	incq %rax
	incq (%rax)
	incq (%rax)
	incq (%rax)
	incq (%rax)
	incq (%rax)
	incq (%rax)
loop4:
	decq %rax
	incq (%rax)
	incq (%rax)
	incq (%rax)
	incq (%rax)
	incq (%rax)
	incq (%rax)
	incq (%rax)
	incq (%rax)
	incq (%rax)
	incq %rax
	decq (%rax)
	cmp 0, %eax
	jle loop4
	decq %rax
	incq (%rax)
	pushq %rax
	call _getchar
	movq %rax, %rbx
	popq %rax
	movq %rbx, (%rax)
	decq %rax
	pushq %rax
	call _getchar
	movq %rax, %rbx
	popq %rax
	movq %rbx, (%rax)
	incq (%rax)
	incq (%rax)
	incq (%rax)
	pushq %rax
	call _getchar
	movq %rax, %rbx
	popq %rax
	movq %rbx, (%rax)
	decq (%rax)
	decq (%rax)
	decq (%rax)
	decq (%rax)
	decq (%rax)
	decq (%rax)
	pushq %rax
	call _getchar
	movq %rax, %rbx
	popq %rax
	movq %rbx, (%rax)
	decq (%rax)
	decq (%rax)
	decq (%rax)
	decq (%rax)
	decq (%rax)
	decq (%rax)
	decq (%rax)
	decq (%rax)
	pushq %rax
	call _getchar
	movq %rax, %rbx
	popq %rax
	movq %rbx, (%rax)
	incq %rax
	incq %rax
	incq %rax
	incq (%rax)
	incq (%rax)
	incq (%rax)
	incq (%rax)
loop5:
	decq %rax
	incq (%rax)
	incq (%rax)
	incq (%rax)
	incq (%rax)
	incq (%rax)
	incq (%rax)
	incq (%rax)
	incq (%rax)
	incq %rax
	decq (%rax)
	cmp 0, %eax
	jle loop5
	decq %rax
	incq (%rax)
	pushq %rax
	call _getchar
	movq %rax, %rbx
	popq %rax
	movq %rbx, (%rax)
	movq %rbp, %rsp
	popq %rbp
	ret