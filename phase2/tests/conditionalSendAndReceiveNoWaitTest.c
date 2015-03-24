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
	result = Sys_MboxCondSend(id, input, &size);
	assert(result == 1);
	USLOSS_Console("Sent the message\n");
	char *hello = calloc(8, sizeof(char));
	size = 8;
	result = Sys_MboxCondReceive(id, hello, &size);
	assert(result == 1);
	USLOSS_Console("Received the message\n");
	USLOSS_Console("You passed all the tests! Treat yourself to a cookie!\n");
	return 7;
}
