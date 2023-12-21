#ifndef _EXCEPTION_H
#define _EXCEPTION_H





extern void zero_div (); // Calls for the divide by zero exception handler
extern void nmi (); // Calls for the nmi handler
extern void debug_exception (); // Calls for the debug exception handler
extern void breakpoint (); // Calls for the breakpoint exception handler
extern void overflow (); // Calls for the overflow exception handler
extern void bound_range (); // Calls for the bound range exception handler
extern void inv_opcode (); // Calls for the invalid opcode exception handler
extern void device_na (); // Calls for the device not available handler
extern void double_fault (); // Calls for the double fault exception handler
extern void coprocessor_segment (); // Calls for the coprocessor segment exception handler
extern void inv_task_state (); // Calls for the invalid task exception handler
extern void segment_not_present (); // Calls for the seggment not present exception handler
extern void stack_seg_fault (); // Calls for the stack segment fault exception handler
extern void general_protection ();  // Calls for the general protection exception handler
extern void page_fault (); // Calls for the page fault exception handler
extern void floating_point (); // Calls for the floating point exception handler
extern void alignment_check (); // Calls for the alignment check exception handler
extern void machine_check (); // Calls for the machine check exception handler
extern void simd_floating_point (); // Calls for the SIMD floating point exception handler

#endif 


