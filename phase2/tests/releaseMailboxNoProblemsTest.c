/*
 * releaseMailboxNoProblemsTest.c
 *
 *  Created on: Mar 8, 2015
 *      Author: jeremy
 */

#include <stdlib.h>
#include <usloss.h>
#include <phase1.h>
#include <phase2.h>
#include <assert.h>
#include <libuser.h>

int
P3_Startup(void *arg) {
	USLOSS_Console("Starting test of releasing simple mailbox with no problems\n");
	int id = 0;
	int result = Sys_MboxCreate(0, 0, &id);
	assert(result == 0);
	assert(id >= 0);
	USLOSS_Console("Mailbox created successfully\n");
	result = Sys_MboxRelease(id);
	assert(result == 0);
	USLOSS_Console("Existing mailbox released successfully\n");
	result = Sys_MboxRelease(id);
	assert(result == -1);
	USLOSS_Console("Non-existing mailbox released successfully\n");
	return 7;
}
