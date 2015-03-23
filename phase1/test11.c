/*
  test case for pseudo clock
  output should be a list of 10 numbers, all close to 100000 like
  
  ---------Starting Test 11 ----------
wait period : 90008
wait period : 99991
wait period : 99998
wait period : 99995
wait period : 99991
wait period : 99992
wait period : 99994
wait period : 99996
wait period : 99994
wait period : 99994
 ---------Ending Test 11 ----------
*/

#include "phase1.h"
#include <stdio.h>


int P2_Startup(void *arg){
  
  USLOSS_Console(" \n---------Starting Test 11 ----------\n");
  int status;
  int r;
  int temp;
  int i=0;
  
  while(i<10){
    //USLOSS_Console("before: %d\t", temp=USLOSS_Clock());
	++i;
    temp=USLOSS_Clock();
    r=P1_WaitDevice(USLOSS_CLOCK_DEV, 0, &status);
    USLOSS_Console("wait period : %d\n", USLOSS_Clock()-temp);
  }
 
  USLOSS_Console(" ---------Ending Test 11 ----------\n");
  return 0;
}
