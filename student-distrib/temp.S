#define ASM     1

.globl context_switch

context_switch:
    movl 4(%esp), %ebp 
    movl 8(%esp), %eax
    leave
    ret                         
    
