#include "terminal.h"
#include "lib.h"

/*
Description: To be implemented in a later checkpoint 
Input: None
Output: None
Side Effects: None
*/
int open(int8_t* filename){
    return 0; 
}

/*
Description: To be implemented in a later checkpoint 
Input: None
Output: None
Side Effects: None
*/
int close (int32_t fd){
    return 0; 
}


/*
Description: Reads the values that are typed on the screen and copies them into the buffer passed in the function 
Input: int file directory, buffer thats passed, bytes to be read 
Output: Size of the number of bytes that are read 
Side Effects: Copies all the values from the buffer used in keyboard.c to the buffer that is passed here 
*/
int terminal_read(int32_t fd,  void* buf, int32_t nbytes){
    //sti();
    //printf("we out here");
    // printf("Our flag s: %d", terminal_cur_reading_flag);

    // terminal_cur_reading_flag =0;
    cleanse(); 
    char* buffy = (char*) buf;
    int size = 0; 
    int i;

   terminal_cur_reading_flag =0;

    if (sys_flag){
        for (i = 0; i < nbytes; i++){
            buffy[i] = '\0';
        }
    }
    
  
    // if(terminal_cur_reading_flag == 0){
        
        sti();
        while(multi_term[next_index].enter_flag == 0); // jumps out of this while loop only unitl when enter flag is pressed 
        cli(); // disabling all interrupts 
        //printf("at cli");
        // while(terminal_cur_reading_flag == 1){
        //     size = 0;
        // }
        for  (i=0 ; i < nbytes; i++){
            if(terminal_buffet[i] != '\0'){ // only passes through if the valuues in the buffer are not 0 
                buffy[i] = terminal_buffet[i]; // copies all the values from buffer implameneted in keyboard.c 
                // putc(buf[i]);
                size+=1; //incrementing size if value is successully read 
                //printf("hi");
            }
            else{
                break; 
            }
        }

   sti();// ENABLING all interrupts
    multi_term[next_index].enter_flag = 0;
   //printf("about to leave"); 
   //buf = buffy;
    return size; // returns size of bytes that is successfullyl read 
    
}

/*
Description: Prints all the values that are typed into the read buffer  
Input: int file directory, buffer thats passed, bytes to be written
Output: Outpts 0 
Side Effects: Prints the characters on the screen 
*/
int terminal_write(int32_t fd, void* buf, int32_t nbytes){
  //printf(buf);
   
    // terminal_cur_reading_flag = 0;
    // cli();
    char* buffy = (char*) buf;
    int j;
//     while(terminal_cur_reading_flag==1){
//    };
    cli(); // disabling all interrupts 
    // if(terminal_cur_reading_flag){
    //     // terminal_cur_reading_flag = 0;
    //      printf("i made it here- write!!");
    //         return -1;
    // }
    for (j= 0 ; j< nbytes; j++){ // iterates through the entire length of the buffer (including '\n')
       if(buffy[j]!= '\0'){ // only passes through if the valuues in the buffer are not 0 
            putc(buffy[j]); // prints all the values from buffer that was passed in the function 
         }
          
    }
    
    if(sys_flag == 0){
        // putc('\n'); // printing a new line 
    }



    sys_flag = 0; // setiing flags back to 0
    
    // multi_term[active_terminal_id].enter_flag = 0; 
    sti(); // enabling all interrupts 
    return 0; 

}
