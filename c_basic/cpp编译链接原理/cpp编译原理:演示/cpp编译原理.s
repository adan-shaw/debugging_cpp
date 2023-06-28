	.file	"cpp\347\274\226\350\257\221\345\216\237\347\220\206.cpp"
	.text
	.globl	m_G_info
	.bss
	.align 16
	.type	m_G_info, @object
	.size	m_G_info, 16
m_G_info:
	.zero	16
	.globl	m_G_body
	.align 16
	.type	m_G_body, @object
	.size	m_G_body, 16
m_G_body:
	.zero	16
	.globl	G_val
	.align 4
	.type	G_val, @object
	.size	G_val, 4
G_val:
	.zero	4
	.globl	G_str
	.data
	.align 32
	.type	G_str, @object
	.size	G_str, 64
G_str:
	.string	"hello world"
	.zero	52
	.globl	G_arr
	.align 16
	.type	G_arr, @object
	.size	G_arr, 20
G_arr:
	.long	1
	.long	2
	.long	3
	.long	4
	.long	5
	.section	.rodata
.LC0:
	.string	"\"suck a cock adan\""
.LC1:
	.string	"%s\n\n"
.LC2:
	.string	"%s: %d%d%d%d%d\n\n"
	.align 8
.LC3:
	.string	"struct G_info=%d\345\255\227\350\212\202  class G_body=%d\345\255\227\350\212\202\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB0:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movl	%edi, -4(%rbp)
	movq	%rsi, -16(%rbp)
	leaq	.LC0(%rip), %rsi
	leaq	.LC1(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	16+G_arr(%rip), %ecx
	movl	12+G_arr(%rip), %edi
	movl	8+G_arr(%rip), %esi
	movl	4+G_arr(%rip), %edx
	movl	G_arr(%rip), %eax
	subq	$8, %rsp
	pushq	%rcx
	movl	%edi, %r9d
	movl	%esi, %r8d
	movl	%edx, %ecx
	movl	%eax, %edx
	leaq	G_str(%rip), %rsi
	leaq	.LC2(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	addq	$16, %rsp
	movl	$16, %edx
	movl	$16, %esi
	leaq	.LC3(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	$0, %eax
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	main, .-main
	.ident	"GCC: (Debian 8.3.0-6) 8.3.0"
	.section	.note.GNU-stack,"",@progbits
