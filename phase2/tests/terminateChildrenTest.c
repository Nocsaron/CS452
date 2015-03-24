/*
 * terminateChildrenTest.c
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


 int child(void *arg) {
 	USLOSS_Console("Child here\n");
    return 0;
}

int P3_Startup(void *arg) {
	USLOSS_Console("Terminating this process which has children\n");
	int pid;
	int i;
	for (i = 0; i < 3; i++) {
		Sys_Spawn("child", child, i, USLOSS_MIN_STACK, 5, &pid);
	}
	USLOSS_Console("Terminating in P3_Startup\n");
	Sys_Terminate(42);
	USLOSS_Console("If you see this something is wrong\n");
	return 7;
}
