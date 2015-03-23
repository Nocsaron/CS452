/* 
   P2_Startup forks off 2 processes. Simply verify that
   run to completion is enabled, and that Join returns
   the status.
   
   Output: (PIDs could differ based on implementation)
   
---------Starting Test 1 ----------
brother starts
brother pid: 2
sister starts
sister pid: 2
child that finished first: 2    Status: 555
child that finished second: 2   Status: 111
 ---------Ending Test 1 ----------
   
 */

#include <stdio.h>
#include "phase1.h"

int brother_pid, sister_pid;
int sister(void *arg) {
	USLOSS_Console("sister starts\n");
    return 111;
}

int brother(void *arg) {
    USLOSS_Console("brother starts\n");
    return 555;
}

int P2_Startup(void *arg) {

  USLOSS_Console(" \n---------Starting Test 1 ----------\n");
  int exit_status1, exit_status2;
  int exit_pid_1, exit_pid_2;

  brother_pid = P1_Fork("brother", brother, NULL, USLOSS_MIN_STACK*2, 3);
  USLOSS_Console("brother pid: %d \n", brother_pid);		

  sister_pid = P1_Fork("sister", sister, NULL, USLOSS_MIN_STACK*2, 1);
  USLOSS_Console("sister pid: %d \n", sister_pid);		   
  
  exit_pid_1 = P1_Join(&exit_status1);
  exit_pid_2 = P1_Join(&exit_status2);
    
  USLOSS_Console("child that finished first: %d\tStatus: %d (should be 555)\n", exit_pid_1, exit_status1);
  USLOSS_Console("child that finished second: %d\tStatus: %d (should be 111)\n", exit_pid_2, exit_status2);
  
  USLOSS_Console(" ---------Ending Test 1 ----------\n");	

  P1_DumpProcesses();

  P1_Quit(0);
  return 0;
}
