#ifndef FILE_SYS_H
#define FILE_SYS_H
// #include "types.h"
#include "lib.h"
// 4096: amount of bytes in a 4 kilobyte block.
#define FOURK_Block_Storage 4096

typedef struct dentry{ // struct for a dentry 
    int8_t filename[32]; //name of file
    int32_t filetype; // type of file: 0 for RTC, 1 for directory, 2 for file
    int32_t inode_num; // which inode in the array
    int8_t reserved[24]; // it is what it is

}__attribute__ ((packed)) dentistry_t; 

typedef struct bootBlock{  // struct for the single boot block
    int32_t dir_count; // number of entries in the directory
    int32_t inode_count; // number of inodes (N)
    int32_t data_count; // number of data blocks 
    int8_t reserved[52]; // it just is
    dentistry_t  direntries[63]; // array of 63 dentries (64 files - 1 statistics = 63 files)
    
}__attribute__ ((packed)) bootBlock_t;

typedef struct inode{ // struct for inodes 
    int32_t length; // length of file in bytes
    int32_t data_block_num[1023]; //indicates data block index for all possible 1023 data blocks for an inode  (1024 - 1 (length) = 1023)
}__attribute__ ((packed)) inode_t;


int32_t file_init(uint32_t boot_block_addr); // initialize structs for file system 
int32_t file_open(int8_t* filename); // open a given file
int32_t file_close(int32_t fd); // closes the previously opened file
int32_t file_read(int32_t fd,  void* buf, int32_t nbytes); // read the contents of file into buffer 
int32_t file_write(int32_t fd, void* buf, int32_t nbytes); // write into file, do nothing for now 
int32_t directory_write(int32_t fd, void* buf, int32_t nbytes); // creates a new directory, do nothing for now 
int32_t directory_open(int8_t* filename); // open a given directory, do nothing for now 
int32_t directory_close(int32_t fd); // close current directory, do nothing for now 
int32_t directory_read(int32_t fd, void* buf, int32_t nbytes); // writes one file name into a buffer per call to function 
int32_t read_dentry_by_name (int8_t* fname, dentistry_t* dentry); // find file given name 
int32_t read_dentry_by_index (int32_t index, dentistry_t* dentry); // find file given index
int32_t read_dentry_by_index_d (int32_t index, dentistry_t* dentry);
int32_t read_data (int32_t inode, int32_t offset, int8_t* buf, int32_t length); // reads "length" bytes of data from file and write into buffer 



bootBlock_t * boot_block_addr_ptr; //pointer to boot block 
int32_t data_block_start_ptr; // pointer to first data block 
inode_t * inode_start; // pointer to first inode 
// int32_t inode_to_offset[64];  // maps the offst to the inode number.
int32_t dirCounter; // keeps track of current file 
dentistry_t curFile; // keeps track of current dentry file 
//  int32_t eofflag; /* end of file flag- indicates that the entire file wad read */
 int32_t current_fd; 

#endif
