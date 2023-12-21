#ifndef _KEYBOARD_H
#define _KEYBOARD_H
#include "lib.h"

#include "types.h"
#include "i8259.h"
#include "terminal_switch.h"

// MAGIC NUMBERS LABELS
#define KEYBOARD_PORT  0x60
#define CAPS_PRESSED  0x3A;
#define CAPS_RELEASED  0xBA;
#define SHIFT_PRESS_RIGHT  0x36;
#define SHIFT_PRESS_LEFT  0x2A;
#define SHIFT_RELEASE_RIGHT  0xB6;
#define SHIFT_RELEASE_LEFT  0xAA;
#define ENTER_PRESSED  0x1D;
#define ENTER_RELEASED  0x9D;
#define ALT_PRESSED_LEFT  0x38;
#define ALT_RELEASED_LEFT  0xB8;
#define CURSOR_UP_PRESSED  0x48;
#define CURSOR_UP_RELEASED  0xC8;
#define CURSOR_DOWN_PRESSED  0x50;
#define CURSOR_DOWN_RELEASED  0xD0;
#define CURSOR_RIGHT_PRESSED  0x4D;
#define CURSOR_RIGHT_RELEASED  0xCD;
#define CURSOR_LEFT_PRESSED  0x4B;
#define CURSOR_LEFT_RELEASED  0xCB;
#define NUM_CHARACTERS  256;
#define MAX_CHAR_TO_SCREEN  128;




//keyboard functions declared 
void keyboard_init();   // initializes the keyboaord
void keyboard_handler();  // handles the keyboard interrupts
void cleanse(); 
//char val_to_display(char scancode);
int enter_flag;  //  a global variable as the enter flag
char terminal_buffet[128]; // a global array that holds all of the characters to be written
char keyboard_buffet[128];
int terminal_cur_reading_flag;
int switch_to_term; 
int buffer_filled_flag;
#endif
