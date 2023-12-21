#ifndef _IDT_H
#define _IDT_H
#include "x86_desc.h"
#include "exception.h"
#include "keyboard.h"
#include "interrupts.h"
#include "rtc.h"
#include "sys_call.h"


void idt_init(); // initializes and loads the exceptions and interrupts into the idt
void zero_div_func(); // handles the divide by zero exception
void nmi_func(); // handles the nmi exception
void debug_exception_func(); // handles the debug exception
void breakpoint_func(); // handles the breakpoint exception
void overflow_func(); // handles the overflow exception
void bound_range_func(); // handles the debug exception
void inv_opcode_func(); // handles the opcode exception
void device_na_func(); // handles the device not available exception
void double_fault_func();  // handles the double fault exception
void coprocessor_segment_func(); // handles the coprocessor segment exception
void inv_task_state_func(); // handles the invalid task state exception
void segment_not_present_func(); // handles the segment not present exception
void stack_seg_fault_func(); // handles the stack segment fault exception
void general_protection_func(); // handles the general protection exception
void page_fault_func(); // handles the page fault exception
void floating_point_func(); // handles the floating point exception
void alignment_check_func(); // handles the alignment check exception
void machine_check_func(); // handles the machine check exception
void simd_floating_point_func(); // handles the simd floating point exception
// void sys_call();
#endif
