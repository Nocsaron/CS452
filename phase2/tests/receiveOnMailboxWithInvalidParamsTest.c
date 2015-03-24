#include <string.h>
#include <stdlib.h>
#include <usloss.h>
#include <phase1.h>
#include <phase2.h>
#include <assert.h>
#include <libuser.h>

int P3_Startup(void *arg) {
	USLOSS_Console(
			"Starting test of creating a simple mailbox with no problems\n");
	int id = -1;
	int result = Sys_MboxCreate(1, 500, &id);
	assert(result == 0);
	assert(id >= 0);
	USLOSS_Console("Created the mailbox\n");
	int size = 7;
	char *input = calloc(7, sizeof(char));
	result = Sys_MboxReceive(-5, input, &size);
	assert(result == -1);
	result = Sys_MboxReceive(5000, input, &size);
	assert(result == -1);

	USLOSS_Console("You passed all the tests! Treat yourself to a cookie!\n");
	return 7;
}