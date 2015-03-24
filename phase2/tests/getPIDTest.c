/*
 * getPIDTest.c
 *
 *  Created on: Mar 8, 2015
 *      Author: jeremy
 */

#include <string.h>
#include <stdlib.h>
#include <usloss.h>
#include <phase1.h>
#include <phase2.h>
#include <assert.h>
#include <libuser.h>

int parentPID;
int childPID;

int child(void *arg) {
 	USLOSS_Console("Child here\n");
 	int thisPID;
 	Sys_GetPID(&thisPID);
 	assert(childPID == thisPID);
 	Sys_GetPID(&childPID);
    return 0;
}

int P3_Startup(void *arg) {
	USLOSS_Console("Start test\n");
	int parentPID;
	Sys_GetPID(&parentPID);
	assert(parentPID >= 0);
	assert(parentPID < P1_MAXPROC);
	Sys_Spawn("child", child, NULL, USLOSS_MIN_STACK, 5, &childPID);
	int pid;
	int result;
	Sys_Wait(&pid, &result);
	assert(childPID != parentPID);
	USLOSS_Console("Test end, let your colors burst\n");
	return 7;
}
