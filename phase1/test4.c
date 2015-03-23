/*
  Test case for basic Semaphores. Ensures that P1_P blocks, then child executes and prints 0, then child unblocks parent and parent prints 1
  
  should print :
  
  ---------Starting Test 4 ----------
 0 
 1 
 ---------Ending Test 4 ----------
  
*/

#include "phase1.h"
#include <stdio.h>

P1_Semaphore sem1; 

int child(void *arg) {
	USLOSS_Console(" %d \n", 0);
	P1_V(sem1);
	P1_Quit(0);
}

int P2_Startup(void *arg){

	USLOSS_Console(" \n---------Starting Test 4 ----------\n");
	int childpid = P1_Fork("child", child, NULL, USLOSS_MIN_STACK * 4, 2);
	sem1 = P1_SemCreate(0); 
	P1_P(sem1);
	USLOSS_Console(" %d \n", 1);
	
	USLOSS_Console(" ---------Ending Test 4 ----------\n");
	P1_Quit(0);
}


