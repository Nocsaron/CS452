#include <syscall.h>
#include <usloss.h>
#include <stddef.h>
#include <assert.h>
#include "libuser.h"

static int mbox;
int child(void *arg) {
    int size = 4;
    int i = 42;
    int result = Sys_MboxSend(mbox, &i, &size);
    assert(result == 0);
    USLOSS_Console("Child is returning\n");
    return 0;
}

int P3_Startup(void *arg) {
	int result;
	Sys_MboxCreate(0, 4, &mbox);
    int pid; 
    Sys_Spawn("child", child, NULL, USLOSS_MIN_STACK, 4, &pid);
    int buf;
    int size = 4;
    result = Sys_MboxReceive(mbox, &buf, &size);
    assert(result == 0);
    assert(buf == 42);
    Sys_Wait(&pid, &result);
    USLOSS_Console("Spawned child\n");
    USLOSS_Console("You passed the test! Treat yourself to a cookie!\n");
    return 0;
}