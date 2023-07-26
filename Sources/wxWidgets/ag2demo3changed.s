	.file	"ag2demo3.for"
	.text
	.def	__powisf2;	.scl	2;	.type	32;	.endef
	.section .rdata,"dr"
	.align 4
.LC1:
	.long	120
	.align 4
.LC2:
	.long	1
	.align 4
.LC3:
	.long	2
	.align 4
.LC4:
	.long	0
	.text
	.globl	MAIN__  /* <-------------- Manually added ------------------ */
	.def	MAIN__;	.scl	3;	.type	32;	.endef
	.seh_proc	MAIN__
MAIN__:
	pushq	%rbp
	.seh_pushreg	%rbp
	pushq	%rbx
	.seh_pushreg	%rbx
	subq	$152, %rsp
	.seh_stackalloc	152  /* <----- use $152 in return block below -----> */
	leaq	144(%rsp), %rbp
	.seh_setframe	%rbp, 144
	.seh_endprologue
	movss	.LC0(%rip), %xmm0
	movss	%xmm0, -64(%rbp)
	movss	.LC0(%rip), %xmm0
	movss	%xmm0, -112(%rbp)
	movl	$2, -4(%rbp)
.L3:
	cmpl	$11, -4(%rbp)
	setg	%al
	movzbl	%al, %eax
	testl	%eax, %eax
	jne	.L4
	movl	-4(%rbp), %eax
	leal	-1(%rax), %edx
	movl	-4(%rbp), %eax
	cltq
	subq	$1, %rax
	pxor	%xmm0, %xmm0
	cvtsi2ssl	%edx, %xmm0
	movss	%xmm0, -64(%rbp,%rax,4)
	movl	-4(%rbp), %eax
	subl	$1, %eax
	pxor	%xmm1, %xmm1
	cvtsi2ssl	%eax, %xmm1
	movd	%xmm1, %eax
	movl	-4(%rbp), %edx
	movslq	%edx, %rdx
	leaq	-1(%rdx), %rbx
	movl	$3, %edx
	movd	%eax, %xmm0
	call	__powisf2
	movd	%xmm0, %eax
	movl	%eax, -112(%rbp,%rbx,4)
	addl	$1, -4(%rbp)
	jmp	.L3
.L4:
	nop
	leaq	.LC1(%rip), %rcx
	call	initt_
	call	binitt_
	leaq	-112(%rbp), %rdx
	leaq	-64(%rbp), %rax
	movq	%rax, %rcx
	call	check_
	leaq	-112(%rbp), %rdx
	leaq	-64(%rbp), %rax
	movq	%rax, %rcx
	call	dsplay_
	movl	$0, -8(%rbp)
	leaq	.LC2(%rip), %rcx
	call	symbl_
	leaq	.LC3(%rip), %rcx
	call	ylab_
	leaq	.LC3(%rip), %rcx
	call	ytype_
	leaq	-8(%rbp), %rax
	movq	%rax, %rcx
	call	ylocrt_
	leaq	.LC4(%rip), %rcx
	call	xlab_
	leaq	.LC4(%rip), %rcx
	call	xfrm_
	leaq	.LC3(%rip), %rcx
	call	yfrm_
	leaq	.LC4(%rip), %rdx
	leaq	.LC4(%rip), %rcx
	call	dlimy_
	leaq	-112(%rbp), %rdx
	leaq	-64(%rbp), %rax
	movq	%rax, %rcx
	call	check_
	leaq	-112(%rbp), %rdx
	leaq	-64(%rbp), %rax
	movq	%rax, %rcx
	call	dsplay_
	call	finitt_
/*	movl	$0, %r8d  <----------- Delete manually --------------------
*	movl	$0, %edx
*	movl	$0, %ecx
*	call	_gfortran_stop_string
*	nop       -------------------  Delete manually --------------------> */
	addq	$152, %rsp /* Manually added, $152 from seh_stackalloc above */
	popq	%rbx       /* Manually added, see prologue above             */
	popq	%rbp       /* Manually added, see prologue above             */
	ret                /* Manually added                                 */
	.seh_endproc
/*	.def	__main;	.scl	2;	.type	32;	.endef <- Delete manually --
*	.globl	main
*	.def	main;	.scl	2;	.type	32;	.endef
*	.seh_proc	main
main:
*	pushq	%rbp
*	.seh_pushreg	%rbp
*	movq	%rsp, %rbp
*	.seh_setframe	%rbp, 0
*	subq	$32, %rsp
*	.seh_stackalloc	32
*	.seh_endprologue
*	movl	%ecx, 16(%rbp)
*	movq	%rdx, 24(%rbp)
*	call	__main
*	movq	24(%rbp), %rax
*	movq	%rax, %rdx
*	movl	16(%rbp), %ecx
*	call	_gfortran_set_args
*	leaq	options.0.0(%rip), %rdx
*	movl	$7, %ecx
*	call	_gfortran_set_options
*	call	MAIN__
*	movl	$0, %eax
*	addq	$32, %rsp
*	popq	%rbp
*	ret
*	.seh_endproc  -------------------  Delete manually ----------------> */
	.section .rdata,"dr"
	.align 16
options.0.0:
	.long	2116
	.long	4095
	.long	0
	.long	1
	.long	1
	.long	0
	.long	31
	.align 4
.LC0:
	.long	1092616192
	.ident	"GCC: (tdm64-1) 10.3.0"
	.def	initt_;	.scl	2;	.type	32;	.endef
	.def	binitt_;	.scl	2;	.type	32;	.endef
	.def	check_;	.scl	2;	.type	32;	.endef
	.def	dsplay_;	.scl	2;	.type	32;	.endef
	.def	symbl_;	.scl	2;	.type	32;	.endef
	.def	ylab_;	.scl	2;	.type	32;	.endef
	.def	ytype_;	.scl	2;	.type	32;	.endef
	.def	ylocrt_;	.scl	2;	.type	32;	.endef
	.def	xlab_;	.scl	2;	.type	32;	.endef
	.def	xfrm_;	.scl	2;	.type	32;	.endef
	.def	yfrm_;	.scl	2;	.type	32;	.endef
	.def	dlimy_;	.scl	2;	.type	32;	.endef
	.def	finitt_;	.scl	2;	.type	32;	.endef
	.def	_gfortran_stop_string;	.scl	2;	.type	32;	.endef
	.def	_gfortran_set_args;	.scl	2;	.type	32;	.endef
	.def	_gfortran_set_options;	.scl	2;	.type	32;	.endef
