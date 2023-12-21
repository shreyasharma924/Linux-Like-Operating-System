#ifndef _SCHEDULING_H
#define _SCHEDULING_H

#define VIDEO_M       0xB8000

#include "paging_helpers.h"
#include "lib.h"
// #include "sys_calls_diff.h" 
#include "idt.h"
// #include "i8259.h"
// #include "keyboard.h"
#include "scheduler.h"
// #include "schannel.h" //SHREYU WANTED THIS (BOTH OF US)

uint32_t temp_ebp1 ; // stores current ebp
uint32_t temp_esp1 ; // stores current esp
int32_t base_shells_init_flag; 


uint32_t pcb_index; 
void PIT_init();
void PIT_handler();
void scheduler_set();

#endif 


