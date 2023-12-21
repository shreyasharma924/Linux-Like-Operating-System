#ifndef _SCHEDULER_H
#define _SCHEDULER_H

#include "types.h"

extern void scheduler_context_switch(uint32_t temp_ebp1,uint32_t temp_esp1); //context switch for scheduling

#endif

