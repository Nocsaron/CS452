#include <syscall.h>
#include <assert.h>
#include <usloss.h>
#include <stdlib.h>
#include <stdarg.h>
#include <phase1.h>
#include <assert.h>
#include "libuser.h"


static int mbox;
static int numMessagesReceived = 0;
//static int mutex;

int Recvr(void *arg) {
    int i = 0;
    while(numMessagesReceived < 30) {
        int buf;
        int size = 5;
        int result = Sys_MboxReceive(mbox, &buf, &size);
        //Sys_SemP(mutex);
        numMessagesReceived++;
        //Sys_SemV(mutex);
        assert(result == 0);
        assert(size == 4);
        assert((buf >= 0 && buf < 10));
        i++;
    }
    USLOSS_Console("Leaving the recv\n");
    Sys_MboxRelease(mbox);
   return 0;
}

int Sender(void *arg) {
    int i;
    for(i = 0; i < 10; i++) {
    	USLOSS_Console("Sending: %d\n", i);
        int size = 4;
        int result = Sys_MboxSend(mbox, &i, &size);
        assert(result == 0);
    }
    USLOSS_Console("Leaving the sender\n");
    return 0;
}

int
P3_Startup(void *arg)
{ 
    int pid;
    int result;
    //Sys_SemCreate(1, &mutex);
    result = Sys_MboxCreate(100, 4, &mbox);
    assert(mbox != -1);
    assert(result == 0);
    result = Sys_Spawn("Receiver 1", Recvr, NULL, USLOSS_MIN_STACK, 4, &pid);
    assert(pid != -1);
    assert(result == 0);
    result = Sys_Spawn("Receiver 2", Recvr, NULL, USLOSS_MIN_STACK, 4, &pid);
    assert(pid != -1);
    assert(result == 0);
    result = Sys_Spawn("Receiver 3", Recvr, NULL, USLOSS_MIN_STACK, 4, &pid);
    assert(pid != -1);
    assert(result == 0);
    //USLOSS_Console("Spawning the senders\n");
    result = Sys_Spawn("Sender 1", Sender, NULL, USLOSS_MIN_STACK, 5, &pid);
    assert(pid != -1);
    assert(result == 0);
    //USLOSS_Console("Spawning the senders\n");
    result = Sys_Spawn("Sender 2", Sender, NULL, USLOSS_MIN_STACK, 5, &pid);
    assert(pid != -1);
    assert(result == 0);
    //USLOSS_Console("Spawning the senders\n");
    result = Sys_Spawn("Sender 3", Sender, NULL, USLOSS_MIN_STACK, 5, &pid);
    assert(pid != -1);
    assert(result == 0);
    //USLOSS_Console("Spawning the senders\n");
    Sys_Wait(&pid, &result);
    Sys_Wait(&pid, &result);
    Sys_Wait(&pid, &result);
    Sys_Wait(&pid, &result);
    Sys_Wait(&pid, &result);
    Sys_Wait(&pid, &result);
    return 0;
}
