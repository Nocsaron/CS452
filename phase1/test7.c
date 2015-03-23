/*
  Test case for Semaphores. Invalid sempahore value
  
  should print some message similar to the below one, and ideally should call Halt, thereby preventing operations on an invalid semaphore.
  
  ---------Starting Test 7 ----------
value of a semaphore cannot be negative
  
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

	USLOSS_Console(" \n---------Starting Test 7 ----------\n");
	int childpid = P1_Fork("child", child, NULL, USLOSS_MIN_STACK * 4, 2);
	sem1 = P1_SemCreate(-1); 
	P1_P(sem1);
	USLOSS_Console(" %d \n", 1);
	
	USLOSS_Console(" ---------Ending Test 7 ----------\n");
	P1_Quit(0);
}


