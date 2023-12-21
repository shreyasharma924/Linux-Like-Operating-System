#ifndef TESTS_H
#define TESTS_H


#include "x86_desc.h"
#include "terminal.h"
#include "rtc.h"
#include "paging_helpers.h"
#include "file_sys.h"
#include "sys_call.h"
#include "scheduling.h"


// test launcher
void launch_tests();
int divbyzero(); // tests divide by zero exception
int page_fault_exception();// tests page fault exception
int rtc_test_cp2(); // tests out rtc for checkpoint 2
int test_terminal();
//int deref(int *a); 
#endif /* TESTS_H */
