#ifndef _RTC_H
#define _RTC_H

#include "lib.h"
#include "types.h"
#include "i8259.h"
//#include "lib.c"

#define RTC_PORT  0x70 // defining out RTC port 




void rtc_init(); // intitalizes rtc
int rtc_handler(); // handles the rtc
extern void test_interrupts(); // tests for any interrupt
int32_t open_rtc ( int8_t* filename); // declaration of the rtc open function 
int32_t close_rtc (int32_t fd); // declaration of the rtc close function 
int32_t write_rtc (int32_t fd,  void* buf, int32_t nbytes); // declaration of the rtc write function 
int32_t read_rtc (int32_t fd, void* buf, int32_t nbytes); // declaration of the rtc read function 

#endif

