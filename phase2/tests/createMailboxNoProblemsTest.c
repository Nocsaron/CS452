#include <stdlib.h>
#include <usloss.h>
#include <phase1.h>
#include <phase2.h>
#include <assert.h>
#include <libuser.h>

int
P3_Startup(void *arg) {
	USLOSS_Console("Starting test of creating a simple mailbox with no problems\n");
	int id = 0;
	int result = Sys_MboxCreate(0, 0, &id);
	assert(result == 0);
	assert(id >= 0);
	return 7;
}
