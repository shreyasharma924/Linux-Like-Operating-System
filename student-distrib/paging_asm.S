#define ASM     1
.text
.globl load_page_directory, flush_TLB


# Description: load_page_directory 
# Input: None
# Output: None
# Side Effects: loads and sets the appropriate bits in the control registers

load_page_directory: 
    pushl %ebp 
    movl %esp, %ebp 

    # callee save the callee save regs
    pushl %esi 
    pushl %edi 
    pushl %edx 

    # loads in the p_dir parameter into EAX to be loaded into the cr3 register
    movl 8(%ebp), %eax 
    movl %eax, %cr3 

    # sets the protected-mode virtual interrupts bit in cr4 to 1 
    movl %cr4, %eax 
    orl $0x00000010, %eax 
    movl %eax, %cr4

    # sets the Protected mode enable bit and the paging bit to 1 enabling paginng and the protection mode
    movl %cr0, %eax  
    orl $0x80000001, %eax 
    movl %eax, %cr0 

    # remove callee save regs
    popl %edx
    popl %edi 
    popl %esi 

    leave
    ret 

flush_TLB:
    # loads in the p_dir parameter into EAX to be loaded into the cr3 register
    movl %cr3, %eax 
    movl %eax, %cr3 
    ret






