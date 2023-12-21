#include "file_sys.h"
#include "sys_calls_diff.h"
/*

Description: Initializes the structs and sets the address for the file system
Input: address of file system location
Output: none
Retrun Value: none
Side Effect: creates three pointers and initializes the file

*/
int32_t file_init(uint32_t boot_block_addr){
    //initialize a bootblock pointer and inode pointer 
    boot_block_addr_ptr = (bootBlock_t *)boot_block_addr; // pointer to boot block entry
    inode_start  =  (inode_t *)((bootBlock_t *)boot_block_addr + 1); // pointer to first inode. plus 1 bc already adding bytes
    data_block_start_ptr = (int32_t)inode_start + (boot_block_addr_ptr->inode_count)*FOURK_Block_Storage; // pointer to start of data block 

   
    dirCounter = 0; // keeps track of what directories have been printed
    //curFile = &curFile;
    //for()
    return 0; 


}


/*

Description: opens requested file 
Input: file to open
Output: none
Retrun Value: 0 if success and -1 if not found or null parameter
Side Effect: the global dentry curfile is initialized to the current open file

*/

int32_t file_open(int8_t* filename){

    if(filename == NULL || strlen(filename) > 32){ // if the parameter is null or name of file is too large, return -1 (error)
        return -1;
    }

    if(read_dentry_by_name(filename, &curFile) == 0){ // sets the currentfile to be the filename's file
        return 0;
    }
    else{
        return -1;  // if the file is not found, return -1 (error)
    }

    

}


/*

Description: file to write into
Input: 
    fd: file descriptor
    buf: buffer - store result
    nbytes: number of bytes to write
Output: none
Retrun Value: none
Side Effect: edits the file with the amount of space given by the buffer

*/
int32_t file_write(int32_t fd,  void* buf, int32_t nbytes){
    return -1;
}

/*

Description: Closes the previously open file
Input: None
Output: None
Retrun Value: None
Side Effect: None

*/
int32_t file_close(int32_t fd){
    //curFile = NULL; 
    return 0; 
}

/*

Description: Reads the file contents - User function
Input: 
    fd: file descriptor
    buf: empty buffer that will hold file contents
    nbytes: number of bytes to read
Output: None
Retrun Value: returns the number of bytes read
Side Effect: edits the buffer to conatin the file contents.

*/
int32_t file_read(int32_t fd,  void* buf, int32_t nbytes){
    
    current_fd = fd; 
  
    // read data reads the file  based on the inode number which is provided from the global file variable
    inode_t * currentInode =  (inode_t*)(active_PCB->fd_table[fd].inode + inode_start);
    read_dentry_by_index(active_PCB->fd_table[fd].inode , &curFile);
    
    if((active_PCB->fd_table[fd].file_position + nbytes > currentInode->length ) ){
       nbytes = currentInode->length - active_PCB->fd_table[fd].file_position;
      
    } 

    if(active_PCB->fd_table[fd].file_position >= currentInode->length){
        //while(1);
        //printf("file pos = %d", active_PCB->fd_table[fd].file_position);
        active_PCB->fd_table[fd].file_position =  currentInode->length - 1; 
        // inode_to_offset[(curFile.inode_num)] = currentInode->length - 1; 
        // printf("about to return 0 from file_read");
        // printf
        return 0; 
    } 

   
    int32_t ret_val = read_data((curFile.inode_num),active_PCB->fd_table[fd].file_position , buf, nbytes);
    // printf("inode to off %d ",inode_to_offset[(curFile.inode_num)]);
    // printf("filename %s ", curFile.filename);
    return ret_val;
  
}

/*

Description: Finds a file by name
Input: 
    fname: desired file's File name
    dentry: dentistry struct that is to be initialized with the file's metadata
Output: None
Retrun Value: 
Side Effect: initializes gloabal file variable to the filename's file

*/
int32_t read_dentry_by_name ( int8_t* fname, dentistry_t* dentry){
    int i;
    //dentistry* diren_ptr = ((boot_block_addr_ptr->direntries[i]));
    for(i = 0; i< boot_block_addr_ptr-> dir_count; i++){ // iterates through all the directories
        // tries to find the file based on file name. This compares the existing files with the given file name
        // 32: A file name has 32 bytes, so we want to read 32 bytes.
        if((strncmp((int8_t*)(fname),(int8_t*) boot_block_addr_ptr->direntries[i].filename, 32)) == 0){
            // copies the data of the requested file into dentry
            // 64: one directory entry is 64 Bytes, thus we want to write 64 bytes into dentry.
            memcpy(dentry,(&(boot_block_addr_ptr->direntries[i])),64); 
            //dentry = boot_block_addr_ptr->direntries[i];
            return 0; // file exists
        }
    }
    return -1; // invalid file
}

/*

Description: Finds a file by index
Input: 
    index: desired file's index
    dentry: dentistry struct that is to be initialized with the file's metadata
Output: None
Retrun Value: 
Side Effect: initializes gloabal file variable to the index's file

*/
int32_t read_dentry_by_index (int32_t index, dentistry_t* dentry){
    int i; //Shreya is happy now
    //dentistry* diren_ptr = ((boot_block_addr_ptr->direntries[index]));

    //63: max number of files + 1
    if(index < 0 && index >=  boot_block_addr_ptr-> dir_count){  // checks if the index passed in is a valid one
       
        return -1; // invalid index
    }
    //memcpy(((buf + bytesWritten)), &(boot_block_addr_ptr->direntries[i].filename),32);
 
    for(i = 0; i< boot_block_addr_ptr-> dir_count; i++){ // iterates through the directories
        // tries to find the file based on file name. This compares the existing files with the given file name
        // 32: A file name has 32 bytes, so we want to read 32 bytes.
        if(boot_block_addr_ptr->direntries[i].inode_num == index){
            //dentry = boot_block_addr_ptr->direntries[i];
            // copies the data of the requested file into dentry
            // 64: one directory entry is 64 Bytes, thus we want to write 64 bytes into dentry.
            memcpy(dentry, (&(boot_block_addr_ptr->direntries[i])),64);
            return 0; // file exists
        }
    }
   
    return -1; // invalid file
}

// Description: Finds a file by index
// Input: 
//     index: desired file's index
//     dentry: dentistry struct that is to be initialized with the file's metadata
// Output: None
// Retrun Value: 
// Side Effect: initializes gloabal file variable to the index's file

int32_t read_dentry_by_index_d (int32_t index, dentistry_t* dentry){
    int i; //Shreya is happy now
    //dentistry* diren_ptr = ((boot_block_addr_ptr->direntries[index]));

    //63: max number of files + 1
    if(index < 0 && index >=  boot_block_addr_ptr-> dir_count){  // checks if the index passed in is a valid one
       
        return -1; // invalid index
    }
    //memcpy(((buf + bytesWritten)), &(boot_block_addr_ptr->direntries[i].filename),32);
 
 for(i = 0; i< boot_block_addr_ptr-> dir_count; i++){ // iterates through all the directories
        // tries to find the file based on file name. This compares the existing files with the given file name
        // 32: A file name has 32 bytes, so we want to read 32 bytes.
        if((strncmp((int8_t*) boot_block_addr_ptr->direntries[index].filename,(int8_t*) boot_block_addr_ptr->direntries[i].filename, 32)) == 0){
            // copies the data of the requested file into dentry
            // 64: one directory entry is 64 Bytes, thus we want to write 64 bytes into dentry.
            memcpy(dentry,(&(boot_block_addr_ptr->direntries[i])),64); 
            //dentry = boot_block_addr_ptr->direntries[i];
            return 0; // file exists
        }
    } 
    return -1; // invalid file
} 
/*

Description: Reads the data from a given inode
Input: 
    inode: index of file information
    offset: starting position into file
    buf: will be written into - stores the file contents
    length: bytes to read
Output: none
Retrun Value: bytes read 
Side Effect: reads data from a given file via its inode and returns number of read bytes.

*/
int32_t read_data (int32_t inode, int32_t offset, int8_t* buf, int32_t length){
    int i;
    // printf("Read Data called\n");
    int32_t startingDB; /* starting data block of where to read data from */
    int32_t startingDB_off; /* offset from starting datablock */
    int32_t endingDB; /* last data block to read data from */
    int32_t endingDB_off; /* offset from last data block to read from */
    int32_t bytes_written; /* total number of bytes written to buffer */
    inode_t * curInode;  /* pointer to current inode */
   
    
    if(inode < 0 && inode >= boot_block_addr_ptr -> inode_count){  // checks that the inode is a valid inode
        return -1;
    }
    curInode = (inode_t*)(inode + inode_start); /* pointer to current inode */
   

    bytes_written = 0; // keeps track of the number of bytes written to the buffer - equal to number of bytes read
    startingDB = offset/FOURK_Block_Storage; // block to start at 
    startingDB_off = offset % FOURK_Block_Storage; // offset from start of starting block
    endingDB = (offset + length) / FOURK_Block_Storage; // last DB 
    endingDB_off = (offset + length) % FOURK_Block_Storage; // last DB off
    // eofflag = 0; // flag to check if you have read the entire file

  

    if(offset >= curInode->length){ // return 0 if the file position is at end or beyond end of file
       // offset = 0; 
        active_PCB->fd_table[current_fd].file_position = curInode->length - 1;  
        // inode_to_offset[inode] = curInode->length - 1;
        //  prntf("about to return 0 from read_data");
        return 0; 
    }

    if(offset + length > curInode->length){ /* check if we read to end of file*/
        // inode_to_offset[inode] = curInode->length - 1; /* reset offset to 0 because we're reading the rest of the file*/
        endingDB = curInode->length/FOURK_Block_Storage; /*data block for end of file*/
        endingDB_off = curInode->length % FOURK_Block_Storage; /*offset for data block for end of file*/
        
    }
    
    else{
        // inode_to_offset[inode] = offset + length; /*save current position */
        active_PCB->fd_table[current_fd].file_position = offset + length;
    }
    // printf("file position = %d offset = %d ", active_PCB->fd_table[current_fd].file_position, offset);
//BYTES WRITTEN IS HOW MANY CHARACTERS YOU HAVE ALREADY READ PREV 
    for (i = startingDB; i <= endingDB; i++){ /*iterate through all blocks to read from*/
        if(startingDB == endingDB){
            memcpy(buf + bytes_written, ((uint8_t *)data_block_start_ptr + (FOURK_Block_Storage)*(curInode->data_block_num[i]) + offset%4096), length); // write from start of endingDB to last byte
            return (length); // number of bytes read 
        }
        if(i == endingDB){ /*check if we're at the final data block*/
            memcpy(buf + bytes_written, (uint8_t *)data_block_start_ptr + (FOURK_Block_Storage)*(curInode->data_block_num[i]), endingDB_off); // write from start of endingDB to last byte
           return (endingDB_off + bytes_written); // number of bytes read 
        }

        else if (i == startingDB){
            memcpy(buf , ((uint8_t *)data_block_start_ptr + (FOURK_Block_Storage)*(curInode->data_block_num[i]) + startingDB_off), FOURK_Block_Storage-startingDB_off); // write from start to end of starting DB
            bytes_written += FOURK_Block_Storage-startingDB_off; // increment num of bytes written 
        }

        else{
            memcpy(buf + bytes_written, (uint8_t *)data_block_start_ptr + (FOURK_Block_Storage)*(curInode->data_block_num[i]) , FOURK_Block_Storage); // write the entirety of a block
            bytes_written += FOURK_Block_Storage; // increment num of bytes written 
        }

    }
    
   return 0;
   
    

}


/*

Description: creates a new directory
Input: 
    fd: file descriptor
    buf: empty buffer that will hold file contents
    nbytes: number of bytes to write
Output: None
Retrun Value: none
Side Effect: none

*/
int32_t directory_write(int32_t fd,   void* buf, int32_t nbytes){
    return -1;
}


/*

Description: opens the requested directory
Input: 
    filename: directory to open
Output: none
Retrun Value: none
Side Effect: none

*/
int32_t directory_open(int8_t* filename){ 
    
    return 0;
}

/*

Description: closes the current directory
Input: none
Output: none
Retrun Value: none
Side Effect: none

*/
int32_t directory_close(int32_t fd){ // dont need to do anything
    return 0;
}

/*

Description: writes one file name into a buffer per call to function 
Input: 
    fd: file descriptor
    buf: empty buffer that will hold file contents
    nbytes: number of bytes to read
Output: None
Retrun Value: returns the number of bytes read
Side Effect: edits the buffer to conatin the file contents.
*/
int32_t directory_read(int32_t fd,   void* buf, int32_t nbytes){
   
    
    //uint32_t numfilenames;
    if (nbytes > 32){ // 32: a filename stores 32 bytes so if more is passed in override to 32. Can not read more than 32 bytes
        nbytes = 32; 
    }
    if(dirCounter >=  boot_block_addr_ptr-> dir_count){
        dirCounter = 0;
        return 0;
    }

    // numfilenames = nbytes/32; // number of files to print

    //dentistry d;

    //check that the directory exists.
    if((read_dentry_by_index_d(dirCounter,&curFile)) == -1){  
        return -1;
    }
    
    // for(i = dirCounter; i< numfilenames + dirCounter; i++){
        int length = strlen((int8_t*)(curFile.filename));
        // 32: a filename stores 32 bytes so if more is passed in override to 32. Can not read more than 32 bytes
        // checks if the length of the file name is greater than 32, is so only store the first 32 characters
        if(length > 32){
            length = 32;
        }
        //write the file into the buffer
        memcpy(buf, (curFile.filename),(uint32_t)nbytes);     
      
    // }
     // increment that a file was read
    dirCounter += 1;
    // length stores number of bytes read
    return length;
}
