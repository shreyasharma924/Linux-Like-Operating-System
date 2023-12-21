#ifndef PAGING_HELPERS_H
#define PAGING_HELPERS_H

#include "types.h"
#include "scheduling.h"
#include "sys_calls_diff.h"


// Loads the page directory
extern void load_page_directory(unsigned int * a);
extern void flush_TLB();

void page_init();                       // initializes paging     
void map_unmap(uint32_t p_id);          // maps to user program
void video_map_func();                  // maps to vid mem
void video_UNMAP_func();                // unmaps vid mem

// created struct for 4KB PDE
// according to descripters.dvi
 typedef struct  page_directory_entry_KB_t {
    uint16_t present_bit    : 1;
    uint16_t r_w_bit        : 1;
    uint8_t  user_sup_bit   : 1;
    uint32_t pwt_bit        : 1;
    uint32_t pcd_bit        : 1;
    uint32_t accessed_bit   : 1;
    uint32_t reserved_bit   : 1;
    uint32_t page_size_bit  : 1;
    uint32_t global_bit     : 1;
    uint32_t available_bits : 3;
    uint32_t addy           : 20;
}page_directory_entry_KB_t ;

// created struct for 4MB PDE
// according to descripters.dvi
 typedef struct page_directory_entry_MB_t{
    uint16_t present_bit    : 1;
    uint16_t r_w_bit        : 1;
    uint8_t  user_sup_bit   : 1;
    uint32_t pwt_bit        : 1;
    uint32_t pcd_bit        : 1;
    uint32_t accessed_bit   : 1;
    uint32_t dirty_bit      : 1;
    uint32_t page_size_bit  : 1;
    uint32_t global_bit     : 1;
    uint32_t available_bits : 3;
    uint32_t pat_bit        : 1;
    uint32_t reserved_bits  : 9;
    uint32_t addy           : 10;
} page_directory_entry_MB_t;

// created struct for page table
// according to descripters.dvi
 typedef struct page_table_entry_t{
    uint16_t present_bit    : 1;
    uint16_t r_w_bit        : 1;
    uint8_t  user_sup_bit   : 1;
    uint32_t pwt_bit        : 1;
    uint32_t pcd_bit        : 1;
    uint32_t accessed_bit   : 1;
    uint32_t dirty_bit      : 1;
    uint32_t pat_bit        : 1;
    uint32_t global_bit     : 1;
    uint32_t available_bits : 3;
    uint32_t addy           : 20;

} page_table_entry_t;


// created union for both types of page directory entries (for both types of pages)
typedef union  union_KB_MB{
     page_directory_entry_KB_t KB;
     page_directory_entry_MB_t MB;
} union_KB_MB;



union_KB_MB p_dir[1024]  __attribute__ ((aligned(4096)));               // PD Array

page_table_entry_t p_tab[1024] __attribute__ ((aligned(4096)));         // PT Array

page_table_entry_t vid_p_tab[1024] __attribute__ ((aligned(4096)));     // PT Array for video memory

page_table_entry_t term_p_table[1024] __attribute__ ((aligned(4096)));     // PT Array for terminal memory




#endif /* _PAGING_H */

