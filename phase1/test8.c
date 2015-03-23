/*
  test case for preemptive scheduling (test if p and v opts are priority aware)
  should output:

---------Starting Test 8 ----------
high start
low start
high finish
low finish
 ---------Ending Test 8 ----------

*/

#include "phase1.h"
#include <stdio.h>

	int childpid1, child_status1, exit_childpid1;
	int childpid2, child_status2, exit_childpid2;
	P1_Semaphore sem;

int high(void *arg){
 	USLOSS_Console("high start\n");
	P1_P(sem);	//will block
	USLOSS_Console("high finish\n");	
	return -1;
}

int low(void *arg){
  	USLOSS_Console("low start\n");
	P1_V(sem);	//will be switched off
	USLOSS_Console("low finish\n");
  	return -2;
}

int P2_Startup(void *arg){
	
	USLOSS_Console(" \n---------Starting Test 8 ----------\n");
	
	sem = P1_SemCreate(0);
	childpid1 = P1_Fork("low", low, NULL, USLOSS_MIN_STACK * 4, 3);
	childpid2 = P1_Fork("high", high, NULL, USLOSS_MIN_STACK * 4, 2);
	exit_childpid1 = P1_Join(&child_status1);	
	exit_childpid2 = P1_Join(&child_status2);
	
	USLOSS_Console(" ---------Ending Test 8 ----------\n");
	return 0;
}
