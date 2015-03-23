/*
  test case for deadlock detection
  system should halt before output instruction
  
  O/P should never contain "should not see this"
  
  like
  
---------Starting Test 12 ----------
deadlock!

*/

#include "phase1.h"
#include <stdio.h>


int P2_Startup(void *arg){

    USLOSS_Console(" \n---------Starting Test 12 ----------\n");
	P1_Semaphore sem = P1_SemCreate(0);
	P1_P(sem);
	USLOSS_Console("should not see this\n");
	return 0;
}
