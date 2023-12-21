#include "scheduling.h"
// #include "scheduler.S"


#define mb_B9 0xB9000
#define four_kb_shift 12

/*

Description: initializes PIT
Input: none
Output: none
Retrun Value: none
Side Effect: schedules the next process using next_index

*/

void PIT_init(){
    outb(0x36, 0x46);
    outb(((1193182)/175) & 0xFF, 0x40);
    outb(((1193182)/175) >> 8, 0x40);
    enable_irq(0); 
}

/*

Description: handles the pit interrupt 
Input: none
Output: none
Retrun Value: none
Side Effect: schedules the next process using next_index

*/
void PIT_handler(){
    send_eoi(0);
    next_index = (curr_index + 1) % 3; // update next_index to go to the next terminal

    scheduler_set(); 


    return; 

}

/*

Description: handles scheduling process for terminal (next_index)
Input: none
Output: none
Retrun Value: none
Side Effect: schedules the next process using next_index

*/
void scheduler_set(){
    // 2. save esp and ebp for curr process
    
    
    // 1. handle terminal switching (checking global flags like F_1) 
    register uint32_t temp_ebp asm("ebp"); // stores prev ebp 
    register uint32_t  temp_esp asm("esp"); // stores prev esp 

    temp_ebp1 = temp_ebp; 
    temp_esp1 = temp_esp; 

    /*saves the current ebp and esp */
    pcb_array[multi_term[curr_index].recent_process_id]->pcb_sch_esp = temp_esp1; 
    pcb_array[multi_term[curr_index].recent_process_id]->pcb_sch_ebp = temp_ebp1; 
    pcb_array[multi_term[curr_index].recent_process_id]->saved_ebp_esp = 1; 
    
    /*mapping according to whether the current terminal is the same as the terminal being displayed*/
    if(next_index == active_terminal_id){
    //   p_tab[0xBC].addy = 0xB8;
    //0xB8 -> vid mem addy
      p_tab[0xB8].addy = 0xB8;
      vid_p_tab[0].addy =  0xB8;                          // addy and VIDEO holds 20 bits 
      flush_TLB();
    }
    else{
    //   p_tab[0xBC].addy = ((mb_B9)+((active_terminal_id)*four_kb)) >> four_kb_shift;  
    //0xB8 -> vid mem addy
      p_tab[0xB8].addy =  ((mb_B9)+((next_index)*four_kb)) >> four_kb_shift;  
      vid_p_tab[0].addy =  ((mb_B9)+((next_index)*four_kb)) >> four_kb_shift; 
      flush_TLB();
    }
     

     curr_index = next_index; // update current index 
     if(multi_term[next_index].running_flag==0){  //checks whether base shell is intialized and accordinly calls execute shell for terminal
        
     
        multi_term[next_index].running_flag = 1;

        
        clear();
        printf("terminal %u \n", (next_index + 1));
        execute_bubbles_handler("shell"); 
        return;
    }

    /*checks if next scheduled process's ebp and esp have been saved (is this the first time it's been scheduled)*/
    if(pcb_array[multi_term[next_index].recent_process_id]->saved_ebp_esp == 0){
      return; 
    }

    /*restores ebp and esp of scheduled process */
    temp_esp1 =  pcb_array[multi_term[next_index].recent_process_id]->pcb_sch_esp; 
    temp_ebp1 =  pcb_array[multi_term[next_index].recent_process_id]->pcb_sch_ebp;

    active_PCB_pid = multi_term[next_index].recent_process_id; 
    active_PCB = pcb_array[active_PCB_pid];
    map_unmap(multi_term[next_index].recent_process_id);

    tss.ss0 = KERNEL_DS;   // TSS -> set ss0 and esp0(kernel base - kernel stack)
    tss.esp0 = 0x800000 - (multi_term[next_index].recent_process_id * 2 * four_kb)-4; // 0x80000 top of kernel stack and subtract to get curent location  

    
     
  

  scheduler_context_switch(temp_ebp1,temp_esp1); 
    
  // 3. switch to scheduled process (queue or for loop)
    

    
    


 // 4. unmap the paging and map (vid mem switch) for 128 mb and vid memory, check for fish

    
}
// qs:
//1. saving cursor pos? and what do we save (show they keyboard.c)
//2. time slice wtf? 
