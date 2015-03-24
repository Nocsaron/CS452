/*
 * simpleClockDriverTest.c
 *
 *  Created on: Mar 9, 2015
 *      Author: tishihara
 */

#include <string.h>
#include <stdlib.h>
#include <usloss.h>
#include <phase1.h>
#include <phase2.h>
#include <assert.h>
#include <libuser.h>


int SleepyProc(void *arg) {
    USLOSS_Console("SleepyProc trys to sleep for -5 seconds...\n");
	int initTime;
	Sys_GetTimeofDay(&initTime);
	Sys_Sleep(-5);
	int finalTime;
	Sys_GetTimeofDay(&finalTime);
	USLOSS_Console("Total time SleepyProc: %f seconds\n", (finalTime-initTime)/1000000.0);
	USLOSS_Console("SleepyProc didn't get any sleep!\n");
    return 1;
}

int P3_Startup(void *arg) {
	USLOSS_Console("Testing invalid calls to Sys_Sleep\n");
    
    // Run sleepy process
    int pid;
    int result;
    result = Sys_Spawn("SleepyProc", SleepyProc, NULL, USLOSS_MIN_STACK, 4, &pid);
    assert(pid != -1);
    assert(result == 0);
    
    
    // P3_Startup sleeps (sleep time is longer than children's)
    // Making sleep time shorter doesn't cause errors, but
    // child procs don't finish before P3_Startup returns
    USLOSS_Console("P3_Startup sleeps for 0 seconds...\n");
	int initTime;
	Sys_GetTimeofDay(&initTime);
	Sys_Sleep(0);
	int finalTime;
	Sys_GetTimeofDay(&finalTime);
	USLOSS_Console("Total time P3_Startup: %f seconds\n", (finalTime-initTime)/1000000.0);
	USLOSS_Console("P3_Startup \"woke up\"|\n");
	USLOSS_Console("You passed the test! Treat yourself to a cookie!\n");
	return 7;
}
