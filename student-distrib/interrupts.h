#ifndef _INTERRUPTS_H
#define _INTERRUPTS_H


extern void keyboard(); // asm label for linking the keyboard interrupt with the idt 
extern void rtc(); // asm label for linking the rtc interrupt with the idt 
extern void PIT();

#endif
