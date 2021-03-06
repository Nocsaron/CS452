#include <syscall.h>
#include <assert.h>
#include <usloss.h>
#include <stdlib.h>
#include <stdarg.h>
#include <phase1.h>
#include <assert.h>
#include "libuser.h"


static int mbox;
static int numMessagesSent = 0;
//static int mutex;

int Recvr(void *arg) {
    int i = 0;
    USLOSS_Console("Starting the recv\n");
    while(numMessagesSent < 3) {
        int buf;
        int size = 5;
        USLOSS_Console("Receiving conditionally\n");
        int result = Sys_MboxCondReceive(mbox, &buf, &size);
        //Sys_SemP(mutex);
        //Sys_SemV(mutex);
        USLOSS_Console("Received\n");
        assert(result == 0 || result == 1 || result == -1);
        if (result == 0) {
            assert(size == 4);
            assert(buf == 9);
        } else if (result == -1) {
            Sys_Terminate(42);
        }
        
        i++;
    }
    USLOSS_Console("Leaving the recv\n");
    
   return 0;
}

int Sender(void *arg) {
    int i = 9;
    while(numMessagesSent < 3) {
    	USLOSS_Console("Sending: %d\n", i);
        int size = 4;
        int result = Sys_MboxSend(mbox, &i, &size);
        numMessagesSent++;
        assert(result == 0);
    }
    USLOSS_Console("Leaving the sender\n");
    Sys_MboxRelease(mbox);
    return 0;
}

int
P3_Startup(void *arg)
{ 
    int pid;
    int result;
    //Sys_SemCreate(1, &mutex);
    result = Sys_MboxCreate(0, 4, &mbox);
    assert(mbox != -1);
    assert(result == 0);
    
    //USLOSS_Console("Spawning the senders\n");
    result = Sys_Spawn("Sender 1", Sender, NULL, USLOSS_MIN_STACK, 4, &pid);
    assert(pid != -1);
    assert(result == 0);
    USLOSS_Console("Spawning the senders\n");
    result = Sys_Spawn("Sender 2", Sender, NULL, USLOSS_MIN_STACK, 4, &pid);
    assert(pid != -1);
    assert(result == 0);
    //USLOSS_Console("Spawning the senders\n");
    result = Sys_Spawn("Sender 3", Sender, NULL, USLOSS_MIN_STACK, 4, &pid);
    assert(pid != -1);
    assert(result == 0);
    result = Sys_Spawn("Receiver 1", Recvr, NULL, USLOSS_MIN_STACK, 5, &pid);
    assert(pid != -1);
    assert(result == 0);
    result = Sys_Spawn("Receiver 2", Recvr, NULL, USLOSS_MIN_STACK, 5, &pid);
    assert(pid != -1);
    assert(result == 0);
    result = Sys_Spawn("Receiver 3", Recvr, NULL, USLOSS_MIN_STACK, 5, &pid);
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
