/*
 * failWithNegativeNumMessagesMBoxTest.c
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
	USLOSS_Console("Starting test of creating a mailbox with a negative number of messages\n");
	int id = 0;
	int result;
	result = Sys_MboxCreate(-5, 5, &id);
	assert(result == -1);
	USLOSS_Console("You passed all the tests!\n");
	return 7;
}

