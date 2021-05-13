
	.extern running_thread
	.extern cleanup_thread

	.text
	
	.global context_switch
	.global internal_exit
	
context_switch:

	pushq %rbp
	pushq %rbx
	pushq %r12
	pushq %r13
	pushq %r14
	pushq %r15

	movq %rsp, (%rdi)
	
	movq %rsi, running_thread(%rip)    # running_thread = nextThread
	
	movq (%rsi), %rsp
	
	popq %r15
	popq %r14
	popq %r13
	popq %r12
	popq %rbx
	popq %rbp
	
	ret

internal_exit:

	movq %rsi, running_thread(%rip)    # running_thread = nextThread
	
	movq (%rsi), %rsp
	
	call cleanup_thread
	
	popq %r15
	popq %r14
	popq %r13
	popq %r12
	popq %rbx
	popq %rbp
	
	ret
