/*
  test case for alarm
  
  O/P should resemble
  
  ---------Starting Test 10 ----------
wait period: 60005  (should be close to 60000)
 ---------Ending Test 10 ----------
*/

#include "phase1.h"
#include <stdio.h>

int P2_Startup(void *arg){
	
	USLOSS_Console(" \n---------Starting Test 10 ----------\n");
	int status;
	int temp;
	temp = USLOSS_Clock();
	//USLOSS_Console("before set alarm: %d\n", USLOSS_Clock());
	USLOSS_DeviceOutput(USLOSS_ALARM_DEV, 0, (void *) 3);
	P1_WaitDevice(USLOSS_ALARM_DEV, 0, &status);
	//USLOSS_Console("after set alarm: %d\n", USLOSS_Clock());
	temp = USLOSS_Clock() - temp;	
	USLOSS_Console("wait period: %d  (should be close to 60000)\n", temp);
	
	USLOSS_Console(" ---------Ending Test 10 ----------\n");
	return 0;
}
