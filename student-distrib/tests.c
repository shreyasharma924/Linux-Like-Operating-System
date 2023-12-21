
#include "tests.h"

#define PASS 1
#define FAIL 0

/* format these macros as you see fit */
#define TEST_HEADER 	\
	printf("[TEST %s] Running %s at %s:%d\n", __FUNCTION__, __FUNCTION__, __FILE__, __LINE__)
#define TEST_OUTPUT(name, result)	\
	printf("[TEST %s] Result = %s\n", name, (result) ? "PASS" : "FAIL");

static inline void assertion_failure(){
	/* Use exception #15 for assertions, otherwise
	   reserved by Intel */
	asm volatile("int $15");
}


/* Checkpoint 1 tests */

/* IDT Test - Example
 * 
 * Asserts that first 10 IDT entries are not NULL
 * Inputs: None
 * Outputs: PASS/FAIL
 * Side Effects: None
 * Coverage: Load IDT, IDT definition
 * Files: x86_desc.h/S
 */
int idt_test(){
	TEST_HEADER;

	int i;
	int result = PASS;
	for (i = 0; i < 10; ++i){
		if ((idt[i].offset_15_00 == NULL) && 
			(idt[i].offset_31_16 == NULL)){
			assertion_failure();
			result = FAIL;
		}
	}

	return result;
}

/*
description: This tests the divide by error exception 
input: none
output: none
return: returns 0 if working
side effects: Tests if the program detects a divide by zero error and calls the divide by zero exception handler
*/
// int divbyzero() {
// 	TEST_HEADER;
// 	// int i = 3;
// 	// int z = 0
// 	//int g;
// 	//g =  3/0; // 3 - just an integer for testing
// 	return 3/0;
// }

/*
description: This tests the page fault exception
input: none
output: none
return: returns 0 if working
side effects: Test if the program detects a page fault exception
*/
// int page_fault_exception(){
// 	TEST_HEADER; 
// 	int result = PASS;
// 	//int b = 0xC0001; 
// 	//printc("%d", &b);
	 
// 	// SHOULD FAIL BECAUSE WE ARE POINTING TO A LOCATION THAT DOES NOT EXIST 
// 	int *a  = NULL;
// 	// SHOULD PASS BECAUSE IT IS OUT OF BOUNDS WITH VIDEO MEMORY 
// 	//int *a  = (int*) 0xC0001;

// 	// SHOULD FAIL BECAUSE IT IS IN BOUNDS WITH VIDEO MEMORY 
// 	//int *a = (int*)0xB8001;
// 	if (*a != 0){
// 		result = FAIL;
// 		printf("Address: %x        ", (int) a);
// 		return result;
		
// 	}
// 	result = PASS;
// 	printf("Address: %x        ", (int) a);
// 	return result;
	
// }

/* Checkpoint 2 tests */ 

/*
description: This tests the directory read function
input: none
output: prints the file name, file type, and length of file for each file 
return: always returns 0 
side effects: Tests if the program detects a divide by zero error and calls the divide by zero exception handler
*/
int ls(){

	uint8_t buf[7000]; //buffer to write into 
	int length_of_file; /*holds the length of given file*/
	int i; /*iterator for first loop */
	int len; /*holds file name length */
	int j; /*iterator for second for loop*/
	inode_t* cur_inode; /*inode pointer for the current file*/
	int file_type; /* holds the file type: 0, 1, or 2*/

	for(i = 0; i< 17; i++){ /* iterate 16 types, 1 per given file*/
		directory_read(10, buf, 32); /* call directory read and write name of one file into buffer*/

		cur_inode = (inode_t*)(curFile.inode_num + inode_start); /*holds start of current inode*/
		length_of_file = cur_inode->length; /*holds length of given file */
		file_type = curFile.filetype; /*holds file type of given file */
		
		printf("filename: %s",buf); /*print one file name*/

	 	len = strlen((int8_t*)(curFile.filename)); /*hold length of a given file name*/
		if (len > 32){ /*when length of file name is greater than 32 characters, just write out 32 characters*/
			len = 32; 
		} 
		for (j = 0; j < 32 - len + 3; j++){ /* to space out the file names in a way that looks nice/aligns them*/
			printf(" ");
		}
		printf("file_type: %d, file_size: %d\n", file_type, length_of_file); /*print out file type and file size for a single file*/

	}


	return 0;
}



/*
description: This tests the file_read function
input: none
output: writes out all the characters in a file and prints out the file name 
return: always return 0
side effects: Tests if the program detects a divide by zero error and calls the divide by zero exception handler
*/
int readFile_test(){
	
	uint8_t file_name[] = "cat"; /* CHANGE: change to file that you want to read*/
	//int file_name_length = strlen((int8_t*)file_name); /*holds length of file name*/
	//int i; /* iterator for for loop */
	int num_bytes; /*holds number of bytes that were written into buffer*/
	//int offset = 0; /*indicates where to start writing into the buffer*/
	//int j = 0; /*keeps track of index for non_null_buff*/
	inode_t* cur_inode; /*inode pointer for the current file*/
	int length_of_file; /*holds the length of given file*/
	
	if(file_open((int8_t*)file_name) == 0){ /* checks if file name exists*/
		cur_inode = (inode_t*)(curFile.inode_num + inode_start); /*holds start of current inode*/
		length_of_file = cur_inode->length; /*holds length of given file */
		char buf[length_of_file]; /*buffer that will hold text in a file*/ 
		num_bytes = file_read(0, buf, length_of_file); /*writes into buffer and returns number of bytes written*/
		
		//printf("numbytes : %d", num_bytes);
		//char no_null_buff[num_bytes]; /*holds characters in file that are not null */
		// file_flag = 1; 
		print_flag = 1; 
		// for (i = 0; i < num_bytes; i++){
		// 	putc(buf[i]);
		// }
		//putc(buf[8]);
		terminal_write(0,buf,num_bytes);
		
		// if(file_name[file_name_length -1] == 't' && file_name[file_name_length -2] == 'x' && file_name[file_name_length -3] == 't'){ /* checks if file is a txt file */
		// 	for(i = 0; i <= num_bytes/128; i++){ /*iterates through calls to terminal_write (buffer holds 128 characters)*/
		// 		file_flag = 1; /*tells terminal write that we want to print out a file*/
		// 		terminal_write(0,buf + offset,0); /*writes from buffer starting from offset position to terminal*/
		// 		offset+= 128; /*increment offset by 128 (number of characters buffer can hold at once)*/
				
		// 	}
		// }
		// else{ /*if file is executable*/
		// 	print_flag = 1; // tells putc that we are currently printing control characters and the function of them should be ignored
		// 	for(i =0; i< 4; i++){
		// 		putc(buf[i]);
		// 		}
		// 	for(i =0; i< 10; i++){
		// 		putc('\n');
		// 		}
		// 	for(i = num_bytes - 37; i < num_bytes; i++){
		// 		putc(buf[i]);
		// 		}

			// for (i = 0; i < num_bytes; i++){ /*iterates through all charcters in buf*/
			// 	if(buf[i] != '\0'){ /*checks if current char in buffer is a null character*/
			// 		no_null_buff[j] = buf[i]; /*puts character into no_null_buff if character isn't null*/
			// 		j++; /*increment index*/
			// 	}
				
			// }
			// for(i = 0; i <= num_bytes/128; i++){ /*iterates through every set of 128 characters in buffer (buffer can only hold 128 charactesr)*/
			// 	file_flag = 1; /*tells terminal write that we want to print out a file*/
			// 	terminal_write(0,no_null_buff + offset,0); /*writes from buffer starting from offset position to terminal*/
			// 	offset+= 128;  /*increment offset by 128 (number of characters buffer can hold at once)*/
				
			// }
			
		
		
		print_flag = 0;
		
	printf("\nfile_name: %s", file_name); /*prints out file name at the end*/
	// printf("2nd Last Character: %d", no_null_buff[j] );
	// printf("Last Character: %d", no_null_buff[j -1] );
	// for(i =0; i< num_bytes; i++){
	// 	putc(buf[i]);
	// }
	

	}
	else{
		printf("nonexistent file"); /*if file doesn't exist, print out error message */
	}
	

	return 0;
	
}

int test_terminal(){
	char buf[128];
	while(1){
		// printf("here");
		// printf(" file_flag: %d", file_flag);
		int read = terminal_read(0, buf, 128);
		// printf("came out of read ");
		// printf(buf);
		// printf("num bytes: %d", read);
		terminal_write(0, buf, read); 
	}
	return PASS; 
}


// int test_terminal(){
// 	char buf[1000];
// 	int j = 0; 
// 	while(1){
// 		memset(buf, '\0', 128);
// 		buf[0] = 'W';
// 		buf[1] = 'a';
// 		buf[2] = 'i';
// 		buf[3] = 'd';
// 		buf[4] = 'a';
// 		buf[5] = 't';
// 		buf[6] = ' ';
// 		uint32_t tmp = terminal_write(1, buf, 7);
// 		uint32_t last_index = terminal_write(1, buf, 128);

// 		if(last_index >0){
// 			uint32_t c = terminal_write(1, buf, last_index);
// 		}
// 		if(last_index != 0){
// 			putc('\n');
// 		}

// 	}
// }
// OUR RTC TEST CASE
int rtc_test_cp2(){
	int32_t b[1];
	int32_t i;
	int j;
	open_rtc(0);
	for (i=2; i<= 1024; i++){
		b[0]=i;
		write_rtc(0, b, 4);
		for (j=0; j < 5; j++){
			read_rtc(0, b, 4);
			putc('d');
		}
		putc('\n'); 
	}
	return 0;
}
int rtc_test2_cp2(){
	int ret_val;
	int garbage;
    int rtc_fd;
	rtc_fd = open_buttercup_handler((uint8_t*)"rtc");
	ret_val = 32;
	ret_val = write_Utonium_handler(rtc_fd, &ret_val, 4);
	read_rtc(rtc_fd, &garbage, 4);
	return 0;
}


/* Checkpoint 3 tests */
/* Checkpoint 4 tests */
/* Checkpoint 5 tests */
                 




/* Test suite entry point */
void launch_tests(){
	//TEST_OUTPUT("idt_test", divbyzero());
	//test_terminal();
	
	// active_PCB_pid = -1; 
	clear();
	set_cursor(0,0);
	//printf("out of execute");
	//printf("whatever");
	// set_cursor(0,0);
	//update_cursor();
	//ls();
	//rtc_test_cp2();
	//readFile_test();
	//TEST_OUTPUT("idt_test", divbyzero());
	//TEST_OUTPUT("page_fault_exception", page_fault_exception());
	// launch your tests here
	
	
	
}
