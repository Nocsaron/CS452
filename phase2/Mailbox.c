#include <stdlib.h>
#include "usloss.h"
#include "phase1.h"
#include "phase2.h"
 
static int ClockDriver(void *arg);
int P2_Spawn(char *name, int(*func)(void *), void *arg, int stackSize, int priority);
int P2_Wait(int *status);

typedef struct Slot *slot_ptr;
typedef struct MailBox  *mbox_ptr;
typedef struct Process  *proc_ptr;

#define TRUE 1
#define FALSE 0

#define FREE 0
#define ACTIVE 1

typedef struct Slot {
        slot_ptr   next;
        char      *message;
        int       size;
} Slot;

typedef struct MailBox {
        P1_Semaphore      empties;
        P1_Semaphore      fulls;
        P1_Semaphore      mutex;

        int          mboxID;
        int          status;
        int          num_slots;
        slot_ptr     slots;
        int          slot_size;
        int          num_slots_used;
} MailBox;

//The mail box table
MailBox mboxTable[P2_MAX_MBOX];

int
P2_Startup(void *arg)
{
    P1_Semaphore      running;
    int               status;
    int               pid;
    int               clockPID;
    /*
     * Check kernel mode
     */
    // ...
 
    /*
     * Create clock device driver 
     */
    running = P1_SemCreate(0);
    clockPID = P1_Fork("Clock driver", ClockDriver, (void *) running, USLOSS_MIN_STACK, 2);
    if (clockPID == -1) {
        USLOSS_Console("Can't create clock driver\n");
    }
    /*
     * Wait for the clock driver to start.
     */
    P1_P(running);
    /*
     * Create other device drivers.
     */
    // ...
 
    /* 
     * Create initial user-level process. You'll need to check error codes, etc. P2_Spawn
     * and P2_Wait are assumed to be the kernel-level functions that implement the Spawn and 
     * Wait system calls, respectively (you can't invoke a system call from the kernel).
     */
    pid = P2_Spawn("P3_Startup", P3_Startup, NULL,  4 * USLOSS_MIN_STACK, 3);
    pid = P2_Wait(&status);
 
 
    /*
     * Kill the device drivers
     */
    P1_Kill(clockPID, 1);
    // ...
    return 0;
}
 
static int
ClockDriver(void *arg)
{
    P1_Semaphore running = (P1_Semaphore) arg;
    int result;
    int status;
    int rc = 0;
 
    /*
     * Let the parent know we are running and enable interrupts.
     */
    P1_V(running);
    USLOSS_PsrSet(USLOSS_PsrGet() | USLOSS_PSR_CURRENT_INT);
    while(1) {
        result = P1_WaitDevice(USLOSS_CLOCK_DEV, 0, &status);
        if (result != 0) {
            rc = 1;
            goto done;
        }
        /*
         * Compute the current time and wake up any processes
         * whose time has come.
         */
    }
done:
    return rc; 
}

void Sys_MboxCreate(USLOSS_Sysargs *args){
        int num_slots = (int) args->arg1;
        int size = (int) args->arg2;

        int result = P2_MboxCreate(num_slots, size);

        args->arg1 = result;

        return;
}

int P2_MboxCreate(int num_slots, int size){

        int mboxID = -1;
        mbox_ptr mbox = NULL;

        /* Semaphore here */
        if (size >= 0 && num_slots >= 0) {    
            //Get an empty mailbox from the mailbox table
            int i = 0;
            while (i < P2_MAX_MBOX){
                if(mboxTable[i].status == FREE){

                    mbox->empties = P1_SemCreate(num_slots);
                    mbox->fulls = P1_SemCreate(0);
                    mbox->mutex = P1_SemCreate(1);

                    mbox = &mboxTable[i];
                    mboxID = mbox->mboxID;
                    mbox->num_slots = num_slots;
                    mbox->slot_size = size;
                    mbox->status = ACTIVE;
                    mbox->num_slots_used = 0;
                    break;
                }
                i++;
            }
        }
        else{
           mboxID = -2;
        }
        /*release semaphore*/

        return mboxID;
}

void Sys_MboxRelease(USLOSS_Sysargs *args){

    int mboxID = (int) args->arg1;
    int result = P2_MboxRelase(mboxID);
    args->arg4 = result;

    return;
}

int P2_MboxRelease(int mboxID){
    if (mboxID < 0 || mboxID >= P2_MAX_MBOX){
        return -1;
    }

    mbox_ptr mbox = &mboxTable[mboxID];
    //check if mbox is active
    if (mbox->status != ACTIVE){
        return -1;
    }

    slot_ptr slot = mbox->slots;
    slot_ptr temp;
    while (slot != NULL) {
        temp = slot;
        slot = slot->next;
        free(temp);
    }

    P1_SemFree(mbox->empties);
    P1_SemFree(mbox->fulls);
    P1_SemFree(mbox->mutex);

    mbox->status = FREE;
    mbox->num_slots = -1;
    mbox->slot_size = -1;
    mbox->num_slots_used = 0;

    return 0;

}

void Sys_MboxSend(USLOSS_Sysargs *args){
    int mboxID = (int) args->arg1;
    char *msg = (int) args->arg2;
    int msgSize = (int) args->arg3;

    int result = P2_MboxSend(mboxID, msg, msgSize, FALSE);
    args->arg4 = result;

    return;
}

void Sys_MboxCondSend(USLOSS_Sysargs *args){
    int mboxID = (int) args->arg1;
    char *msg = (int) args->arg2;
    int msgSize = (int) args->arg3;

    int result = P2_MboxSend(mboxID, msg, msgSize, TRUE);
    args->arg4 = result;

    return;
}

int P2_MboxSend(int mboxID, char *msg, int size, int cond){

    mbox_ptr mbox = &mboxTable[mboxID];

    P1_P(mbox->mutex);
    if(mbox->status != ACTIVE){
        P1_V(mbox->mutex);
        return -1;
    }

    if(mbox->slot_size < size){
        P1_V(mbox->mutex);
        return -1;
    }

    //all slots are used
    if(cond == TRUE && mbox->num_slots == mbox->num_slots_used){
        P1_V(mbox->mutex);
        return 1;
    }

    P1_V(mbox->mutex);
    P1_P(mbox->empties);
    P1_P(mbox->mutex);

    if(mbox->status != ACTIVE){
        P1_V(mbox->mutex);
        Sys_Terminate(42);
    }

    Slot *slot = malloc(sizeof(Slot));
    memcpy(slot->message, msg, size);

    slot_ptr temp = mbox->slots;
    while(temp != NULL){
        temp = temp->next;
    }

    temp = slot;

    mbox->num_slots_used++;
    P1_V(mbox->fulls);
    P1_V(mbox->mutex);

    return 0;

}

void Sys_MboxReceive(USLOSS_Sysargs *args){
    int mboxID = (int) args->arg1;
    char *rcv_buffer = (int) args->arg2;
    int size = (int) args->arg3;

    int result = P2_MboxReceive(mboxID, rcv_buffer, size, FALSE);

    if(result == -1){
        args->arg4 = result;
    }

    else{
        args->arg2 = result;
        args->arg4 = 0;
    }

    return;
}

void Sys_MboxCondReceive(USLOSS_Sysargs *args){
    int mboxID = (int) args->arg1;
    char *rcv_buffer = (int) args->arg2;
    int size = (int) args->arg3;

    int result = P2_MboxReceive(mboxID, rcv_buffer, size, TRUE);

    if(result == -1 || result == 1){
        args->arg4 = result;
    }
    else{
        args->arg2 = result;
        args->arg4 = 0;
    }

    return;
}

P2_MboxReceive(int mboxID, char *rcv_msg, int rcv_size, int cond){

    mbox_ptr mbox = &mboxTable[mboxID];

    P1_P(mbox->mutex);
    if(mbox->status != ACTIVE){
        P1_V(mbox->mutex);
        return -1;
    }

    //all slots are used
    if(cond == TRUE && mbox->num_slots_used == 0){
        P1_V(mbox->mutex);
        return 1;
    }

    P1_V(mbox->mutex);
    P1_P(mbox->fulls);
    P1_P(mbox->mutex);

    if(mbox->status != ACTIVE){
        P1_V(mbox->mutex);
        Sys_Terminate(42);
    }

    slot_ptr slot = mbox->slots;
    mbox->slots = mbox->slots->next;
    int true_size = min(rcv_size, slot->size);
    memcpy(rcv_msg, slot->message, true_size);

    free(slot);
    mbox->num_slots_used--;

    P1_V(mbox->empties);
    P1_V(mbox->mutex);

    return true_size;
}
















 
