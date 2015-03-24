/*
 * terminateChildrenTwoTreesTest.c
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

int sem;

int child(void *arg) {
 	USLOSS_Console("Child here\n");
    return 0;
}

int firstTree(void *arg) {
	USLOSS_Console("First tree here\n");
	
	int i;
	int pid;
	for (i = 0; i < 30; i++) {
		Sys_SemP(sem);
		Sys_Spawn("child of first tree", child, i, USLOSS_MIN_STACK, 5, &pid);
		Sys_SemV(sem);
	}
	USLOSS_Console("Returning from first tree\n");
	return 1;
}

int secondTree(void *arg) {
	USLOSS_Console("Second tree here\n");
	int i;
	int pid;
	for (i = 0; i < 30; i++) {
		Sys_SemV(sem);
		Sys_Spawn("child of second tree", child, i, USLOSS_MIN_STACK, 5, &pid);
		Sys_SemP(sem);
	}
	USLOSS_Console("Returning from second tree\n");
	return 2;
}

int P3_Startup(void *arg) {
	USLOSS_Console("Terminating this process which has children\n");
	int createResult;
	createResult = Sys_SemCreate(0, &sem);
	int pid;
	Sys_Spawn("child", firstTree, NULL, USLOSS_MIN_STACK, 4, &pid);
	Sys_Spawn("child", secondTree, NULL, USLOSS_MIN_STACK, 4, &pid);
	int result;
	Sys_Wait(&pid, &result);
	Sys_Wait(&pid, &result);
	USLOSS_Console("Terminating in P3_Startup\n");
	Sys_Terminate(3);
	USLOSS_Console("If you see this something is wrong\n");
	return 3;
}
