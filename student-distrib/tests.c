#include "tests.h"
#include "x86_desc.h"
#include "lib.h"

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
/* Checkpoint 3 tests */
/* Checkpoint 4 tests */
/* Checkpoint 5 tests */


/* Test suite entry point */
void launch_tests(){
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
}
