#include "paging_helpers.h"
//#include "paging.S"

#define four_kb_shift 12
#define four_mb_shift 22
#define four_mb 4194304
#define eight_mb 8388608
#define VIDEO 0xB8000
#define NUM_EN 1024
#define mb_B9 0xB9000
#define four_kb 4096

/*
Description: initializes the different pages 
Input: None
Output: None
Side Effects: Creates the page directory along with two entries that represent the two ways to create virtual memory. 
              This also loads the page directory and enables paging.
*/
void page_init(){
    int i;
    
    for(i=0; i<NUM_EN;i++){
        
        /* first table (PDE). Will hold video memory (KB)*/
        if(i==0){
            
            p_dir[i].KB.present_bit = 1;                                    // All valid PDEs need to have their present bit set to 1
            p_dir[i].KB.r_w_bit = 1;                                        // mark all pages read/write.
            p_dir[i].KB.user_sup_bit = 0;                                   // 1: user privilege level for everything that isn't video (privilege levels 0-3 can access this memory), we run user-level code at pl 3, allows us to mark pages as kernel only
            p_dir[i].KB.pwt_bit = 0;                                        // always want writeback, if page being cached, this bit makes the caching write through and not writeback 
            p_dir[i].KB.pcd_bit = 0;                                        // set to 0 for video memory (because these pages contain mem mapped I/O and shouldn't be cached), won't cache any memory that falls in this PDE's range 
            p_dir[i].KB.accessed_bit = 0;                                   // won't use this bit (1 when a process touches memory that this PDE maps/ access is made to a virtual addr that corresponds to this entry)
            p_dir[i].KB.reserved_bit = 0;                                   // always 0
            p_dir[i].KB.page_size_bit = 0;                                  // 0 for 4kb 
            p_dir[i].KB.global_bit = 0;                                     // only set for kernel pages (virtual→physical translations for this page visible to all processes, TLB entries won't be cleared when CR3 reloaded)
            p_dir[i].KB.available_bits = 0;                                 // available for our use but we're not going to ues these bits 
            p_dir[i].KB.addy = ((uint32_t)p_tab) >> four_kb_shift;      // addy only holds 20 bits but addr of p_tab is 32 bits

            // p_tab gets the starting addy (head) of paging table, shift by 12 (last 12 bits only give us byte within page)
        }

        /* kernel page (4MB), map from directory directly to kernel code page*/
        else if(i==1){
           
            p_dir[i].MB.present_bit = 1;                    //All valid PDEs need to have their present bit set to 1
            p_dir[i].MB.r_w_bit = 1;                        // mark all pages read/write.
            p_dir[i].MB.user_sup_bit = 0;                   // supervisor privilege level, 0 for kernel pages, (privilege levels 0-2 can access this memory, but not privilege level 3)
            p_dir[i].MB.pwt_bit = 0;                        // always want writeback, if page being cached, this bit makes the caching write through and not writeback 
            p_dir[i].MB.pcd_bit = 0;                        // set to 1 for program code and data pages (kernel pages and program pages), caches memory in this PDE's range 
            p_dir[i].MB.accessed_bit = 0;                   // won't use this bit (1 when a process touches memory that this PDE maps/ access is made to a virtual addr that corresponds to this entry)
            p_dir[i].MB.dirty_bit = 0;                      // set to 1 by processor if page has been written to, bit remains 1 until cleared by software, won't use so set to 0
            p_dir[i].MB.page_size_bit = 1;                  // 1 for 4mb 
            p_dir[i].MB.global_bit = 1;                     // only set for kernel pages (virtual→physical translations for this page visible to all processes, TLB entries won't be cleared when CR3 reloaded)
            p_dir[i].MB.available_bits = 0;                 // available for our use but we're not going to ues these bits 
            p_dir[i].MB.pat_bit = 0;                        // page attribute table index, won't use it so set to 0 
            p_dir[i].MB.reserved_bits = 0;                  // must be 0 or they'll get an invalid PDE exception 
            
            p_dir[i].MB.addy = 1;
            //addy is only 10 bits, shift the last 22 bits (2^22 = 4 MB), s kernel page should be a single 4 MB page (4-8MB)
            //p_dir[i].MB.addy = 1;                           // ADDY (4MB >> (32- 10) == 1) (four_mb >> four_mb_shift) * 
        }

        /* unused page directory entries */
        else{
            
            // if 0, the page range associated with this PDE/PTE does not exist
            p_dir[i].KB.present_bit = 0;                    // 8 MB to 4GB should be marked not present and any unused pages should be marked not present
            p_dir[i].KB.r_w_bit = 0;                        // mark all pages read/write.
            p_dir[i].KB.user_sup_bit = 0;                   // 1: user privilege level for everything that isn't video (privilege levels 0-3 can access this memory), we run user-level code at pl 3, allows us to mark pages as kernel only
            p_dir[i].KB.pwt_bit = 0;                        // always want writeback, if page being cached, this bit makes the caching write through and not writeback 
            p_dir[i].KB.pcd_bit = 0;                        // set to 0 for non kernel pages, won't cache memory in this PDE's range 
            p_dir[i].KB.accessed_bit = 0;                   // won't use this bit (1 when a process touches memory that this PDE maps/ access is made to a virtual addr that corresponds to this entry)
            p_dir[i].KB.reserved_bit = 0;                   // always 0
            p_dir[i].KB.page_size_bit = 0;                  // 0 for 4k 
            p_dir[i].KB.global_bit = 0;                     // only set for kernel pages (virtual→physical translations for this page visible to all processes, TLB entries won't be cleared when CR3 reloaded)
            p_dir[i].KB.available_bits = 0;                 // available for our use but we're not going to ues these bits 
            p_dir[i].KB.addy = 0;                           // addy only holds 20 bits but addr of p_tab is 32 bits
        }

    }

    // B8 == VIDEO/4096 (VIDEO >> 12), B9 == VIDEO/4096 + 4KB (VIDEO >> 12) ~ terminal 1, BA == VIDEO/4096 + 8KB (VIDEO >> 12) ~ terminal 2, BB == VIDEO/4096 + 12KB (VIDEO >> 12) ~ terminal 3, BC == VIDEO/4096 + 16KB (VIDEO >> 12) ~ putc content    
    for(i=0; i<NUM_EN; i++){
        if( i == 0xB8 || i == 0xB9 || i == 0xBA || i == 0xBB || 0xBC) {      //create pages for vid mem as well as terminal pages  
            p_tab[i].present_bit = 1;                   // 1: marks page as valid, 0: page range assoc. w/ this PTE doesn't exist 
            p_tab[i].r_w_bit = 1;                       // 1: pages are r/w, 0 : read only, mark all pages r/w 
            p_tab[i].user_sup_bit = 0;                  // if 1, memory mapped by this PTE is user-level (PL 0-3 can access this memory), 0: memory is supervisor only (PL 0-2 can access), 0 for kernel pages
            p_tab[i].pwt_bit = 0;                       // always want writeback, if page being cached, this bit makes the caching write through and not writeback 
            p_tab[i].pcd_bit = 0;                       // set to 0 for video memory (because these pages contain mem mapped I/O and shouldn't be cached), won't cache any memory that falls in this PDE's range 
            p_tab[i].accessed_bit = 0;                  // won't use this bit (1 when a process touches memory that this PDE maps/ access is made to a virtual addr that corresponds to this entry)
            p_tab[i].dirty_bit = 0;                     // set to 1 by processor if page has been written to, bit remains 1 until cleared by software, won't use so set to 0
            p_tab[i].pat_bit = 0;                       // page attribute table index, won't use it so set to 0
            p_tab[i].global_bit = 0;                    // only set for kernel pages (virtual→physical translations for this page visible to all processes, TLB entries won't be cleared when CR3 reloaded)
            p_tab[i].available_bits = 0;                // available for our use but won't use 
            p_tab[i].addy = i;                          // addy and VIDEO holds 20 bits 
        }

        else{
            p_tab[i].present_bit = 0;                   // 1: marks page as valid, 0: page range assoc. w/ this PTE doesn't exist 
            p_tab[i].r_w_bit = 1;                       // 1: pages are r/w, 0 : read only, mark all pages r/w 
            p_tab[i].user_sup_bit = 0;                  // if 1, memory mapped by this PTE is user-level (PL 0-3 can access this memory), 0: memory is supervisor only (PL 0-2 can access), 0 for kernel pages
            p_tab[i].pwt_bit = 0;                       // always want writeback, if page being cached, this bit makes the caching write through and not writeback 
            p_tab[i].pcd_bit = 0;                       // set to 0 for non kernel pages, won't cache memory in this PDE's range 
            p_tab[i].accessed_bit = 0;                  // won't use this bit (1 when a process touches memory that this PDE maps/ access is made to a virtual addr that corresponds to this entry)
            p_tab[i].dirty_bit = 0;                     // set to 1 by processor if page has been written to, bit remains 1 until cleared by software, won't use so set to 0
            p_tab[i].pat_bit = 0;                       // page attribute table index, won't use it so set to 0
            p_tab[i].global_bit = 0;                    // only set for kernel pages (virtual→physical translations for this page visible to all processes, TLB entries won't be cleared when CR3 reloaded)
            p_tab[i].available_bits = 0;                // available for our use but won't use 
            p_tab[i].addy = i;                   // index times the amount of bytes in an entry which is 4096
        }
    }
    
    p_tab[0xBC].addy = 0xB8; // mapping to vid mem
    load_page_directory((unsigned int*)p_dir);           // loads our page directory and enables paging 
}


/*
Description: initializes the page for user program
Input: p_id (program id)
Output: None
Side Effects: Checks the validity of our p_id value and creates the page directory entry at PDE 32 for the usedr program
*/
void map_unmap(uint32_t p_id){
    /* kernel page (4MB), map from directory directly to kernel code page*/
       
       if (p_id < 0 || p_id > 5){ // 6 -> only 6 programs
            // printf("cant page");
            return; 
       }
        // we want to pla   
        p_dir[32].MB.present_bit = 1;                    //All valid PDEs need to have their present bit set to 1
        p_dir[32].MB.r_w_bit = 1;                        // mark all pages read/write.
        p_dir[32].MB.user_sup_bit = 1;                   // supervisor privilege level, 0 for kernel pages, (privilege levels 0-2 can access this memory, but not privilege level 3)
        p_dir[32].MB.pwt_bit = 0;                        // always want writeback, if page being cached, this bit makes the caching write through and not writeback 
        p_dir[32].MB.pcd_bit = 0;                        // set to 1 for program code and data pages (kernel pages and program pages), caches memory in this PDE's range 
        p_dir[32].MB.accessed_bit = 0;                   // won't use this bit (1 when a process touches memory that this PDE maps/ access is made to a virtual addr that corresponds to this entry)
        p_dir[32].MB.dirty_bit = 0;                      // set to 1 by processor if page has been written to, bit remains 1 until cleared by software, won't use so set to 0
        p_dir[32].MB.page_size_bit = 1;                  // 1 for 4mb 
        p_dir[32].MB.global_bit = 0;                     // only set for kernel pages (virtual→physical translations for this page visible to all processes, TLB entries won't be cleared when CR3 reloaded)
        p_dir[32].MB.available_bits = 0;                 // available for our use but we're not going to ues these bits 
        p_dir[32].MB.pat_bit = 0;                        // page attribute table index, won't use it so set to 0 
        p_dir[32].MB.reserved_bits = 0;                  // must be 0 or they'll get an invalid PDE exception 
        
        p_dir[32].MB.addy = ( eight_mb + (p_id * four_mb)) >> 22;
        //addy is only 10 bits, shift the last 22 bits (2^22 = 4 MB), s kernel page should be a single 4 MB page (4-8MB)
        
        //load_page_directory((unsigned int*)p_dir);           // loads our page directory and enables paging 
        flush_TLB(); // flush tlb
     
}

/*
Description: initializes a 4KB page to be mapped to video memory
Input: None
Output: None
Side Effects: maps the new 4KB page to the already created video memory in physical memory
*/
void video_map_func(){
    /* video mem page (4KB), map from directory to page table to video memory */

        int i;
        // 34 -> 136MB (location in virtual memory) / 4MB (size of each pde) = entiry 34
        p_dir[34].KB.present_bit = 1;                                    // All valid PDEs need to have their present bit set to 1
        p_dir[34].KB.r_w_bit = 1;                                        // mark all pages read/write.
        p_dir[34].KB.user_sup_bit = 1;                                   // 1: user privilege level for everything that isn't video (privilege levels 0-3 can access this memory), we run user-level code at pl 3, allows us to mark pages as kernel only
        p_dir[34].KB.pwt_bit = 0;                                        // always want writeback, if page being cached, this bit makes the caching write through and not writeback 
        p_dir[34].KB.pcd_bit = 0;                                        // set to 0 for video memory (because these pages contain mem mapped I/O and shouldn't be cached), won't cache any memory that falls in this PDE's range 
        p_dir[34].KB.accessed_bit = 0;                                   // won't use this bit (1 when a process touches memory that this PDE maps/ access is made to a virtual addr that corresponds to this entry)
        p_dir[34].KB.reserved_bit = 0;                                   // always 0
        p_dir[34].KB.page_size_bit = 0;                                  // 0 for 4kb 
        p_dir[34].KB.global_bit = 0;                                     // only set for kernel pages (virtual→physical translations for this page visible to all processes, TLB entries won't be cleared when CR3 reloaded)
        p_dir[34].KB.available_bits = 0;                                 // available for our use but we're not going to ues these bits 

        p_dir[34].KB.addy = ((uint32_t)vid_p_tab) >> four_kb_shift;      // addy only holds 20 bits but addr of vid_p_tab is 32 bits
        //addy is only 10 bits, shift the last 22 bits (2^22 = 4 MB), s kernel page should be a single 4 MB page (4-8MB)
        
        for(i=0; i<NUM_EN; i++){
            if( i == 0) {                                // 0 -> first entry in the page table
                vid_p_tab[i].present_bit = 1;                   // 1: marks page as valid, 0: page range assoc. w/ this PTE doesn't exist 
                vid_p_tab[i].r_w_bit = 1;                       // 1: pages are r/w, 0 : read only, mark all pages r/w 
                vid_p_tab[i].user_sup_bit = 1;                  // if 1, memory mapped by this PTE is user-level (PL 0-3 can access this memory), 0: memory is supervisor only (PL 0-2 can access), 0 for kernel pages
                vid_p_tab[i].pwt_bit = 0;                       // always want writeback, if page being cached, this bit makes the caching write through and not writeback 
                vid_p_tab[i].pcd_bit = 0;                       // set to 0 for video memory (because these pages contain mem mapped I/O and shouldn't be cached), won't cache any memory that falls in this PDE's range 
                vid_p_tab[i].accessed_bit = 0;                  // won't use this bit (1 when a process touches memory that this PDE maps/ access is made to a virtual addr that corresponds to this entry)
                vid_p_tab[i].dirty_bit = 0;                     // set to 1 by processor if page has been written to, bit remains 1 until cleared by software, won't use so set to 0
                vid_p_tab[i].pat_bit = 0;                       // page attribute table index, won't use it so set to 0
                vid_p_tab[i].global_bit = 0;                    // only set for kernel pages (virtual→physical translations for this page visible to all processes, TLB entries won't be cleared when CR3 reloaded)
                vid_p_tab[i].available_bits = 0;                // available for our use but won't use 
                // vid_p_tab[i].addy =  ((mb_B9)+((active_terminal_id)*four_kb)) >> four_kb_shift;                          // addy and VIDEO holds 20 bits 
                vid_p_tab[i].addy =  0xB8; 
            
            
            }

            else{
                vid_p_tab[i].present_bit = 0;                   // 1: marks page as valid, 0: page range assoc. w/ this PTE doesn't exist 
                vid_p_tab[i].r_w_bit = 1;                       // 1: pages are r/w, 0 : read only, mark all pages r/w 
                vid_p_tab[i].user_sup_bit = 0;                  // if 1, memory mapped by this PTE is user-level (PL 0-3 can access this memory), 0: memory is supervisor only (PL 0-2 can access), 0 for kernel pages
                vid_p_tab[i].pwt_bit = 0;                       // always want writeback, if page being cached, this bit makes the caching write through and not writeback 
                vid_p_tab[i].pcd_bit = 0;                       // set to 0 for non kernel pages, won't cache memory in this PDE's range 
                vid_p_tab[i].accessed_bit = 0;                  // won't use this bit (1 when a process touches memory that this PDE maps/ access is made to a virtual addr that corresponds to this entry)
                vid_p_tab[i].dirty_bit = 0;                     // set to 1 by processor if page has been written to, bit remains 1 until cleared by software, won't use so set to 0
                vid_p_tab[i].pat_bit = 0;                       // page attribute table index, won't use it so set to 0
                vid_p_tab[i].global_bit = 0;                    // only set for kernel pages (virtual→physical translations for this page visible to all processes, TLB entries won't be cleared when CR3 reloaded)
                vid_p_tab[i].available_bits = 0;                // available for our use but won't use 
                vid_p_tab[i].addy = i;                   // index times the amount of bytes in an entry which is 4096
            }
    }


        //load_page_directory((unsigned int*)p_dir);         // loads our page directory and enables paging 
        //*screen_start = (uint8_t*)mb_64;
        flush_TLB(); // flush tlb
     
}

/*
Description: unmaps the 4KB page created in the video_map_func
Input: None
Output: None
Side Effects: sets the present bit of this PDE to 0
*/
void video_UNMAP_func(){
        //int i;
        // 34 -> 136MB (location in virtual memory) / 4MB (size of each pde) = entiry 34
        p_dir[34].KB.present_bit = 0;                                    // All valid PDEs need to have their present bit set to 1
        p_dir[34].KB.r_w_bit = 1;                                        // mark all pages read/write.
        p_dir[34].KB.user_sup_bit = 1;                                   // 1: user privilege level for everything that isn't video (privilege levels 0-3 can access this memory), we run user-level code at pl 3, allows us to mark pages as kernel only
        p_dir[34].KB.pwt_bit = 0;                                        // always want writeback, if page being cached, this bit makes the caching write through and not writeback 
        p_dir[34].KB.pcd_bit = 0;                                        // set to 0 for video memory (because these pages contain mem mapped I/O and shouldn't be cached), won't cache any memory that falls in this PDE's range 
        p_dir[34].KB.accessed_bit = 0;                                   // won't use this bit (1 when a process touches memory that this PDE maps/ access is made to a virtual addr that corresponds to this entry)
        p_dir[34].KB.reserved_bit = 0;                                   // always 0
        p_dir[34].KB.page_size_bit = 0;                                  // 0 for 4kb 
        p_dir[34].KB.global_bit = 0;                                     // only set for kernel pages (virtual→physical translations for this page visible to all processes, TLB entries won't be cleared when CR3 reloaded)
        p_dir[34].KB.available_bits = 0;                                 // available for our use but we're not going to ues these bits 

        p_dir[34].KB.addy = 0;      // addy only holds 20 bits but addr of vid_p_tab is 32 bits
        

        flush_TLB(); // flush tlb

        
    
}




