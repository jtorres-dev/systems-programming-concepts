	.file	"hello.i"
	.intel_syntax noprefix
	.text
	.section	.rodata
.LC0:
	.string	"Hello world."
	.text
	.globl	main
	.type	main, @function
main:
.LFB0:
	.cfi_startproc
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	sub	rsp, 16			#the space for int retval is created
	mov	DWORD PTR -4[rbp], 67   #value is placed in retval
	lea	rdi, .LC0[rip]
	call	puts@PLT		#printf is called
	mov	eax, DWORD PTR -4[rbp]
	leave				#rbp is popped off and returned
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 7.4.0-1ubuntu1~18.04.1) 7.4.0"
	.section	.note.GNU-stack,"",@progbits
