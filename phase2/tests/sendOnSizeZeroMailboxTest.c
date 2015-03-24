#include <string.h>
#include <stdlib.h>
#include <usloss.h>
#include <phase1.h>
#include <phase2.h>
#include <assert.h>
#include <libuser.h>

int P3_Startup(void *arg) {
	USLOSS_Console(
			"Starting test of sending and recieving 0 size mailboxes with conditional send and receive\n");
	int id = -1;
	int result = Sys_MboxCreate(0, 500, &id);
	assert(result == 0);
	assert(id >= 0);
	USLOSS_Console("Created the mailbox\n");
	int size = 7;
	char *input = "hello!";
	USLOSS_Console("Should block here forever or reach a deadlock\n");
	result = Sys_MboxSend(id, input, &size);
	
	return 7;
}
