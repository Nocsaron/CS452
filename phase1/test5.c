/*
  Test case for Semaphores. Ensures that a semaphore initialized to 3, blocks - only after allowing 3 P1_P calls without blocking. 
  
  should print :
  
  ---------Starting Test 5 ----------
 0 
 1 
 ---------Ending Test 5 ----------
  
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

	USLOSS_Console(" \n---------Starting Test 5 ----------\n");
	int childpid = P1_Fork("child", child, NULL, USLOSS_MIN_STACK * 4, 2);
	sem1 = P1_SemCreate(3); 
	P1_P(sem1);
	P1_P(sem1);
	P1_P(sem1);
	P1_P(sem1);
	USLOSS_Console(" %d \n", 1);
	
	USLOSS_Console(" ---------Ending Test 5 ----------\n");
	P1_Quit(0);
}


