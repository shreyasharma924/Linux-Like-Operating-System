#include "keyboard.h"
#include "lib.h"



// initialize global variables

int cap_flag = 0; 
int shift_flag = 0;
int ctrl_flag = 0; 
int alty = 0; 
int F_1 = 0; 
int F_2 = 0; 
int F_3 = 0; 
int backspace_flag ;
int c= 0; 
//int idx; 



/*
Description: stores the array of characters that include lowercase letters, numbers and special characters that don't need shift
Input: keyboard scan codes
Output: the coresponding character or -1 if invalid
Side Effects: converts scancodes to characters to be outputted to the screen
*/
char lower_chars(int scancode) {
    // we only want to consider scancodes from 0 to 256
    char mapping_lower[512] = {'\0','\0','1', '2', '3', '4', '5', '6', '7', '8',
                                '9','0','-', '=','\0', '\0','q', 'w', 'e', 'r', 
                                't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\0','\0',
                                'a', 's' ,'d', 'f', 'g', 'h', 'j', 'k', 'l', ';', 
                                '\'', '`', '\0', '\\','z','x','c','v','b','n',
                                'm', ',', '.', '/', '\0', '*','\0',' '};
                                
                  if(scancode >= 0 && scancode < 128) {
                    return mapping_lower[scancode];
                  }
                  // if the scancode is out of these bounds
                  return -1;        
}

/*
Description: stores the array of characters that include uppercase letters, numbers and special characters that don't need shift
Input: keyboard scan codes
Output: the coresponding character or -1 if invalid
Side Effects: converts scancodes to characters to be outputted to the screen
*/
char upper_chars(int scancode) {
    // we only want to consider scancodes from 0 to 256
    char mapping_upper[512]= {'\0','\0','1', '2', '3', '4', '5', '6', '7', '8',
                              '9','0','-', '=','\0', '\0','Q', 'W', 'E', 'R', 
                              'T', 'Y', 'U', 'I', 'O', 'P', '[', ']', '\0','\0',
                              'A', 'S' ,'D', 'F', 'G', 'H', 'J', 'K', 'L', ';', 
                              '\'', '`', '\0', '\\','Z','X','C','V','B','N',
                              'M', ',', '.', '/', '\0', '*','\0',' '};
                            
                          
                  if(scancode >= 0 && scancode < 128) {
                    return mapping_upper[scancode];
                  }
                  // if the scancode is out of these bounds
                  return -1;       
}

/*
Description: stores the array of characters that include lowercase letters and special characters that need shift
Input: keyboard scan codes
Output: the coresponding character or -1 if invalid
Side Effects: converts scancodes to characters to be outputted to the screen
*/
char caps_shift(int scancode) {
    // we only want to consider scancodes from 0 to 256
    char mapping_lower[512] = {'\0','\0','!', '@', '#', '$', '%', '^', '&', '*',
                                '(',')','_', '+','\0', '\0','q', 'w', 'e', 'r', 
                                't', 'y', 'u', 'i', 'o', 'p', '{', '}', '\0','\0',
                                'a', 's' ,'d', 'f', 'g', 'h', 'j', 'k', 'l', ':', 
                                '"', '~', '\0', '|','z','x','c','v','b','n',
                                'm', '<', '>', '?', '\0', '*','\0',' '};
                          
                  if(scancode >= 0 && scancode < 128) {
                    return mapping_lower[scancode];
                  }
                  // if the scancode is out of these bounds
                  return -1;       
}


/*
Description: stores the array of characters that include uppercase letters and special characters that need shift
Input: keyboard scan codes
Output: the coresponding character or -1 if invalid
Side Effects: converts scancodes to characters to be outputted to the screen
*/
char nocaps_shift(int scancode) {
    // we only want to consider scancodes from 0 to 256
    char mapping_upper[512]= {'\0','\0','!', '@', '#', '$', '%', '^', '&', '*',
                              '(',')','_', '+','\0', '\0','Q', 'W', 'E', 'R', 
                              'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\0','\0',
                              'A', 'S' ,'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', 
                              '"', '~', '\0', '|','Z','X','C','V','B','N',
                              'M', '<', '>', '?', '\0', '*','\0',' '};
                            
                          
                  if(scancode >= 0 && scancode < 128) {
                    return mapping_upper[scancode];
                  }
                  // if the scancode is out of these bounds
                  return -1;      
}

/*
Description: enabling the keyboard in the correct IRQ line in the PIC
Input: None
Output: None
Side Effects: It enables the keyboard on the the 1st IRQ line in the PIC
*/
void keyboard_init(){
    enable_irq(1); // enables the keyboard in IRQ 1 
}

/*
Description: fills the terminal buffer and clears the keyboard buffer
Input: None
Output: None
Side Effects: sets the enter flag to 1 bc it has been pressed
              fills the terminal buffer
              clears the keyboard buffer and sets its index back to 0
*/
void cleanse(){
    int i; 
    for (i=0; i<128 ; i++){
        terminal_buffet[i] = keyboard_buffet[i];                        // transfer keybord buffer to terminal buffer
        keyboard_buffet[i] = '\0';                                      // unused spaces hold the null \0 value
        multi_term[active_terminal_id].enter_flag = 1;
    }
    multi_term[active_terminal_id].buff_idx = 0;
    

    if(buffer_filled_flag ==1){
        multi_term[active_terminal_id].enter_flag = 1;
        buffer_filled_flag =0;
    }
    else{
        multi_term[active_terminal_id].enter_flag = 0;
    }
}

/*F
Description: clears the keyboard buffer
Input: None
Output: None
Side Effects: clears the keyboard buffer and sets its index back to 0
*/
void reset(){
    int i; 
    for (i=0; i<128; i++){
        keyboard_buffet[i] = '\0';   
    }
    multi_term[active_terminal_id].buff_idx = 0;
}

/*
Description: handles all the keyboard scancodes that are sent through the keyboard interrupt
Input: None
Output: characters to the screen
Side Effects: maps all the incoming scancodes to their coresponding characters and functionality
*/
void keyboard_handler(){
    
    //cli();  
     int scanCode = inb(KEYBOARD_PORT);  
                                                                            // start of critical section
    //int scanCode = inb(KEYBOARD_PORT);                                                  // save that scancode for the keyboard
    char output = -1;   
    send_eoi(1);       
                                                                // used to save the output of our scancode arrays
    
    switch(scanCode){                                                                   // switch case to set all our necessary flags
        case 0x3A:                                      // caps lock key pressed (0x3A)
            if (cap_flag == 0){
                cap_flag = 1;
            }
            else if (cap_flag == 1){
                cap_flag = 0; 
            }
            break;

        case 0x36:                                      // right shift key pressed (0x36)
        case 0x2A:                                      // left shift key pressed (0x2A)
            if(shift_flag == 0){
                shift_flag  = 1; 
            }
            break;
        
        case 0xB6:                                      // right shift key released (0xB6)
        case 0xAA:                                      // left shift key pressed (0xAA)
            if(shift_flag == 1){
                shift_flag  = 0; 
            }
            break;

        case 0x1D:                                      // ctrl key pressed (0x1D)
            if(ctrl_flag == 0){
                ctrl_flag = 1;
            }
            break;
        
        case 0x9D:                                       // ctrl key released (0x9D)
             if(ctrl_flag == 1){
                ctrl_flag = 0;
            }
            break; 
        
        case 0x0E:                                      // backspace key pressed (0x0E)
                backspace_flag = 1;

            break; 
        
        case 0x1C:                                      // enter key pressed (0x1C)
                multi_term[active_terminal_id].enter_flag = 1;

            break;

        case 0x38:                                      // alt key pressed (0x38)
        case 0xE0:                                      // alt key pressed (0xE0)
            if(alty == 0){
                alty  = 1; 
            }
            break;
        
        case 0xB8:                                      // alt released 
        //case 0xE0:                                     
            if(alty == 1){
                alty  = 0; 
            }
            break;

        case 0x3B:                                                                           // f1 key pressed
            if(F_1 == 0){
                F_1  = 1; 
            }
            break;
        case 0xBB:                                      // f1 key released
            if(F_1 == 1){
                F_1  = 0; 
            }
            break;
        
        case 0x3C:                                      // f2 key pressed
            if(F_2 == 0){
                F_2  = 1; 
            }
            break;
        case 0xBC:                                      // f2 key released
            if(F_2 == 1){
                F_2  = 0; 
            }
            break;
        case 0x3D:                                     // f3 key pressed
            if(F_3 == 0){
                F_3  = 1; 
            }
            break;
        case 0xBD:                                      // f3 key released
            if(F_3 == 1){
                F_3  = 0; 
            }
            break;

    }

    // this while loop prevents the listed keys from printing a space on the screen when pressed:
        // shift, enter, alt, up cursor, down cursor, right cursor, left cursor, and caps lock
    // this is done by setting the scancode to their release scancode as soon as the flags have been processed
    while(scanCode == 0x3A || scanCode == 0x36 || scanCode == 0x2A || scanCode == 0x1D || scanCode == 0x38 || scanCode == 0x48 || scanCode == 0x50 || scanCode == 0x4D || scanCode == 0x4B){
        if(scanCode == 0x36){                       // right shift pressed (0x36)
            scanCode = SHIFT_RELEASE_RIGHT;
        }
        else if(scanCode == 0x2A){                  // left shift pressed (0x2A)
            scanCode = SHIFT_RELEASE_LEFT;
        }
        else if(scanCode == 0x1D){                  // enter pressed (0x1D)
            scanCode = ENTER_RELEASED;
        }
        else if(scanCode == 0x38){                  // alt  pressed (0x38)
            scanCode = ALT_RELEASED_LEFT;
        }
        else if(scanCode == 0x48){                  // cursor up pressed (0x48)
            scanCode = CURSOR_UP_RELEASED;
        }
        else if(scanCode == 0x50){                  // cursor down pressed (0x50)
            scanCode = CURSOR_DOWN_RELEASED;
        }
        else if(scanCode == 0x4D){                  // cursor right pressed (0x4D)
            scanCode = CURSOR_RIGHT_RELEASED;
        }
        else if(scanCode == 0x4B){                  // cursor left pressed (0x4B)
            scanCode = CURSOR_LEFT_RELEASED;
        }
        else{
            // set to caps_released scancode
            scanCode = CAPS_RELEASED;
        }

    }
    


    // only want to print a max of 127 characters to the screen since the last space on the buffer is reserved for newline character
    if (multi_term[active_terminal_id].buff_idx < 127){
        buffer_filled_flag = 0;
        
        if(scanCode == 0x0F){                                                 // tab pressed (0x0F)
            if(multi_term[active_terminal_id].buff_idx < 123){                                                    // if the buffer has space for 4 or more characters
            putc_pt2('\t');                                                       // then tab is allowed and can be added to the buffer
            keyboard_buffet[multi_term[active_terminal_id].buff_idx] = ' ';
            keyboard_buffet[multi_term[active_terminal_id].buff_idx+1] = ' ';
            keyboard_buffet[multi_term[active_terminal_id].buff_idx+2] = ' ';
            keyboard_buffet[multi_term[active_terminal_id].buff_idx+3] = ' ';
            multi_term[active_terminal_id].buff_idx = multi_term[active_terminal_id].buff_idx + 4; 
            }
            send_eoi(1);                                        // calls for the end of the intterupt signal in IRQ 1
        }

        else if (cap_flag == 1 && shift_flag == 1 ){            // both caps and shift are active 
            output = caps_shift(scanCode);                      // allow for lowercase letters and special characters that need shift
            if (output != -1){                                  // check if scancode is within bounds
                if(output != '\b'){
                    keyboard_buffet[multi_term[active_terminal_id].buff_idx] = output;              // add to buffer
                    multi_term[active_terminal_id].buff_idx++;  
                }
                putc_pt2(output);                                   // print to screen
            
            }
            send_eoi(1);                                        // calls for the end of the intterupt signal in IRQ 1 
        
        }

        else if (backspace_flag == 1){                          // backspace pressed
            if(multi_term[active_terminal_id].buff_idx != 0){                                       // if screen_x is not 0
                backspace_flag = 0;
                keyboard_buffet[multi_term[active_terminal_id].buff_idx] = '\0';                    // first put a null character in that current index then decrement the index of the buffer and
                multi_term[active_terminal_id].buff_idx-=1;                                         
                putc_pt2('\b');                                     // show the backspace on the screen
                send_eoi(1);                                    // calls for the end of the intterupt signal in IRQ 1
            }
            else{
                backspace_flag = 0;                              // if screen_x is 0, do nothing
                send_eoi(1);                                     // calls for the end of the intterupt signal in IRQ 1
            }
        
        }

        else if (cap_flag == 0 && shift_flag == 1 && ctrl_flag==0 && multi_term[active_terminal_id].enter_flag == 0 ){         // if shift is the being held down
            output = nocaps_shift(scanCode);                                                    // get character matching the scancode
            if (output != -1){                                                                  // check if valid scancode

                if(output != '\b'){                             
                keyboard_buffet[multi_term[active_terminal_id].buff_idx] = output;                                                  // add to buffer
                    multi_term[active_terminal_id].buff_idx++;  
                }
                putc_pt2(output);                                                                   // print character to screen
                
            }
            send_eoi(1);                                                                        // calls for the end of the intterupt signal in IRQ 1 
            
        }

        else if (cap_flag == 1 && shift_flag == 0 && ctrl_flag==0 && multi_term[active_terminal_id].enter_flag == 0){          // if capslock was pressed
            output = upper_chars(scanCode);                                                     // get character matching the scancode
            if (output != -1){                                                                  // check if valid scancode
                
                if(output != '\b'){
                    keyboard_buffet[multi_term[active_terminal_id].buff_idx] = output;                                              // add to buffer
                    multi_term[active_terminal_id].buff_idx++; 
                }
                putc_pt2(output);                                                                   // print character to screen
            }
            send_eoi(1);                                                                        // calls for the end of the intterupt signal in IRQ 1 
            
        }

        else if((ctrl_flag == 1 && scanCode == 0x26 && shift_flag == 0) || (ctrl_flag == 1 && scanCode == 0x26 && shift_flag == 1)){    // if ctrl and l or L are being pressed
            clear_pt2();                                                                    // clear the screen
            set_cursor(0,0);                                                            // set the cursor to the top left corner
            update_cursor();                                                            // set this change in memory
            reset();                                                                    // call reset to clear our keyboard buffer
            keyboard_buffet[multi_term[active_terminal_id].buff_idx] = '\0';                                                // since L or l is being pressd, it will be added to the buffer, so we need to clear the first position in the buffer
            backspace_flag = 0;                                                         // if screen_x is 0, do nothing

            send_eoi(1);                                                                // calls for the end of the intterupt signal in IRQ 1 
            
        }

        else if(scanCode == 0x1C){                                                      // if enter has been pressed
            putc_pt2('\r');                                                                 // add this symbol to the end of the buffer
            keyboard_buffet[multi_term[active_terminal_id].buff_idx] = '\n';
            cleanse();                                                                  // now reset the buffer and set the enter flag
            multi_term[active_terminal_id].enter_flag = 1;                                                             // reset the enter flag since no longer pressed
            send_eoi(1);   
                                                                     // calls for the end of the intterupt signal in IRQ 1 
        }

        /* switch to the corresponding terminal depending on which key was pressed*/
        else if(alty == 1 && F_1 == 1){      
           switching_terminals(0); 
        }
        else if(alty == 1 && F_2 == 1){
           switching_terminals(1); 
        }
        else if(alty == 1 && F_3 == 1){
           switching_terminals(2); 
        }
        else{                                                                           // if none of the other conditions are met then
            output = lower_chars(scanCode);                                             // then proceed normally with adding the character to the buffer and printing to the screen
            if (output != -1){
                if(output != '\b'){
                keyboard_buffet[multi_term[active_terminal_id].buff_idx] = output;                                          
                    multi_term[active_terminal_id].buff_idx++; 
                }
                putc_pt2(output);
            }
            send_eoi(1);                                                                // calls for the end of the intterupt signal in IRQ 1 
        }
  
    }

    else if(multi_term[active_terminal_id].buff_idx == 127){                                                    // if we're at the end of the buffer(only one space remaining)

        if(backspace_flag == 1){                                            // if backspace pressed 
            backspace_flag = 0;                                             
            putc_pt2('\b');                                                     // putc_pt2 backspace
            multi_term[active_terminal_id].buff_idx--;                                                          // dec index
            keyboard_buffet[multi_term[active_terminal_id].buff_idx] = '\0';                                    // add a null character at the decremented index
        }

        else if(multi_term[active_terminal_id].enter_flag == 1){                                           // if enter pressed 
            multi_term[active_terminal_id].enter_flag = 0;
            putc_pt2('\n');                                                     // putc_pt2 newline
            keyboard_buffet[multi_term[active_terminal_id].buff_idx] = '\n';                                    // add a newline charater to the buffer
            buffer_filled_flag = 1;
            cleanse();                                                      // reset buffer and load terminal buffer and set enter flag
        }
        
        send_eoi(1);                                                        // calls for the end of the intterupt signal in IRQ 1 
    }
    

}

