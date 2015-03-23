/*
  test case for preemptive scheduling (simple case, which just see if P1_Fork is priority awared)
  should output:
  
	---------Starting Test 3 ----------
low start
high start
high finish
low finish
 ---------Ending Test 3 ----------
*/

#include "phase1.h"
#include <stdio.h>

	int childpid1, child_status1, exit_childpid1;
	int childpid2, child_status2, exit_childpid2;

int high(void *arg){
 	USLOSS_Console("high start\n");
	USLOSS_Console("high finish\n");	
	return -1;
}

int low(void *arg){
  	USLOSS_Console("low start\n");
	childpid2 = P1_Fork("high", high, NULL,USLOSS_MIN_STACK * 4, 2);
	USLOSS_Console("low finish\n");
	exit_childpid2 = P1_Join(&child_status2);
  	return -1;
}

int P2_Startup(void *arg){
	
	USLOSS_Console(" \n---------Starting Test 3 ----------\n");
	
	childpid1 = P1_Fork("low", low, NULL, USLOSS_MIN_STACK * 4, 3);
	exit_childpid1 = P1_Join(&child_status1);	
	
	USLOSS_Console(" ---------Ending Test 3 ----------\n");
	P1_Quit(0);
	return 0;
}
