/*
 * terminateNoChildrenTest.c
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

int P3_Startup(void *arg) {
	USLOSS_Console("Terminating this process which has no children\n");
	USLOSS_Console("Should see nothing after this line\n");
	Sys_Terminate(42);
	USLOSS_Console("If you see this something is wrong\n");
	return 7;
}
