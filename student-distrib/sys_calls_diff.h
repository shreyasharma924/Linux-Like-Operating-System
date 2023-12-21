#ifndef _SYS_CALLS_DIFF_H
#define _SYS_CALLS_DIFF_H

#define four_mb 4194304
#define four_kb 4096 
#define user_stack 138412028 //132 -4 MB
#define pro_im_start 134217728 //128mb
#define pro_im_end  138412032 //132mb 
#define mb_136 0x8800000



#include "types.h"
#include "file_sys.h"
#include "rtc.h"
#include "terminal.h"
#include "paging_helpers.h"
#include "x86_desc.h" 
#include "lib.h"
#include "terminal.h"
#include "temp.h"

// Loads the page dire

// opt_table struct 
typedef struct opt_table{

    int (*open)(int8_t* filename);
    int (*close)(int32_t fd);
    int (*read)(int32_t fd,  void* buf, int32_t nbytes);
    int (*write)(int32_t fd,  void* buf, int32_t nbytes);

}octopus_optometrist;

// stuct for flags
 typedef struct  flags {
    uint32_t program_running      : 1;
    uint32_t interruptable        : 1;
    uint32_t  uninterruptable     : 1;
    uint32_t paused               : 1;
    uint32_t killed               : 1;
}flags_t ;

// stuct for our fd 
typedef struct fd{
    octopus_optometrist op_table_ptr; 
    uint32_t inode; 
    uint32_t file_position; 
    uint32_t flags_fd;
}fd_t;  


typedef struct pcb{ // struct for a pcb 
    fd_t fd_table[8]; 
    //uint32_t starting_addr;
    int32_t mother; // parent process - way to return to the inital program (shell)
    uint32_t pcb_esp; 
    uint32_t pcb_ebp; 
    uint32_t pcb_sch_esp;  // esp for scheduling
    uint32_t pcb_sch_ebp; // ebp for scheduling
    flags_t flags;
    int32_t term_id; // the terminal the current process is running in
    int32_t base_shell;
    uint32_t saved_ebp_esp; 
    
    

}pcb_t; 




extern int32_t halt_blossom_handler(uint8_t status);                            // syscall halt
extern int32_t execute_bubbles_handler( int8_t *command);                       // syscall execute                        
extern int32_t read_mojojojo_handler(int32_t fd, void *buf, int32_t nbytes);    // syscall read
extern int32_t write_Utonium_handler(int32_t fd, void *buf, int32_t nbytes);    // syscall write
extern int32_t open_buttercup_handler( uint8_t *filename);                      // syscall open
extern int32_t close_FuzzyLumpkins_handler(int32_t fd);                         // syscall close
int32_t getargs(uint8_t * buf, int32_t nbytes);
int32_t set_handler(int32_t signum, void *handler_address); 
int32_t how_dare_you();
void pcb_init();

dentistry_t d;  // global variable to store the executable file
//dentistry_t actual_file;
                    // global variable 



int8_t cmd[33];  
uint8_t file[33];
pcb_t *active_PCB;                  // current pcb
int32_t active_PCB_pid;             // current pid
pcb_t* pcb_array[6];                // array of pcb pointers
inode_t * cur_read_inode;
int pcb_counter; 
// term_van_luster_t* active_terminal; //displayed



#endif





