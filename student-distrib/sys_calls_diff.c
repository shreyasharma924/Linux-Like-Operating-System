#include "sys_calls_diff.h"

/*

Description: initializes pcb array
Input: none
Output: none
Retrun Value: none
Side Effect: fills pcb array with pointers

*/
void pcb_init(){
    int i; 
    pcb_counter = -1;
    for (i = 0; i < 6; i++){
        pcb_array[i] = (pcb_t*) (2*four_mb - (2 * four_kb*(i + 1)));       // creates the pcb and p points to the pcb      // adds this to the array of pointers to the pcb
        pcb_array[i]->base_shell = -1; 
        pcb_array[i]->mother = -1; 
        pcb_array[i]->flags.program_running = 0;
        pcb_array[i]->saved_ebp_esp = 0; 
    }
}
/*

Description: Executes out sytem calls
Input: comand (inputed user program to execute)
Output: none
Retrun Value: none
Side Effect: executes the program in user space

*/
int32_t execute_bubbles_handler(  int8_t *command){
    cli();
    // terminal_cur_reading_flag =0;
    // uint8_t file[33];
    // looping variables
    int i; 
    int j;
    
    
    // uint8_t file[32];               // executable command (32 -> max file name length)
    // int8_t cmd[32];                 // name of file (32 -> max file name length)
    uint8_t elf_buf[4];             // stores the first few character of executable file (validates that it is an executable) (4 -> is 4 charaters)
    uint8_t eip_buf[6000];          // buffer that stores entry points into the file (6000 -> made buffer a big number 6000)
    uint32_t eip = 0;               // eip
    
    for (i  = 0; i  < 33; i++){     // initalized the file and command to null characters (32 -> max file name length)
         cmd[i] =  '\0';
         file[i] = '\0';
    }
   
    for (i  = 0; i  < 33; i++){     // parses user input and fills file buffer with the first word (so we know what we are executing) (33 -> max file name length)
        if (command[i] == ' ' || command[i] == '\0' || command[i] == '\n'){
          break; 
        }
     memcpy(file +i, command + i, 1); 
    }

    int k = 0;

    
    for (j  = i + 1; j  < 66; j++){     // fills the cmd buff with the second word (for rtc and text files) (66 -> to include the entire second arg length)
        if (command[j] == '\0' || command[j] == '\n'){
            break; 
            }
        memcpy(cmd+k, command + j, 1);
        k++;
    }
    
    
    if(read_dentry_by_name((int8_t*) file, &d)== -1){   // see if the file is valid
        return -1;
    } 


    // if(active_PCB_pid >= 2 && pcb_array[2]->flags.program_running == 1){
    //     multi_term[active_terminal_id].pcb_count +=1; 
    // }

   
    
    // else if(active_PCB_pid < 2 && pcb_array[active_PCB_pid]->flags.program_running == 1){
    //     multi_term[active_terminal_id].pcb_count +=1; 
    // }
    

    pcb_counter +=1; 

// if (pcb_counter < 0 || pcb_counter > 5){      // validates the pid (5 -> since zero indexed can be more than 5 (6 pids allowed only))
//         pcb_counter--;
//         multi_term[active_terminal_id].pcb_count --; 
//         printf("--out of PCBs--\n"); 
//         return 0; 
// }

/*finds first empty entry in pcb array and grabs the pid*/
    for(i=0; i<7; i++){
        if(i == 6){
            multi_term[active_terminal_id].pcb_count --; 
            printf("--out of PCBs--\n"); 
            return 0; 
        }
        if(pcb_array[i]->flags.program_running == 0){
            active_PCB_pid = i; 
            break; 
        }
        
    }
// }
    if(active_PCB_pid >= 3){
        multi_term[active_terminal_id].pcb_count +=1; 
    }
                                       // adds this to the array of pointers to the pcb
    active_PCB = pcb_array[active_PCB_pid];                                                             // our active pcb is a pointer to the current pcb
    
    if(active_PCB_pid > 2){                                  // set the parent pcb when this is not the first process
        active_PCB->mother = multi_term[active_terminal_id].recent_process_id;
        multi_term[active_terminal_id].recent_process_id = active_PCB_pid;

    }
    else{                                                   // set the parent pcbn when this is the first process
         active_PCB->mother = -1;               
    }


    active_PCB->flags.program_running = 1;                  // setting the flag because this program is active 

 // ----- setting up for paging-----//
    map_unmap(active_PCB_pid);
   // ------TSS SETUP--------//
   
    tss.ss0 = KERNEL_DS;                                        // TSS -> set ss0 and esp0(kernel base - kernel stack)
    tss.esp0 = 0x800000 - (active_PCB_pid * 2 * four_kb)-4;     // TSS -> memory address (0x800000 -> memory location)

    // active_PCB_pid +=1;                                                 // increments the pcb

    inode_t *current_Inode = (inode_t*)(d.inode_num + inode_start);     // the current inode for the current file that we are in 

    
    current_fd = 2; // 2 -> sets the fd to 2 for read data's file position
    active_PCB->fd_table[current_fd].file_position = 0; // sets file position to zero
    read_data (d.inode_num,0,(int8_t*)0x08048000,current_Inode->length); // read the file contents into  the kernel stack (0x08048000 -> start of the kernel stack)


    //----file validity----//

    if(read_data (d.inode_num,0,(int8_t*) elf_buf,4)==-1){ // reading 4 bytes
        return -1; 
    }
    
   

    if((elf_buf[0] == 0x7F && elf_buf[1] == 0x45 && elf_buf[2] == 0x4C && elf_buf[3] == 0x46) != 1 ){ // ffinds elf
        return -1; 
    }

    active_PCB->fd_table[1].op_table_ptr.write = terminal_write;             // sets stdout for fd table
    active_PCB->fd_table[1].flags_fd= 1;
    active_PCB->fd_table[1].op_table_ptr.read = how_dare_you;                // sets fd 1's read to a null function 
    active_PCB->fd_table[0].op_table_ptr.read = terminal_read;               // sets stdin for fd table
    active_PCB->fd_table[0].flags_fd =1;
    active_PCB->fd_table[0].op_table_ptr.write = how_dare_you; 

                 // sets fd 1's read to a null function 
    
    // clears the current pid's program info to set up for the next pid
      for(i = 2; i< 8; i++){ // 8 spots in  the fd table
        //  active_PCB->fd_table[i].
        active_PCB->fd_table[i].flags_fd = 0; // sets the files to be inactive
        active_PCB->fd_table[i].file_position  =0; 
    }

    // read the program data into the page we just created

    if(read_data (d.inode_num,24,(int8_t*) eip_buf,4)==-1){                 // find the entry point in the file
        return -1; 
    }

    current_fd = 2; // 2 -> sets the fd to 2 for read data's file position
    active_PCB->fd_table[current_fd].file_position = 0; // sets file position to zero

    
    eip = (eip_buf[3]<<24)+ (eip_buf[2]<<16)+(eip_buf[1]<<8)+(eip_buf[0]);   // place the eip value into a variabel in order to push to the stack

    //---------CONTEXT SWITCH--------// 
    register uint32_t temp_ebp asm("ebp"); // stores current ebp
    register uint32_t temp_esp asm("esp"); // stores current esp
    active_PCB->pcb_esp = temp_esp; 
    active_PCB->pcb_ebp = temp_ebp; 
  
    asm volatile (  
        "movw $0x2B, %%ax;"
        "movw %%ax, %%ds;"           
        "pushl   %0;"                      
        "pushl   %1;"                      
        "pushfl;" 
        "popl %%eax;"
        "orl $0x00000200, %%eax;"                         
        "pushl %%eax;" 
        "pushl   %2;"                   
        "pushl   %3;"        
        "iret;" 
        "DIP: ;"
        
        :
        : "r"(USER_DS), "r"(user_stack),"r"(USER_CS),"r"(eip)
        : "memory", "eax" 
    );

     //printf("pic drawn?");
    // active_PCB->halt_addr  = halt_addr_temp;
 
    return 0; 

}


/*

Description: Halts out sytem calls
Input: status (the code that represents the status of the user program)
Output: none
Retrun Value: none
Side Effect: ends the current user program and returns to its parent program

*/

int32_t halt_blossom_handler(uint8_t status){
     cli();

    terminal_cur_reading_flag = 0; 

    //clear pcb - set program running flag to false - be careful with overwriting
    active_PCB->flags.program_running = 0; 

    

    
    // uint32_t temp_esp = active_PCB->pcb_esp;                        
    uint32_t temp_ebp = active_PCB->pcb_ebp; 
    
    /*close fd table*/
    int i; 
    for(i  = 0; i < 8; i++){
        close_FuzzyLumpkins_handler(i);
    }
    

    

    /*checks if we're exiting out of base shell*/
    

    if(p_tab[0xB8].addy == 0xB8){
        if (multi_term[active_terminal_id].recent_process_id == 0 || multi_term[active_terminal_id].recent_process_id == 1 || multi_term[active_terminal_id].recent_process_id == 2){
        // printf("Starting 391 Shell \n");
        // multi_term[active_terminal_id].running_flag==0;
        // active_PCB->flags.program_running = 1; 
        // active_PCB_pid = active_terminal_id;
        pcb_array[multi_term[active_terminal_id].recent_process_id]->flags.program_running = 0;
        pcb_counter -=1; 
        execute_bubbles_handler("shell");
    }

        pcb_counter = pcb_counter -1;

        multi_term[active_terminal_id].pcb_count -=1; 
        pcb_array[multi_term[curr_index].recent_process_id]->saved_ebp_esp = 0; 
        multi_term[active_terminal_id].recent_process_id = pcb_array[multi_term[active_terminal_id].recent_process_id]->mother;
        map_unmap(multi_term[active_terminal_id].recent_process_id );
        // video_UNMAP_func();         // unmap the PDE for video memory if used 
        // 2 -> doubles four, to calculate 8 mb and 8 kb

        active_PCB_pid = multi_term[active_terminal_id].recent_process_id;

    active_PCB = (pcb_t*) (2*four_mb - (2 * four_kb*(multi_term[active_terminal_id].recent_process_id + 1)));     // set the active pcb's location in memory back to the parent's 

   
    // reset paging back to parent
    
     // right before jump - clear TSS
    tss.ss0 = KERNEL_DS;   // TSS -> set ss0 and esp0(kernel base - kernel stack)
    tss.esp0 = 0x800000 - (multi_term[active_terminal_id].recent_process_id * 2 * four_kb)-4; // 0x80000 top of kernel stack and subtract to get curent location
 

    }
    else{
        if (multi_term[next_index].recent_process_id == 0 || multi_term[next_index].recent_process_id == 1 || multi_term[next_index].recent_process_id == 2){
        // printf("Starting 391 Shell \n");
        // multi_term[active_terminal_id].running_flag==0;
        // active_PCB->flags.program_running = 1; 
        // active_PCB_pid = next_index;
        pcb_counter -=1; 
        pcb_array[multi_term[next_index].recent_process_id]->flags.program_running = 0;

        execute_bubbles_handler("shell");
    }
        pcb_counter = pcb_counter -1;

        multi_term[next_index].pcb_count -=1; 
        pcb_array[multi_term[curr_index].recent_process_id]->saved_ebp_esp = 0; 
        multi_term[next_index].recent_process_id = pcb_array[multi_term[next_index].recent_process_id]->mother;
        active_PCB_pid = multi_term[next_index].recent_process_id;

    
        // basically unmap
        map_unmap(multi_term[next_index].recent_process_id );
        // video_UNMAP_func();         // unmap the PDE for video memory if used 
    // 2 -> doubles four, to calculate 8 mb and 8 kb

   

    active_PCB = (pcb_t*) (2*four_mb - (2 * four_kb*(multi_term[next_index].recent_process_id + 1)));     // set the active pcb's location in memory back to the parent's 

   
    // reset paging back to parent
    
     // right before jump - clear TSS
    tss.ss0 = KERNEL_DS;   // TSS -> set ss0 and esp0(kernel base - kernel stack)
    tss.esp0 = 0x800000 - (multi_term[next_index].recent_process_id * 2 * four_kb)-4; // 0x80000 top of kernel stack and subtract to get curent location
    }
    
 
    
 
    // active_PCB_pid +=1;

    // active_PCB_pid = multi_term[next_index].recent_process_id; // updates pid 

    halt_context(temp_ebp, (uint32_t) status);


    return 0; 
}



/*

Description: Reads the user program that needs to be executed
Input: fd (current file descriptor), buffer (buffer holding contents read)), nbytes (the amount bytes to read)
Output: none
Retrun Value: the number of bytes read
Side Effect: none

*/
int32_t read_mojojojo_handler(int32_t fd, void *buf, int32_t nbytes){
    if (fd < 0 || fd > 7){ // 7 -> number of spots in fd
      
        return -1; 
    }
    
      
   if(active_PCB->fd_table[fd].flags_fd == 0){ // checks if a file is open
        return -1;
   }

  

   sys_flag = 1; 
   int retVal =  active_PCB->fd_table[fd].op_table_ptr.read(fd,buf,nbytes); // read file


   sys_flag = 0; 
   
  
   return retVal; 
}


/*

Description: Writes to the user program that needs to be executed
Input: fd (current file descriptor), buffer (buffer holding contents read)), nbytes (the amount bytes to write)
Output: none
Retrun Value: the number of bytes written
Side Effect: none

*/
int32_t write_Utonium_handler(int32_t fd, void *buf, int32_t nbytes){
    
   
    if (fd < 0 || fd > 7){ // 7 -> number of spots in fd
        return -1; 
    }
    
    if(active_PCB->fd_table[fd].flags_fd == 0){
        return -1;
    }

    sys_flag = 1;
  
    int retVal = active_PCB->fd_table[fd].op_table_ptr.write(fd,buf,nbytes); // write to the file
    sys_flag = 0; 
    
    return retVal; 
}

/*

Description: Sets up file descripter table for the respective file type
Input: filename (name of file)
Output: none
Retrun Value: none
Side Effect: fills up the fd table for the respective file 

*/

int32_t open_buttercup_handler( uint8_t *filename){
   
   
    int i;
    
    // validates the file
    
    if (cmd[0] == '\0'){
        if(read_dentry_by_name((int8_t*)filename,&d)==-1){
        
            return -1;
        }
    }
    else{
        if (strlen(cmd) >32) {
            return -1;
        }
        if(read_dentry_by_name((int8_t*)filename,&curFile)==-1){


         return -1;
        }
        d = curFile;
    }
    
    // checking for an empty file descriptor
    for(i =2; i < 8; i++){
        if(active_PCB->fd_table[i].flags_fd == 0){
            break;
        }
    }
    if (i == 8){
        return -1;
    }
    // based on the file type, we fill up the file descriptor with its necessary functions

    if(d.filetype == 0){ // rtc
        active_PCB->fd_table[i].op_table_ptr.open = (void*)open_rtc;
        active_PCB->fd_table[i].op_table_ptr.close = (void*)close_rtc;
        active_PCB->fd_table[i].op_table_ptr.read = (void*)read_rtc;
        active_PCB->fd_table[i].op_table_ptr.write = (void*)write_rtc;
        active_PCB->fd_table[i].flags_fd =1;
        active_PCB->fd_table[i].inode = d.inode_num;
    }
    else if (d.filetype == 1){ // directory
        active_PCB->fd_table[i].op_table_ptr.open = (void*)directory_open;
        active_PCB->fd_table[i].op_table_ptr.close = (void*)directory_close;
        active_PCB->fd_table[i].op_table_ptr.read = (void*)directory_read;
        active_PCB->fd_table[i].op_table_ptr.write = (void*)directory_write;
        active_PCB->fd_table[i].flags_fd = 1;
        active_PCB->fd_table[i].inode = d.inode_num;
    }
    else{ // file systems
        active_PCB->fd_table[i].op_table_ptr.open = (void*)file_open;
        active_PCB->fd_table[i].op_table_ptr.close = (void*)file_close;
        active_PCB->fd_table[i].op_table_ptr.read = (void*)file_read;
        active_PCB->fd_table[i].op_table_ptr.write = (void*)file_write;
        active_PCB->fd_table[i].flags_fd = 1;
        active_PCB->fd_table[i].inode = d.inode_num;
        active_PCB->fd_table[i].file_position  = 0;
        // eofflag = 0; 
    }

  
    return i;
}
 

 /*

Description: closes file descripter table
Input: fd (current file descriptor)
Output: none
Retrun Value: none
Side Effect: make fd table at that fd avaliable (meaning it can now be used)

*/
 
int32_t close_FuzzyLumpkins_handler(int32_t fd){
    // if the fd is invalid retun -1 or if the slot is avaliable
    if (fd < 2 || fd > 7 ||  active_PCB->fd_table[fd].flags_fd != 1){ // 7 -> number of spots in fd
        return -1; 
    }
    // else set functions to null
    active_PCB->fd_table[fd].flags_fd = 0;
    active_PCB->fd_table[fd].file_position = 0;
    active_PCB->fd_table[fd].op_table_ptr.close = NULL; 
    active_PCB->fd_table[fd].op_table_ptr.open = NULL; 
    active_PCB->fd_table[fd].op_table_ptr.write = NULL; 
    active_PCB->fd_table[fd].op_table_ptr.read = NULL; 
    return 0; 
}


 /*

Description: empty fuction for terminal functions
Input: none
Output: none
Retrun Value: -1
Side Effect: none

*/
int32_t how_dare_you(){
    return -1;
}


 /*

Description: gets the argument and stores it in a buffer for further processes
Input: buf to fill with string, how many bytes to fill the buffer
Output: none
Retrun Value: none
Side Effect: none

*/

int32_t getargs(uint8_t * buf, int32_t nbytes){
    if(cmd == '\0' || nbytes <= 0){
        return -1;
    }

    memcpy((int8_t*)buf,cmd,nbytes);
    return 0; 
}


 /*

Description: maps vidoes memory
Input: address for the start of the screen
Output: none
Retrun Value: none
Side Effect: chacking if the address is valid and mapping it the virtual address of the video memory from C1

*/

int32_t vidmap(uint8_t** screen_start){
    //cli();
    if (screen_start == NULL){              // checking if screen_start is pointing to a valid addess
        return -1;
    }
    if (((int32_t)screen_start < pro_im_start ) || ((int32_t)screen_start > pro_im_end )){      // checking if screen_start is within the bounds of the program image (128MB - 132MB)
        return -1;                                                                              // if not return -1
    }

    //active_PCB->fd_table[1].op_table_ptr.write = how_dare_you; 
    video_map_func();                       // create a new 4k page for video memory
    *screen_start = (uint8_t*) mb_136;      // set screen_start to the virtul memory that will map to this new page
    //sti();
    return 0; 
}


 /*

Description: none
Input: none
Output: none
Retrun Value: none
Side Effect: none

*/

int32_t set_handler(int32_t signum, void *handler_address){
   
    if(handler_address == NULL){
        
    }
    
    return 0; 
}

 /*

Description: none
Input: none
Output: none
Retrun Value: none
Side Effect: none

*/


int32_t sigreturn(){
    return 0; 
}




