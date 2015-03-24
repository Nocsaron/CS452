/*
 * failWithTooManyMailboxesTest.c
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
	USLOSS_Console("Starting test of creating more than the max number of mailboxes\n");
	int id = 0;
	int result;
	int i;
	for (i = 0; i < P2_MAX_MBOX; i++) {
		Sys_MboxCreate(5, 5, &id);
	}
	result = Sys_MboxCreate(5, 5, &id);
	assert(result == 0);
	assert(id == -1);
	USLOSS_Console("You passed all the tests!\n");
	return 7;
}
