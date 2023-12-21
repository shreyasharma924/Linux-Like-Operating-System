#include "terminal_switch.h"

#define mb_B9 0xB9000
#define four_kb_shift 12

/*

Description: initializes terminal array
Input: none
Output: none
Retrun Value: none
Side Effect: none

*/
void init_switching_terminals(){
    int i = 0; 
    int j; 

    for (i=0; i<3; i++){ // 3 terminals
        multi_term[i].cursor_van_x = 0;
        multi_term[i].cursor_van_y = 0; 
        multi_term[i].running_flag = 0;
        multi_term[i].rtc_counter = 0 ; 
        multi_term[i].rtc_flag = 0;
        multi_term[i].rtc_frequency = 0; 
        // multi_term[i].p_diddy = i ; 
        multi_term[i].recent_process_id = i;
        multi_term[i].init_flag = 0;
        multi_term[i].pcb_count = 0;
        multi_term[i].interrupt_flag = 0;
        multi_term[i].inter_counter = 0; 
        
        for (j = 0; j < NUM_ROWS * NUM_COLS; j++) {
            *(uint8_t *)((mb_B9+(i*4096)) + (j << 1)) = ' '; // 4096->size of 4kb pages
            *(uint8_t *)((mb_B9+(i*4096))  + (j << 1) + 1) = ATTRIB; // 4096->size of 4kb pages
    }
    }
    multi_term[0].running_flag = 1;

    
    // multi_term[0].running_flag = 1;
    // multi_term[0].init_flag = 1;

}

/*

Description: handles switching between terminals
Input: none
Output: none
Retrun Value: none
Side Effect: saves information for terminal we're switching from and restoring information for what we're switching to

*/
uint32_t switching_terminals(uint32_t term_to_switch_to){

    cli();
    // error check that is a valid terminal id
    if(term_to_switch_to > 2 || term_to_switch_to < 0){ // 2-> all three terminals initialized
        return -1; // invalid terminal id
    }
       
    update_cursor();                                                            // set this change in memory
    int i; 
    for(i=0; i<128; i++){ // 128 -> max size of keyboard buff
        multi_term[active_terminal_id].termie_buffet[i] = keyboard_buffet[i];
    }
    for(i=0; i<128; i++){ // 128 -> max size of keyboard buff
        keyboard_buffet[i] = multi_term[term_to_switch_to].termie_buffet[i];
    }

    // 0xB8 -> video memory
   
    uint32_t saved =  p_tab[0xB8].addy;

    multi_term[active_terminal_id].cursor_van_x = get_screen_X(); // update x position for terminal
    multi_term[active_terminal_id].cursor_van_y = get_screen_y();// update y position for terminal

    //p_tab[0xB8].addy = ((mb_B9)+((active_terminal_id)*four_kb)) >> four_kb_shift;
    p_tab[0xB8].addy =0xB8;
    flush_TLB();
    memcpy( (void*)((mb_B9)+(active_terminal_id*4096)),(void*) VIDEO_M , 4096); // save what's currently being displayed (terminal we're switching from) 4096->size of 4kb pages
    // clear();
    active_terminal_id = term_to_switch_to;
    
    

    // update the cursor in the next terminal
    set_cursor( multi_term[term_to_switch_to].cursor_van_x ,  multi_term[term_to_switch_to].cursor_van_y);
    update_cursor();
    memcpy((void*) VIDEO_M, (void*)((mb_B9)+(term_to_switch_to*4096)), 4096); //display terminal that we're switching to  4096->size of 4kb pages
    p_tab[0xB8].addy = saved;
    flush_TLB();
  
    sti();

    return 0; 
}
// 1. having shells in the pcb array
// 2. can we just change vid mem to the buffer for the terminal pages instead of making two putc's 





