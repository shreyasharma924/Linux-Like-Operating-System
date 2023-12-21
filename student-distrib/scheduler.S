#define ASM     1

.globl scheduler_context_switch

scheduler_context_switch:
    movl  4(%esp) , %ebp                   
    movl  8(%esp), %esp 
    leave
    ret                         
    
