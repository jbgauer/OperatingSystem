#include "tests.h"
#include "x86_desc.h"
#include "lib.h"

#include "rtc.h"
#include "terminal.h"
#include "keyboard.h"
#include "filesys_driver.h"

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

#define NUM_COLS    80
#define NUM_ROWS    25

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

/* divideByZero_test
 * 
 * Tests divide by zero exception
 * Inputs: None
 * Outputs: PASS/FAIL
 * Side Effects: None
 * Coverage: divide by zero exception
 * Files: idt.c/h
 */
void divideByZero_test(){
	int i;
	int j = 0;
	i = 1/j;
}

/* bound_range_exceeded_test
 * 
 * Tests bound range exception
 * Inputs: None
 * Outputs: PASS/FAIL
 * Side Effects: None
 * Coverage: bound range exception
 * Files: handlerWrap.S/h
 */
void bound_range_exceeded_test(){
	// unsigned int i[1];
	// i[0] = 5;
	// i[1] = 2;
	asm("INT $0x05");
}

/* syscall_test
 * 
 * Tests system call is loaded in idt
 * Inputs: None
 * Outputs: PASS/FAIL
 * Side Effects: None
 * Coverage: system calls in idt
 * Files: idt.c/h
 */
void syscall_test(){
	asm("INT $0x80");                                  
}

/* rtc_test
 * 
 * Tests that rtc interrupt happens
 * Inputs: None
 * Outputs: PASS/FAIL
 * Side Effects: None
 * Coverage: rtc device interrupts
 * Files: idt.c/h i8259.c/h
 */
void rtc_test(){
	asm("INT $0x28");
}

/* deref_test_zero
 * 
 * Tests memory location 0 is null
 * Inputs: None
 * Outputs: PASS/FAIL
 * Side Effects: None
 * Coverage: memory location 0
 * Files: paging.c/h
 */
void deref_test_zero() {
	unsigned* a;
	unsigned b;
	a = (unsigned*)0;
	b = *a;
	printf("%x", b);
}

/* deref_test_kernel
 * 
 * Tests memory location at start of kernel isn't null
 * Inputs: None
 * Outputs: PASS/FAIL
 * Side Effects: None
 * Coverage: memory location 0x00400000
 * Files: paging.c/h
 */
void deref_test_kernel() {
	unsigned* a;
	unsigned b;
	a = (unsigned*)0x00400000;
	b = *a;
	printf("%x", b);
}

/* deref_test_after_kernel
 * 
 * Tests memory location right after kernel is null
 * Inputs: None
 * Outputs: PASS/FAIL
 * Side Effects: None
 * Coverage: memory location 0x00800000
 * Files: paging.c/h
 */
void deref_test_after_kernel() {
	unsigned* a;
	unsigned b;
	a = (unsigned*)0x00800000;
	b = *a;
	printf("%x", b);
}

/* deref_test_vid_mem
 * 
 * Tests memory location at start of video memory isn't null
 * Inputs: None
 * Outputs: PASS/FAIL
 * Side Effects: None
 * Coverage: memory location 0xB8000
 * Files: paging.c/h
 */
void deref_test_vid_mem() {
	unsigned* a;
	unsigned b;
	a = (unsigned*)0xB8000;
	b = *a;
	printf("%x", b);
}


// add more tests here

/* Checkpoint 2 tests */

/* rtc_open_test
 * 
 * Test for rtc_open
 * Inputs: None
 * Outputs: None
 * Side Effects: None
 * Coverage: rtc device interrupts
 * Files: rtc.c
 */
void rtc_open_test(){
	rtc_open(NULL);
	clear(); screen_x = 0; screen_y = 0; update_cursor(); //clear screen

	while(1){
		rtc_read(NULL, NULL, NULL);
		putc('1');
	}
}

/* rtc_driver_test
 * 
 * Test for rtc read and write
 * Inputs: None
 * Outputs: None
 * Side Effects: None
 * Coverage: rtc device functions
 * Files: rtc.c
 */
void rtc_driver_test(){
	
	rtc_open(NULL);
	int i,j;
	
	while(1){
		for(i=2; i<=1024; i*=2){	//include frequencies up to the max frequency
			clear(); screen_x = 0; screen_y = 0; update_cursor(); //clear screen
			printf("\nfrequency: %dHz\n", i);
			
			if(rtc_write(NULL, (void*)&i, 4)==-1) return;

			for(j=0; j<i; j++){
				rtc_read(NULL, NULL, NULL);
				putc('1');
			}
		}
	}
	rtc_close(NULL);
}

/* rtc_virt_test
 * 
 * Test for rtc virtual read and write
 * Inputs: None
 * Outputs: None
 * Side Effects: None
 * Coverage: rtc device functions
 * Files: rtc.c
 */
void rtc_virt_test(int freq){
	
	rtc_virt_open(NULL);

	clear(); screen_x = 0; screen_y = 0; update_cursor(); //clear screen
	
	rtc_virt_write(NULL, (void*)&freq, 4);

	while(1){
		rtc_virt_read(NULL, NULL, NULL);
		putc('1');
	}
	rtc_virt_close(NULL);
}


/* term_read_test
 * 
 * Test for terminal read 
 * Inputs: None
 * Outputs: None
 * Side Effects: None
 * Coverage: rtc device functions
 * Files: term.c
 */
// void term_read_test() {
// 	int i;
// 	int hold = 0;
// 	char buf[128];
// 	term_open(NULL);
// 	for(i = 0; i < 128; i++) {
// 		hold = term_read(NULL, buf, i);
// 		printf("i = %d ", i);
// 		if(hold == -1) {
// 			printf("Error -1");
// 		} else {
// 			printf("User buff: %s", buf);
// 		}
		
// 	}
// 	term_close(NULL);
// }

void term_test() {
	int hold = 0;
	itn wrote = 0;
	int i;
	char buf[128];
	for(i = 0; i < strlen(buf); i++) {
		buf[i] = 0;
	}
	//nbytes = 0 Check
	term_open(NULL);
	hold = term_read(NULL, buf, 0);
	if(hold == -1) {
		printf("Error nybtes = 0\n");
	} else {
		printf("User buf: ");
		term_write(NULL, buf, 0);
	}
	char buf1[129];
	for(i = 0; i < strlen(buf1); i++) {
		buf1[i] = 0;
	}
	//NULL buf check
	hold = term_read(NULL, NULL, 127);
	if(hold == -1) {
		printf("Error NULL buf\n");
	} else {
		printf("User buf: ");
		wrote = term_write(NULL, buf1, 129);
		printf("Bytes written: %d", wrote);
		printf("Bytes read: %d", hold);
	}
	char buf3[128];
	for(i = 0; i < strlen(buf3); i++) {
		buf3[i] = 0;
	}
	//Full 127 check
	hold = term_read(NULL, buf3, 127);
	if(hold == -1) {
		printf("Error -1");
	} else {
		printf("User buf: ");
		wrote = term_write(NULL, buf3, 128);
		printf("Bytes written: %d", wrote);
		printf("Bytes read: %d", hold);
	}

	char buf2[128];
	for(i = 0; i < strlen(buf2); i++) {
		buf2[i] = 0;
	}
	//7 Char test
	hold = term_read(NULL, buf2, 7);
	if(hold == -1) {
		printf("Error -1");
	} else {
		printf("User buf: ");
		wrote = term_write(NULL, buf2, 8);
		printf("Bytes written: %d", wrote);
		printf("Bytes read: %d", hold);
	}
	char buf4[500];
	for(i = 0; i < strlen(buf4); i++) {
		buf4[i] = 0;
	}
	//nbytes > 128 check
	hold = term_read(NULL, buf4, 435);
	if(hold == -1) {
		printf("Error -1");
	} else {
		printf("User buf: ");
		wrote = term_write(NULL, buf4, 129);
		printf("Bytes written: %d", wrote);
		printf("Bytes read: %d", hold);
	
	}

 	term_close(NULL);
}

/* term_write_test
 * 
 * Test for terminal read 
 * Inputs: None
 * Outputs: None
 * Side Effects: None
 * Coverage: rtc device functions
 * Files: term.c
 */
void term_write_test() {
	int i;
	char* buf = "Testing";
	for(i = 0; i < strlen(buf)+3; i++) {
		printf("bytes = %d. Write: ", i);
		term_write(NULL, buf, i);
		printf("\n");
	}
}

/* file_test
 * 
 * Test filesystem
 * Inputs: None
 * Outputs: None
 * Side Effects: None
 * Coverage: file/dir open, close, and read
 * Files: filesys_driver.c
 */
void file_test(){
	//my system is wonky because no PCB
	//fd is index in file array
	//use fd as the way of keeping track which file to do stuff to
	uint8_t file0[32] = "frame0.txt";
	int fd0 = 0;
	int len0 = 23;//strlen(file0);
	uint8_t buf0[len0];
	int result;
	int i;

	open_file(file0, fd0);
	result = read_file(fd0, buf0, len0);

	//check byte read
	printf("actual len: %d\n", len0);
	printf("bytes read: %d\n", result);

	printf("file:\n");
	for(i=0; i < len0; i++){
		putc(buf0[i]);
	}
	printf("\nfile done");

	close_file(fd0);
}

/* Checkpoint 3 tests */
/* Checkpoint 4 tests */
/* Checkpoint 5 tests */


/* Test suite entry point */
void launch_tests(){
	// CHECKPOINT 1:
	//TEST_OUTPUT("idt_test", idt_test());
	// launch your tests here
	// divideByZero_test();
	// bound_range_exceeded_test();
	// syscall_test();
	// rtc_test();
	// deref_test_zero();
	// deref_test_vid_mem();
	// deref_test_kernel();
	// deref_test_after_kernel();
	
	// CHECKPOINT 2: //
	// rtc_open_test();
	// rtc_driver_test();
	// rtc_virt_test(10);
	//term_test();
	//term_write_test();
	//file_test();
}
