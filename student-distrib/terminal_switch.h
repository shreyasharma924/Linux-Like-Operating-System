#ifndef _TREMINAL_SWITCH_H
#define _TERMINAL_SWITCH_H

#include "scheduling.h"
#include "sys_calls_diff.h"
#include "paging_helpers.h"
#include "lib.h"
#include "keyboard.h"

void init_switching_terminals(); 
uint32_t switching_terminals(uint32_t term_to_switch_to);

#endif

