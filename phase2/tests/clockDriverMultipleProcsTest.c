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


int SleepyProc1(void *arg) {
    USLOSS_Console("SleepyProc1 sleeps for 5 seconds...\n");
	int initTime;
	Sys_GetTimeofDay(&initTime);
	Sys_Sleep(5);
	int finalTime;
	Sys_GetTimeofDay(&finalTime);
	USLOSS_Console("Total time SleepyProc1: %f seconds\n", (finalTime-initTime)/1000000.0);
	USLOSS_Console("SleepyProc1 woke up!\n");
    return 1;
}

int SleepyProc2(void *arg) {
    USLOSS_Console("SleepyProc2 sleeps for 8 seconds...\n");
	int initTime;
	Sys_GetTimeofDay(&initTime);
	Sys_Sleep(8);
	int finalTime;
	Sys_GetTimeofDay(&finalTime);
	USLOSS_Console("Total time SleepyProc2: %f seconds\n", (finalTime-initTime)/1000000.0);
	USLOSS_Console("SleepyProc2 woke up!\n");
    return 2;
}

int SleepyProc3(void *arg) {
    USLOSS_Console("SleepyProc3 sleeps for 2 seconds...\n");
	int initTime;
	Sys_GetTimeofDay(&initTime);
	Sys_Sleep(2);
	int finalTime;
	Sys_GetTimeofDay(&finalTime);
	USLOSS_Console("Total time SleepyProc3: %f seconds\n", (finalTime-initTime)/1000000.0);
	USLOSS_Console("SleepyProc3 woke up!\n");
    return 3;
}

int P3_Startup(void *arg) {
	USLOSS_Console("Testing multiple processes calling Sys_Sleep\n");
    
    // Run sleepy processes
    int pid;
    int result;
    result = Sys_Spawn("SleepyProc1", SleepyProc1, NULL, USLOSS_MIN_STACK, 4, &pid);
    assert(pid != -1);
    assert(result == 0);
    result = Sys_Spawn("SleepyProc2", SleepyProc2, NULL, USLOSS_MIN_STACK, 4, &pid);
    assert(pid != -1);
    assert(result == 0);
    result = Sys_Spawn("SleepyProc3", SleepyProc3, NULL, USLOSS_MIN_STACK, 4, &pid);
    assert(pid != -1);
    assert(result == 0);
    
    // P3_Startup sleeps (sleep time is longer than children's)
    // Making sleep time shorter doesn't cause errors, but
    // child procs don't finish before P3_Startup returns
    USLOSS_Console("P3_Startup sleeps for 10 seconds...\n");
	int initTime;
	Sys_GetTimeofDay(&initTime);
	Sys_Sleep(10);
	int finalTime;
	Sys_GetTimeofDay(&finalTime);
	USLOSS_Console("Total time P3_Startup: %f seconds\n", (finalTime-initTime)/1000000.0);
	USLOSS_Console("P3_Startup woke up!\n");
	USLOSS_Console("You passed the test! Treat yourself to a cookie!\n");
	return 7;
}
