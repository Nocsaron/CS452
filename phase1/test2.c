/*
  test case for round robin scheduling
  Output should print 0 and 1 in an alternate fashion pretty much similar to the below one
  
  ---------Starting Test 2 ----------
0000000000000000000000111111111111111111111111000000000000000000000000111111111111111111111111100000000
--------Ending Test 2 ----------
*/

#include "phase1.h"
#include <stdio.h>

int child(void *arg){
  int i,j;
  for(j=0; j<200; j++){
    for(i=0;i<2000000;i++){
	    ;
	}
	USLOSS_Console("%d", *(int *)arg);
  }	
  return -1;
}

int P2_Startup(void *arg){

	USLOSS_Console(" \n---------Starting Test 2 ----------\n");
	int childpid1, child_status1, exit_childpid1;
	int childpid2, child_status2, exit_childpid2;

	int p=0, q=1;

	childpid1 = P1_Fork("p", child, &p, USLOSS_MIN_STACK * 4, 2);
  	//USLOSS_Console("Child p created %d \n", childpid1);	

	childpid2 = P1_Fork("q", child, &q, USLOSS_MIN_STACK * 4, 2);
  	//USLOSS_Console("Child q created %d \n", childpid2);

	exit_childpid1 = P1_Join(&child_status1);	
	exit_childpid2 = P1_Join(&child_status2);
	
	USLOSS_Console(" \n---------Ending Test 2 ----------\n");
	P1_Quit(0);
	return 0;
}
