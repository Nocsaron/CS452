/*
 * sysspawnWithInvalidArgsTest.c
 *
 *  Created on: Mar 1, 2015
 *      Author: jermowery
 */
#include <stdlib.h>
#include <usloss.h>
#include <phase1.h>
#include <phase2.h>
#include <assert.h>

void notPrintf(void *arg) {
	USLOSS_Console("Output of Test Process\n");
	USLOSS_Console("I am not a god damned four, I am: %d\n", *((int *)(&arg)));
}

int
P3_Startup(void *arg) {
	// Verify that checks that the function is non-null is working
	USLOSS_Console("Start test\n");
	USLOSS_Sysargs *args = calloc(1, sizeof(USLOSS_Sysargs));
	args->number = SYS_SPAWN;
	args->arg1 = NULL;
	args->arg2 = (void *)12;
	args->arg3 = (void *)USLOSS_MIN_STACK;
	args->arg4 = (void *)1;
	args->arg5 = "Test Process";
	USLOSS_Syscall(args);
	assert(*((int *)&args->arg4) == -1);
	USLOSS_Console("Part 1 passed\n");
	// Verify that the checks that the size is too small works
	args->arg1 = notPrintf;
	args->arg3 = (void *)(USLOSS_MIN_STACK -1);
	USLOSS_Syscall(args);
	assert(*((int *)&args->arg4) == -1);
	USLOSS_Console("Part 2 passed\n");
	args->arg3 = (void *)(-500);
	USLOSS_Syscall(args);
	assert(*((int *)&args->arg4) == -1);
	USLOSS_Console("Part 3 passed\n");
	// Verify that the checks that the priority is too high works
	args->arg4 = (void *)0;
	USLOSS_Syscall(args);
	assert(*((int *)&args->arg4) == -1);
	USLOSS_Console("Part 4 passed\n");
	args->arg4 = (void *)-5;
	USLOSS_Syscall(args);
	assert(*((int *)&args->arg4) == -1);
	USLOSS_Console("Part 5 passed\n");
	// Verify that the checks that the priority is too low works
	args->arg4 = (void *)20;
	USLOSS_Syscall(args);
	assert(*((int *)&args->arg4) == -1);
	USLOSS_Console("Part 6 passed\n");
	args->arg4 = (void *)6;
	USLOSS_Syscall(args);
	assert(*((int *)&args->arg4) == -1);
	USLOSS_Console("Part 7 passed\n");
	USLOSS_Console("All tests passed, baby you are a firework\n");
	USLOSS_Console("End test\n");
	return 7;
}
