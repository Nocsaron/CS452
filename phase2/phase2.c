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


#define DEBUG   1
#define SET_USERMODE USLOSS_PsrSet(USLOSS_PsrGet() & ~USLOSS_PSR_CURRENT_MODE)

typedef struct {
    int current_status;
    int parent_pid;
    int return_status;
    //Need list of children
} User_PCB;


/* -----------------------Global Variables--------------------------------- */
P1_Semaphore spawning;
char *launch_name;
int(*launch_func)(void *);
void *launch_arg;
int parent_pid;
/* ----------------------Function Defnitions------------------------------- */
int P2_Spawn(char *name, int(*func)(void *), void *arg, int stackSize, int priority);
int P2_Wait(int *status);
void set_up_sysvec();
void checkMode();
int user_launch(void *arg);
/* -----------------Sys Call Function Definitions-------------------------- */
void Spawn_Syscall(USLOSS_Sysargs *saPtr);
void Terminate_Syscall(USLOSS_Sysargs *saPtr);

int P2_Startup(void *arg) {
    spawning = P1_SemCreate(1);
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
