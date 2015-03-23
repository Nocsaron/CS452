/* 
   P2_Startup forks off 2 processes, they try to kill each other, one will get killed.
   The killed one should finish first and return status 666, other than 111
   This is intend to test the  functions found in milestone submission
   
   Output: (PIDs could differ based on implementation)
   
   ---------Starting Test 1 ----------
killer1 pid: 2 
killer2 pid: 3 
killer1 starts
child finish first: 3	Status: 666  (should be 666)
child finish second: 2	Status: 111  (should be 111)
 ---------Ending Test 1 ----------
   
 */

#include <stdio.h>
#include "phase1.h"

int childpid1, childpid2;

int killer1(void *arg) {
	USLOSS_Console("killer1 starts\n");
	P1_Kill(childpid2, 666);
	return 111;
}

int killer2(void *arg) {
	USLOSS_Console("killer2 starts\n");
	P1_Kill(childpid1, 666);
	return 111;
}

int P2_Startup(void *arg) {

  USLOSS_Console(" \n---------Starting Test 1 ----------\n");
  int exit_status1, exit_status2;
  int exit_childpid1, exit_childpid2;

  //USLOSS_Console("This is P2_Startup \n");

  childpid1 = P1_Fork("killer1", killer1, NULL, USLOSS_MIN_STACK * 4, 2);
  USLOSS_Console("killer1 pid: %d \n", childpid1);		

  childpid2 = P1_Fork("killer2", killer2, NULL, USLOSS_MIN_STACK * 4 * 4, 3);
  USLOSS_Console("killer2 pid: %d \n", childpid2);		

  exit_childpid1 = P1_Join(&exit_status1);
  exit_childpid2 = P1_Join(&exit_status2);

  USLOSS_Console("child finish first: %d\tStatus: %d  (should be 666)\n", exit_childpid1, exit_status1);
  USLOSS_Console("child finish second: %d\tStatus: %d  (should be 111)\n", exit_childpid2, exit_status2);
  
  USLOSS_Console(" ---------Ending Test 1 ----------\n");	
  
  P1_Quit(0);
  return 0;
}
