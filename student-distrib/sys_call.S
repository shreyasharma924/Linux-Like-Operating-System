#define ASM     1
  

.globl jump_on_it


jump_on_it:

    cmp $10, %EAX 
	jg invalidCallNum # sys_call num is invalid, so return -1
    cmp $1, %EAX
    jl invalidCallNum # sys_call num  is invalid, so return -1

    pushl %ebp
    pushl %edi
    pushl %esi
    pushl %edx
    pushl %ecx 
    pushl %ebx
    pushfl
    pushl %esp

    pushl %edx
    pushl %ecx 
    pushl %ebx
	decl %eax

	call *sys_call_jump_table(, %eax,4) # since CMD is valid, go to and execute the specific cmd
    
    addl $12, %esp

    popl %esp 
    popfl
    popl %ebx
    popl %ecx                                            
    popl %edx 
    popl %esi
    popl %edi
    popl %ebp
    iret
  
	invalidCallNum:
		movl $-1,%eax
		IRET

sys_call_jump_table:
	.long halt_blossom_handler, execute_bubbles_handler, read_mojojojo_handler, write_Utonium_handler, open_buttercup_handler, close_FuzzyLumpkins_handler, getargs, vidmap, set_handler, sigreturn

.text

.globl halt_context
halt_context:
    movl 4(%esp), %ebp 
    movl 8(%esp), %eax
    leave
    ret                         
    
