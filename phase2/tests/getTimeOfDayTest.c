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

int P3_Startup(void *arg) {
	USLOSS_Console("Start test\n");
	int tod;
	Sys_GetTimeofDay(&tod);
	assert(tod > 0);
	int i;
	for (i = 0; i < 10000; i++) {

	}
	int tod2;
	Sys_GetTimeofDay(&tod2);
	assert(tod2 > 0);
	assert(tod < tod2);
	USLOSS_Console("End test, everything is awesome!\n");
	return 7;
}
