#ifndef _TREMINAL_H
#define _TERMINAL_H

#include "keyboard.h"

int open(); // not implemented yet
int close (); // not implemented yet
int terminal_read(int fd,  void* buf, int nbytes); // declaration of the terminal read function 
int terminal_write(int fd,  void* buf, int nbytes); // declaration of the terminal write function 

int sys_flag; // initializing file flag 
// int terminal_cur_reading_flag;


#endif
