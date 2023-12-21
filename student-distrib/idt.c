#include "idt.h"
#include "lib.h"

int i; 
// The below 20 functions are the exception handlers

/*

Description: Exception handler for dividing by zero 
Input: None
Output: None
Retrun Value: None
Side Effect: Prints to the screen that a division by zero exception occured and the program continous to spin to prevent any further
progress in the program.

*/
void zero_div_func(){ 
    puts("Division by zero exception");
    halt_blossom_handler((uint8_t) 256); 
}

/*

Description: Exception handler for debug exception
Input: None
Output: None
Retrun Value: None
Side Effect: Prints to the screen that a debug exception occured and the program continous to spin to prevent any further
progress in the program.

*/
void debug_exception_func(){

    puts("Debug exception");
    halt_blossom_handler((uint8_t) 256); 
}

/*

Description: Exception handler for nmi exception
Input: None
Output: None
Retrun Value: None
Side Effect: Prints to the screen that a nmi exception occured and the program continous to spin to prevent any further
progress in the program.

*/
void nmi_func(){

    puts("NMI exception");
    halt_blossom_handler((uint8_t) 256); 
       
}

/*

Description: Exception handler for breakpoint exception
Input: None
Output: None
Retrun Value: None
Side Effect: Prints to the screen that a breakpoint exception occured and the program continous to spin to prevent any further
progress in the program.

*/
void breakpoint_func(){

    puts("Breakpoint exception");
    halt_blossom_handler((uint8_t) 256); 
       
}

/*

Description: Exception handler for bound range exception
Input: None
Output: None
Retrun Value: None
Side Effect:  Prints to the screen that a bound range exception occured and the program continous to spin to prevent any further
progress in the program.

*/
void bound_range_func(){

    puts("Bound Range exception");
    halt_blossom_handler((uint8_t) 256); 
       
}

/*

Description: Exception handler for invalid opcode exception
Input: None
Output: None
Retrun Value: None
Side Effect:  Prints to the screen that an invalid opcode exception occured and the program continous to spin to prevent any further
progress in the program.

*/
void inv_opcode_func(){

    puts("Invalid Opcode exception");
    halt_blossom_handler((uint8_t) 256); 
       
}

/*

Description: Exception handler for coprocessor exception
Input: None
Output: None
Retrun Value: None
Side Effect:  Prints to the screen that a coprocessor exception occured and the program continous to spin to prevent any further
progress in the program.

*/
void coprocessor_func(){

    puts("Coprocessor exception");
    halt_blossom_handler((uint8_t) 256); 
       
}

/*

Description: Exception handler for double fault exception 
Input: None
Output: None
Retrun Value: None
Side Effect:  Prints to the screen that a double fault exception occured and the program continous to spin to prevent any further
progress in the program.

*/
void double_fault_func(){

    puts("Double Fault exception");
    halt_blossom_handler((uint8_t) 256); 
       
}

/*

Description: Exception handler for coprocessor segment fault exception
Input: None
Output: None
Retrun Value: None
Side Effect:  Prints to the screen that a coprocessor segment exception occured and the program continous to spin to prevent any further
progress in the program.

*/
void coprocessor_segment_func(){

    puts("Coprocessor Segment exception");
    halt_blossom_handler((uint8_t) 256); 
       
}

/*

Description: Exception handler for invalid task function
Input: None
Output: None
Retrun Value: None
Side Effect: Prints to the screen that an invalid task exception occured and the program continous to spin to prevent any further
progress in the program.

*/
void inv_task_func(){ 

    puts("Invalid Task exception");
    halt_blossom_handler((uint8_t) 256); 
       
}

/*

Description: Exception handler for segment not present exception
Input: None
Output: None
Retrun Value: None
Side Effect:  Prints to the screen that a segment not present exception occured and the program continous to spin to prevent any further
progress in the program.

*/
void segment_not_present_func(){

    puts("Segment Not Present exception");
    halt_blossom_handler((uint8_t) 256); 
       
}

/*

Description: Exception handler for stack segment fault exception
Input: None
Output: None
Retrun Value: None
Side Effect:  Prints to the screen that a stack seg fault exception occured and the program continous to spin to prevent any further
progress in the program.

*/
void stack_seg_fault_func(){

    puts("Stack Segment Fault exception");
    halt_blossom_handler((uint8_t) 256); 
       
}

/*

Description: Exception handler for general protection exception
Input: None
Output: None
Retrun Value: None
Side Effect:  Prints to the screen that a general protection exception occured and the program continous to spin to prevent any further
progress in the program. 

*/
void general_protection_func(){

    puts("General Protection exception");
    halt_blossom_handler((uint8_t) 256); 
       
}

/*

Description: Exception handler for page fault exception
Input: None
Output: None
Retrun Value: None
Side Effect:  Prints to the screen that a page fault exception occured and the program continous to spin to prevent any further
progress in the program.

*/
void page_fault_func(){

    puts("Page Fault exception");
    halt_blossom_handler((uint8_t) 256); 
       
}

/*

Description: Exception handler for floating point exception
Input: None
Output: None
Retrun Value: None
Side Effect:  Prints to the screen that a floating point exception occured and the program continous to spin to prevent any further
progress in the program.

*/
void floating_point_func(){

    puts("Floating Point exception");
    halt_blossom_handler((uint8_t) 256); 
       
}

/*

Description: Exception handler for alignment check exception
Input: None
Output: None
Retrun Value: None
Side Effect:  Prints to the screen that an alignment check exception occured and the program continous to spin to prevent any further
progress in the program.

*/
void alignment_check_func(){

    puts("Alignment Check exception");
    halt_blossom_handler((uint8_t) 256); 
       
}

/*

Description: Exception handler for machine check exception
Input: None
Output: None
Retrun Value: None
Side Effect:  Prints to the screen that a machine check exception occured and the program continous to spin to prevent any further
progress in the program.

*/
void machine_check_func(){

    puts("Machine Check exception");
    halt_blossom_handler((uint8_t) 256); 
       
}

/*

Description: Exception for simd floating point exception
Input: None
Output: None
Retrun Value: None
Side Effect:  Prints to the screen that a simd floating point exception occured and the program continous to spin to prevent any further
progress in the program.

*/
void simd_floating_point_func(){

    puts("SIMD Floating Point exception");
    halt_blossom_handler((uint8_t) 256); 
       
}

/*

Description: Exception handler for overflow exception
Input: None
Output: None
Retrun Value: None
Side Effect:  Prints to the screen that an overflow exception occured and the program continous to spin to prevent any further
progress in the program.

*/
void overflow_func(){

    puts("overflow exceptions");
    halt_blossom_handler((uint8_t) 256); 
       
}

/*

Description: Exception handler for device not available exception
Input: None
Output: None
Retrun Value: None 
Side Effect:  Prints to the screen that a device is not available exception occured and the program continous to spin to prevent any further
progress in the program.

*/
void device_na_func(){

    puts("device not available exception");
    halt_blossom_handler((uint8_t) 256); 
       
}

/*

Description: Exception handler for invalid task state exception
Input: None
Output: None
Retrun Value: None
Side Effect:  Prints to the screen that an invalid task state exception occured and the program continous to spin to prevent any further
progress in the program.

*/
void inv_task_state_func(){

    puts("invalid task state segment exception");
    halt_blossom_handler((uint8_t) 256); 
       
}

// void sys_call(){

//     puts(" System Call Executing ");
//     while(1){ };
// }

/*

Description: Loads the idt table with the exceptions and interrupts
Input: None
Output: None
Retrun Value: none
Side Effect: loads the idt using each exception and interrupt function

*/
 /* Construct an IDT entry in the GDT */
 void idt_init(){
    // the below array holds all the asm labels that need to be loaded into the idt so that the processor can access them.
    // 21: there are 20 total functions and 21 due to zero offset index
    long asm_func[23] = {(long)zero_div, (long)debug_exception,(long) nmi, (long)breakpoint,(long) overflow, (long)bound_range, (long)inv_opcode, (long)device_na, 
            (long) double_fault, (long)coprocessor_segment, (long)inv_task_state,(long) segment_not_present,(long) stack_seg_fault, 
            (long)general_protection, (long)page_fault, (long)floating_point,(long) alignment_check,(long) machine_check,(long) simd_floating_point, (long) keyboard, (long)rtc,(long)jump_on_it, (long)PIT};
    //  The below for loop is used to load the idt with each function at the correct index.
    // 41: The last entered value for the interrupt is the RTC, so the max of the for loop is 41. This is x29 in hex and x28 
    //     is the index for the RTC in the interrupt handler.
    for(i =0; i <= 128; i++){  // 128 -> to include system calls
        if(i < 19){ // 19: there are 19 exception handler functions
            idt[i].seg_selector = (uint16_t) KERNEL_CS;
            idt[i].reserved4 = 0;
            idt[i].reserved3 = 1;
            idt[i].reserved2 = 1;
            idt[i].reserved1 = 1;
            idt[i].size = 1;
            idt[i].reserved0= 0;
            idt[i].dpl = 0;
            idt[i].present = 1; 
            SET_IDT_ENTRY(idt[i],asm_func[i]); // using idt table to trigger interrupt, connect the index to adm function
        }
        else if(i >= 20 && i <= 31){ // skip these exception handlers (skipping 14(20) to 1F(31))
            idt[i].seg_selector = (uint16_t) KERNEL_CS;
            idt[i].reserved4 = 0;
            idt[i].reserved3 = 1;
            idt[i].reserved2 = 1;
            idt[i].reserved1 = 1;
            idt[i].size = 1;
            idt[i].reserved0= 0;
            idt[i].dpl = 0;
            idt[i].present = 1; 
            idt[i].offset_31_16 = 0;
            idt[i].offset_15_00 = 0;
            //SET_IDT_ENTRY(idt[i],);
        }
        //   else if(i == 32){ // skipping this interrupt - only care about keyboard and rtc interrupt - Interrupt index 0x20
        //     idt[i].seg_selector = (uint16_t) KERNEL_CS;
        //     idt[i].reserved4 = 0;
        //     idt[i].reserved3 = 0;
        //     idt[i].reserved2 = 1;
        //     idt[i].reserved1 = 1;
        //     idt[i].size = 1;
        //     idt[i].reserved0= 0;
        //     idt[i].dpl = 0;
        //     idt[i].present = 1; 
        //     idt[i].offset_31_16 = 0;
        //     idt[i].offset_15_00 = 0;
        // }

        else if(i == 0x20){ // 0x21 - PIT IDT entry  
            idt[i].seg_selector = (uint16_t) KERNEL_CS;
            idt[i].reserved4 = 0;
            idt[i].reserved3 = 0;
            idt[i].reserved2 = 1;
            idt[i].reserved1 = 1;
            idt[i].size = 1;
            idt[i].reserved0 = 0;
            idt[i].dpl = 0;
            idt[i].present = 1; 
            SET_IDT_ENTRY(idt[i],PIT);
        } 
        else if(i == 0x21){ // 0x21 - Keyboard IDT entry  
            idt[i].seg_selector = (uint16_t) KERNEL_CS;
            idt[i].reserved4 = 0;
            idt[i].reserved3 = 0;
            idt[i].reserved2 = 1;
            idt[i].reserved1 = 1;
            idt[i].size = 1;
            idt[i].reserved0 = 0;
            idt[i].dpl = 0;
            idt[i].present = 1; 
            SET_IDT_ENTRY(idt[i],asm_func[19]);
        } 
        else if(i >= 0x22 && i <= 0x27){ // skipping these interrupt handlers: - only care about keyboard and rtc interrupt
            idt[i].seg_selector = (uint16_t) KERNEL_CS;
            idt[i].reserved4 = 0;
            idt[i].reserved3 = 0;
            idt[i].reserved2 = 1;
            idt[i].reserved1 = 1;
            idt[i].size = 1;
            idt[i].reserved0= 0;
            idt[i].dpl = 0;
            idt[i].present = 1; 
            idt[i].offset_31_16 = 0;
            idt[i].offset_15_00 = 0;
        }
        else if(i == 0x28){ // 0x28 - rtc handler into idt
            idt[i].seg_selector = (uint16_t) KERNEL_CS;
            idt[i].reserved4 = 0;
            idt[i].reserved3 = 0;
            idt[i].reserved2 = 1;
            idt[i].reserved1 = 1;
            idt[i].size = 1;
            idt[i].reserved0 = 0;
            idt[i].dpl = 0;
            idt[i].present = 1; 
            SET_IDT_ENTRY(idt[i],asm_func[20]);
        } 
        else if(i == 0x80){ // 0x80: system call in idt table
            idt[i].seg_selector = (uint16_t) KERNEL_CS;
            idt[i].reserved4 = 0;
            idt[i].reserved3 = 1;
            idt[i].reserved2 = 1;
            idt[i].reserved1 = 1;
            idt[i].size = 1;
            idt[i].reserved0 = 0;
            idt[i].dpl = 3;
            idt[i].present = 1; 
            SET_IDT_ENTRY(idt[i],asm_func[21]);
        }
        else{ // skip any other values
            
            idt[i].seg_selector = (uint16_t) KERNEL_CS;
            idt[i].reserved4 = 0;
            idt[i].reserved3 = 0;
            idt[i].reserved2 = 1;
            idt[i].reserved1 = 1;
            idt[i].size = 1;
            idt[i].reserved0= 0;
            idt[i].dpl = 0;
            idt[i].present = 0; 
            idt[i].offset_31_16 = 0;
            idt[i].offset_15_00 = 0;
        }
    
    }
    lidt(idt_desc_ptr);
 }










     
