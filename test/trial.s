	.text
	.file	"hello.c"
	.globl	foo                             # -- Begin function foo
	.p2align	4, 0x90
	.type	foo,@function
foo:                                    # @foo
	.cfi_startproc
# %bb.0:                                # %entry
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	movl	%edi, -4(%rbp)
	movl	-4(%rbp), %eax
	movl	$2, %ecx
	cltd
	idivl	%ecx
	cmpl	$0, %edx
	jne	.LBB0_2
# %bb.1:                                # %if.then
	movl	-4(%rbp), %eax
	subl	$1, %eax
	movl	%eax, -8(%rbp)
	jmp	.LBB0_3
.LBB0_2:                                # %if.else
	movl	-4(%rbp), %eax
	movl	$2, %ecx
	cltd
	idivl	%ecx
	movl	%eax, -8(%rbp)
.LBB0_3:                                # %return
	movl	-8(%rbp), %eax
	popq	%rbp
	.cfi_def_cfa %rsp, 8
	add $8, %rsp
	cmpq $(.foo_ret_site_0), -8(%rsp)
	je .foo_ret_site_0
	cmpq $(.foo_ret_site_1), -8(%rsp)
	je .foo_ret_site_1
	call _start
.Lfunc_end0:
	.size	foo, .Lfunc_end0-foo
	.cfi_endproc
                                        # -- End function
	.globl	bar                             # -- Begin function bar
	.p2align	4, 0x90
	.type	bar,@function
bar:                                    # @bar
	.cfi_startproc
# %bb.0:                                # %entry
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$16, %rsp
	movl	%edi, -8(%rbp)
	imull	$3, -8(%rbp), %eax
	movl	%eax, -4(%rbp)
	movl	-4(%rbp), %edi
	callq	foo
.foo_ret_site_0:
	addq	$16, %rsp
	popq	%rbp
	.cfi_def_cfa %rsp, 8
	add $8, %rsp
	cmpq $(.bar_ret_site_0), -8(%rsp)
	je .bar_ret_site_0
	cmpq $(.bar_ret_site_1), -8(%rsp)
	je .bar_ret_site_1
	call _start
.Lfunc_end1:
	.size	bar, .Lfunc_end1-bar
	.cfi_endproc
                                        # -- End function
	.globl	fez                             # -- Begin function fez
	.p2align	4, 0x90
	.type	fez,@function
fez:                                    # @fez
	.cfi_startproc
# %bb.0:                                # %entry
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	movl	$2, %edi
	callq	bar
.bar_ret_site_0:
	popq	%rbp
	.cfi_def_cfa %rsp, 8
	retq
.Lfunc_end2:
	.size	fez, .Lfunc_end2-fez
	.cfi_endproc
                                        # -- End function
	.globl	_start                          # -- Begin function _start
	.p2align	4, 0x90
	.type	_start,@function
_start:                                 # @_start
	.cfi_startproc
# %bb.0:                                # %entry
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$32, %rsp
	movl	%edi, -4(%rbp)
	movq	%rsi, -24(%rbp)
	movl	-4(%rbp), %edi
	callq	foo
.foo_ret_site_1:
	movl	%eax, -8(%rbp)
	movl	-4(%rbp), %edi
	callq	bar
.bar_ret_site_1:
	cltq
	movq	%rax, -16(%rbp)
	#APP
	movq	$60, %rax
	syscall

	#NO_APP
	addq	$32, %rsp
	popq	%rbp
	.cfi_def_cfa %rsp, 8
	retq
.Lfunc_end3:
	.size	_start, .Lfunc_end3-_start
	.cfi_endproc
                                        # -- End function
	.ident	"clang version 17.0.0 (https://github.com/llvm/llvm-project.git 603c286334b07f568d39f6706c848f576914f323)"
	.section	".note.GNU-stack","",@progbits
