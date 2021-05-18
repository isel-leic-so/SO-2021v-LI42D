
.extern running_thread
.extern internal_cleanup 

.global context_switch
.global internal_exit

/*
 * void context_switch(uthread_t * currThread, uthread_t * nextThread) 
 *
 * in gcc linux 64 bits C/C++ compiler convention 
 * registers saved by callee are:
 *   R12
 *   R13
 *   R14
 *   R15
 *   RBX
 *   RBP
 *
 * Parameters are passed by register:
 *   RDI : currThread
 *	 RSI : nextThread
 *
 */
context_switch:
 
	#
	# Switch out the running currThread, saving the execution context
	# on the thread's own stack. The return address is atop the stack,
	# having been placed there by the call to this function.
	#
	 
	push %rbp
	push %rbx
	push %r12
	push %r13
	push %r14
	push %r15
	
	#
	# Save ESP in currThread->context
	#
	mov	%rsp, (%rdi)  

	#
	# Set nextThread as the running thread.
	#
	# Note the access relative to RIP (PIC Code)
	#
	mov %rsi, running_thread(%rip) 

	#
	# Load nextThread's context, starting by switching to its stack,
	# where the registers are saved.
	#
	mov (%rsi), %rsp 

	#
	# Restore nextThread's context
	#
	pop	%r15
	pop	%r14
	pop	%r13
	pop	%r12
	pop	%rbx
	pop	%rbp

	#
	# Jump to the return address saved on nextThread's stack when the
	# function was called.
	#
	ret

 

/*
 * void internal_exit(uthread_t * currThread, uthread_t * nextThread)
 */
internal_exit: 
	
	#
	# Set nextThread as the running thread.
	#
	mov %rsi, running_thread(%rip) 
		
	#
	# Load nextThread's stack pointer before calling ut_cleanup.
	# Making the call while still using currThread's stack would mean
	# using the same memory being freed -- the stack.
	#
	mov	(%rsi), %rsp 

	#
	# Free currThread resources.
	#
	call internal_cleanup
	
	#
	# Restore nextThread's context
	#
	pop	%r15
	pop	%r14
	pop	%r13
	pop	%r12
	pop	%rbx
	pop	%rbp

	ret
