#include <stdlib.h>
#include <usloss.h>
#include <usyscall.h>
#include <libuser.h>
#include <phase1.h>
#include <phase2.h>

/*
 * CSC452 - Operating Systems
 * Author: N R Callahan, Noah Sleiman
 * Phase2
 */

//TODO: Clock Driver
//TODO: P2_Sleep
#define DEBUG   1
#define SET_USERMODE USLOSS_PsrSet(USLOSS_PsrGet() & ~USLOSS_PSR_CURRENT_MODE)

typedef struct {
    int current_status;
    int parent_pid;
    int return_status;
    //Need list of children
} User_PCB;

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



/* -----------------------Global Variables--------------------------------- */
P1_Semaphore spawning;
char *launch_name;
int(*launch_func)(void *);
void *launch_arg;
int parent_pid;
typedef struct Slot *slot_ptr;
typedef struct MailBox  *mbox_ptr;
typedef struct Process  *proc_ptr;
//The mail box table
MailBox mboxTable[P2_MAX_MBOX];
/* ----------------------Function Defnitions------------------------------- */
int P2_Spawn(char *name, int(*func)(void *), void *arg, int stackSize, int priority);
int P2_Wait(int *status);
static int ClockDriver(void *arg);


void set_up_sysvec();
void checkMode();
int user_launch(void *arg);
/* -----------------Sys Call Function Definitions-------------------------- */
void Spawn_Syscall(USLOSS_Sysargs *saPtr);
void Terminate_Syscall(USLOSS_Sysargs *saPtr);

int P2_Startup(void *arg) {
    spawning = P1_SemCreate(1);
    USLOSS_Halt(0);
    set_up_sysvec();
    P2_Spawn("P3_Startup",P3_Startup,NULL,4*USLOSS_MIN_STACK,3);
    return 0;
}
/* ------------------------Helper Functions-------------------------------- */
void set_up_sysvec() {
    syscalls_vec[SYS_SPAWN] = Spawn_Syscall;
    syscalls_vec[SYS_TERMINATE] = Terminate_Syscall;
}
void checkMode() {
    if((USLOSS_PSR_CURRENT_MODE & USLOSS_PsrGet()) == 0 ) {
        USLOSS_Console("ERROR: Not in kernel mode.");
        USLOSS_Halt(1);
    }
}
/* -------------------------- Functions ----------------------------------- */
void Spawn_Syscall(USLOSS_Sysargs *saPtr) {
    if(DEBUG == 1) USLOSS_Console("Spawn_Syscall(): Executing\n");
    long error=-1;
//--Get info out of SysArgs Struct
    char *name = (char *)(saPtr->arg5);
    int(*func)(void *) = saPtr->arg1;
    void *arg = saPtr->arg2;
    int stackSize = (int)((long)(saPtr->arg3));
    int priority = (int)((long)saPtr->arg4);
//--Spawn the process
    long pid = (long)P2_Spawn(name,func,arg,stackSize,priority);
//--Return based on results
    if(DEBUG == 1) USLOSS_Console("Spawn_Syscall(): Spawn Process %s return with %d\n",name,pid);
    if(pid < 0) {
        saPtr->arg1=(void *)error;
        saPtr->arg4=(void *)error;
    }
    else {
        saPtr->arg1=(void *)pid;
        saPtr->arg4=(void *)(error+1);
    }
}
void Terminate_Syscall(USLOSS_Sysargs *saPtr) {
    if(DEBUG == 1) USLOSS_Console("Term_SysCall(): Executing\n");
    if(DEBUG == 1) USLOSS_Console("Term_Syscall(): Process exited with: %d\n",(int)((long)(saPtr->arg1)));
    P1_Quit((int)((long)saPtr->arg1));
}
int P2_Spawn(char *name, int(*func)(void *), void *arg, int stackSize, int priority) {
    if(DEBUG == 1) USLOSS_Console("P2_Spawn(): Executing\n");
    checkMode();
    P1_P(spawning);
    if(DEBUG==1) USLOSS_Console("P2_Spawn(): Past spawning semaphore.");
    launch_name = name;
    launch_func = func;
    launch_arg = arg;
    parent_pid = P1_GetPID();
    int pid =  P1_Fork(name,user_launch,arg,stackSize,priority);
    P1_V(spawning);
    return pid;
}
int P2_Sleep(int seconds) {
    if(seconds < 0) { return 1; }

    return 0;
}
int user_launch(void *arg) {
    SET_USERMODE;
//--Create user pcb
    User_PCB new_proc;
    new_proc.parent_pid=parent_pid;
    USLOSS_Console("Launching Startup!\n");
    int return_val = launch_func(arg);
    Sys_Terminate(return_val);
    return 0;
}
static int ClockDriver(void *arg) {
    P1_Semaphore running = (P1_Semaphore) arg;
    int result;
    int status;
    int time_elapsed = 0;
//    int time_desired = running;
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
        time_elapsed += 100;
        /*
         * Compute the current time and wake up any processes
         * whose time has come.
         */
    }
done:
    return rc;
}
/* -------------------------- Mailbox Functions ----------------------------------- */
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
