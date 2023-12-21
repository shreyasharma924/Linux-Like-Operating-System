#include "rtc.h"
//#include "exception.h"
//#include "interrutps.h"
#include "lib.h"
#define REAL_FREAK 1024/2

//#include <stdio.h>
//#include "i8259.c"

int32_t freak = 2;

//int32_t rate;


/*
Description: Initializing RTC and enabling it on the IRQ Line 8 
Input: None
Output: None
Side Effects: RTC is enabled on line 8 
*/
void rtc_init(){
    //2: the pin that secondary pic is connected to on the primary pic
    outb(0x8B, 0x70);		// disabling NMI and slecting reg B
    char prev = inb(0x71);	// prev stores current value in reg B
    outb(0x8B, 0x70);		// set the index again - read will reset the index to register D 
    outb(prev | 0x40 , 0x71);	//ORs the pervious value with 0X40 
    outb(0x0C, 0x70);       // selecting reg C
    inb(0x71);              //read from reg C
   // enable_irq(1); // enables the keyboard in IRQ 1 
    enable_irq(8); // enables the rtc interrupt

}

/*
Description: RTC handler 
Input: None
Output: None
Side Effects: Acknowleges RTC interrupt, checks if interrupt is working and send the end of interrupt signal, also now implements virtualization of the frequency 
*/
int rtc_handler(){
    // outb(0x0C, 0x70);       // selecting reg C
    inb(0x71);              //read from reg C
    send_eoi(8);            // end of interrupt on IRQ line 8 
    // test_interrupts();      //calling to check if interrupts are functioning
    multi_term[next_index].inter_counter++; // incrementing out interrupt counter 
    if(multi_term[next_index].inter_counter % (REAL_FREAK/freak) == 0){ // checks if the value of the interrupt counter is equal to the REAL_FREAK/freak 
        multi_term[next_index].interrupt_flag = 1; // setting out interrupt counter to 1 
        multi_term[next_index].inter_counter = 0; //resetting out interrupt counter 
       // putc("1");
    }    
    //interrupt_flag = 1;
    return 0;
} 


/*
Description: setting our initial frequency value 
Input: filename
Output: returns 0 
Side Effects: initalizes our first frequncy 
*/
int32_t open_rtc (int8_t* filename){
    freak = 2; // initializing our frequency to 2 
    return 0;

}

/*
Description: reading RTC 
Input: file directory, buffer, bytes to read 
Output: returns 0 
Side Effects: waits for an interrupt to occur to tick off rtc 
*/
int32_t read_rtc (int32_t fd, void* buf, int32_t nbytes){
    multi_term[next_index].interrupt_flag = 0; // setting interrupt flag to 0
    while(multi_term[next_index].interrupt_flag == 0); // keeps lopping unitl interrupt occurs 

    return 0; 
}


/*
Description: writing RTC 
Input: file directory, buffer, bytes to read 
Output: returns 0 
Side Effects: waits for an interrupt to occur to tick off rtc 
*/
int32_t write_rtc (int32_t fd,  void* buf, int32_t nbytes){
    freak = *((int32_t*) buf); // initilaizing freak to the buffer passed 
    return nbytes;

}


/*
Description: close RTC 
Input: file directory
Output: returns 0 
Side Effects: none
*/
int32_t close_rtc (int32_t fd){
    return 0; 
}


