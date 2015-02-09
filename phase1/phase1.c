
/* ------------------------------------------------------------------------
   skeleton.c
 
   Skeleton file for Phase 1. These routines are very incomplete and are
   intended to give you a starting point. Feel free to use this or not.
 
 
   ------------------------------------------------------------------------ */
 
#include <stddef.h>
#include <stdlib.h>
#include "usloss.h"
#include "phase1.h"
#include "linked_list.h"

#define DEBUG 1
 
/* -------------------------- Globals ------------------------------------- */
 
int childpid1, childpid2;
typedef struct {
    USLOSS_Context      context;
    int                 (*startFunc)(void *);   /* Starting function */
    void                 *startArg;             /* Arg to starting function */
    unsigned int priority;
    char *name; 
    char *stack;
    unsigned int startTime;
    unsigned int cyclesUsed;
    int status;
    int parentPid;
    List *list;
} PCB;

List *readyList;
List *blockedList;
/* the process table */
PCB procTable[P1_MAXPROC];
 
 
/* current process ID */
int pid = -1;
 
/* number of processes */
 
int numProcs = 0;

char **STATUS=(char *[]){"Ready","Finished","Running"};

static int sentinel(void *arg);
static void launch(void);
/* -------------------------- Functions ----------------------------------- */
int P2_Startup(void *arg) {
    USLOSS_Console("P2_Startup()\n");
    P1_DumpProcesses();
    USLOSS_Halt(0);
}

void P1_Quit(int status){

    if((USLOSS_PSR_CURRENT_MODE & USLOSS_PsrGet()) == 0){
        USLOSS_Halt(1);
    }

    Node *temp = procTable[pid].list->first;

    while(temp->next != NULL){
        procTable[temp->pid].parentPid = -1;
        temp = temp->next;
    }

    if(procTable[pid].parentPid < 0){
        remove_node(procTable[procTable[pid].parentPid].list, pid);
    }

    free(procTable[pid].stack);
    procTable[pid].status = NO_PROCESS;


}

int P1_Kill(int PID, int status) {
    if(DEBUG) USLOSS_Console("P1_Kill(): entering\n");
//--Check to make sure this is a valid Kill
//--Are you trying to kill yourself?  I can't let you do that Dave.
    if(pid == PID) {
        if(DEBUG) USLOSS_Console("P1_Kill(): You cannot kill yourself\n");
        return -2;
    }
//--Do you even know what your doing?  This process can not even exist
    if(PID < 0 || PID > P1_MAXPROC) {
        if(DEBUG) USLOSS_Console("P1_Kill(): Invalid PID (Out of Bounds)\n");
        return -1;
    }
//--Uh oh, that process doesn't actually exist so you can't kill it.....yet.
    else if(procTable[PID].status=NO_PROCESS) {
        if(DEBUG) USLOSS_Console("P1_Kill(): No such process\n");
        return -1;
    }

//--Valid Kill Order. Execute.
    
}   


/* ------------------------------------------------------------------------
   Name - startup
   Purpose - Initializes semaphores, process lists and interrupt vector.
             Start up sentinel process and the P2_Startup process.
   Parameters - none, called by USLOSS
   Returns - nothing
   Side Effects - lots, starts the whole thing
   ----------------------------------------------------------------------- */
void startup()
{
    USLOSS_Console("Running startup()\n");
    int i;
  /* initialize the process table here */
    if(DEBUG) USLOSS_Console("startup(): Init PCBs\n");
    for(i = 0; i < P1_MAXPROC; i++) {
        procTable[i].status=NO_PROCESS;
    }
  /* Initialize the Ready list, Blocked list, etc. here */
    if(DEBUG) USLOSS_Console("startup(): Init Ready List\n");
    readyList = create_list();
    if(DEBUG) USLOSS_Console("startup(): Init Blocked List\n");
    blockedList = create_list();

  /* Initialize the interrupt vector here */
 
  /* Initialize the semaphores here */
 
  /* startup a sentinel process */
    if(DEBUG) USLOSS_Console("startup(): Forking sentinel\n");
  int result = P1_Fork("sentinel", sentinel, NULL, USLOSS_MIN_STACK, 6);
    if(result < 0) {
        if(DEBUG) USLOSS_Console("startup(): Forking sentinel failed!\n\tReturn Code: %d\n",result);
        USLOSS_Halt(1);
    }
  /* start the P2_Startup process */
  P1_Fork("P2_Startup", P2_Startup, NULL, 4 * USLOSS_MIN_STACK, 1);
 
    dispatcher(); 
  /* Should never get here (sentinel will call USLOSS_Halt) */
 
  return;
} /* End of startup */
 
/* ------------------------------------------------------------------------
   Name - finish
   Purpose - Required by USLOSS
   Parameters - none
   Returns - nothing
   Side Effects - none
   ----------------------------------------------------------------------- */
void finish()
{
#ifdef DEBUG
    USLOSS_Console("in finish...\n");
#endif /*DEBUG*/
} /* End of finish */
 
/* ------------------------------------------------------------------------
   Name - P1_Fork
   Purpose - Gets a new process from the process table and initializes
             information of the process.  Updates information in the
             parent process to reflect this child process creation.
   Parameters - the process procedure address, the size of the stack and
                the priority to be assigned to the child process.
   Returns - the process id of the created child or an error code.
   Side Effects - ReadyList is changed, procTable is changed, Current
                  process information changed
   ------------------------------------------------------------------------ */
int P1_Fork(char *name, int (*f)(void *), void *arg, int stacksize, int priority)
{

    if((USLOSS_PSR_CURRENT_MODE & USLOSS_PsrGet()) == 0){
        USLOSS_Halt(1);
    }

    if(priority > 6 || priority < 1){
      return -3;
    }

    if(stacksize < USLOSS_MIN_STACK){
      return -2;
    }

    disableInterrupts();

    int newPid = 0;
    if(DEBUG) USLOSS_Console("P1_Fork(): Looking for empty block\n");
    while(procTable[newPid].status != NO_PROCESS){
      if(newPid == P1_MAXPROC-1){
        return -1;
      }
      newPid++;
    }
    
    PCB newProcessBlock;
    procTable[newPid] = newProcessBlock;

    void *stack;
    /* newPid = pid of empty PCB here */
    if(DEBUG) USLOSS_Console("P1_Fork(): Initiliazing PCB\n");
    procTable[newPid].startFunc = f;
    procTable[newPid].startArg = arg;
    procTable[newPid].startTime = 0;
    procTable[newPid].name = name;
    procTable[newPid].cyclesUsed = 0;
    procTable[newPid].status = READY;
    procTable[newPid].parentPid = pid;
    procTable[newPid].list = create_list();
    procTable[newPid].priority=priority;

    /* Allocate stack here */

    if(DEBUG) USLOSS_Console("P1_Fork(): Initiliazing PCB Stack\n");
    procTable[newPid].stack = malloc(stacksize);

    if(DEBUG) USLOSS_Console("P1_Fork(): Calling ContextInit\n");
    USLOSS_ContextInit(&(procTable[newPid].context), USLOSS_PsrGet(), procTable[newPid].stack, 
        stacksize, launch);

    /* Add new process to the parent's child list */
    if(DEBUG) USLOSS_Console("P1_Fork(): Appending to Parent's childs list\n");
    if(pid > -1)
        append_list(procTable[pid].list, newPid, procTable[newPid].status);

    /* Add new process to the readyList */
    if(DEBUG) USLOSS_Console("P1_Fork(): Putting new process in readyList\n");
    append_list(readyList, newPid, procTable[newPid].status);


    numProcs++;
    if(DEBUG) USLOSS_Console("P1_Fork(): Calling dispatcher()\n");
    dispatcher();
    
    return newPid;
} /* End of fork */


 
/* ------------------------------------------------------------------------
   Name - launch
   Purpose - Dummy function to enable interrupts and launch a given process
             upon startup.
   Parameters - none
   Returns - nothing
   Side Effects - enable interrupts
   ------------------------------------------------------------------------ */
void launch(void)
{
    if(DEBUG) USLOSS_Console("launch(): entered\n");
  int  rc;
    if(DEBUG) USLOSS_Console("launch(): PsrSet\n");
  USLOSS_PsrSet(USLOSS_PsrGet() | USLOSS_PSR_CURRENT_INT);
    if(DEBUG) USLOSS_Console("launch(): Start Function\t%s %d\n",procTable[pid].name,pid);
  rc = procTable[pid].startFunc(procTable[pid].startArg);
  /* quit if we ever come back */
  P1_Quit(rc);
} /* End of launch */
 
 
 
/* ------------------------------------------------------------------------
   Name - sentinel
   Purpose - The purpose of the sentinel routine is two-fold.  One
             responsibility is to keep the system going when all other
             processes are blocked.  The other is to detect and report
             simple deadlock states.
   Parameters - none
   Returns - nothing
   Side Effects -  if system is in deadlock, print appropriate error
                   and halt.
   ----------------------------------------------------------------------- */
int sentinel (void *notused)
{
    if(DEBUG) USLOSS_Console("sentinel():  Running sentinel process");
    while (numProcs > 1)
    {
        /* Check for deadlock here */
        /* USLOSS_WaitInt(); */
    }
    USLOSS_Halt(0);
    /* Never gets here. */
    return 0;
} /* End of sentinel */

/* ------------------------------------------------------------------------
   Name - disableInterrupts
   Purpose - Disables interrupts by changing the PSR_CURRENT_INT;
   Parameters - none
   Returns - nothing
   Side Effects -  changes the PSR_CURRENT_INT;
   ----------------------------------------------------------------------- */

void disableInterrupts() {
  if((USLOSS_PSR_CURRENT_MODE & USLOSS_PsrGet()) == 0) {
    USLOSS_Halt(1);
  } else
    USLOSS_PsrSet(USLOSS_PsrGet() & ~USLOSS_PSR_CURRENT_INT);
} 

/* ------------------------------------------------------------------------
   Name - dispatcher
   Purpose - dispatches processes. 
   Parameters - none
   Returns - nothing
   Side Effects - processes change
   ----------------------------------------------------------------------- */
void dispatcher() {
    if(DEBUG) USLOSS_Console("dispatcher(): Checking mode\n");
    if((USLOSS_PSR_CURRENT_MODE & USLOSS_PsrGet()) == 0) {
        USLOSS_Halt(1);
    }

    if(DEBUG) USLOSS_Console("dispatcher(): Popping process from readyList\n");
    Node *next = pop(readyList); //obtain the next process from readyList and remove node from head of list

    if(DEBUG) USLOSS_Console("dispatcher(): Checking current process\n");
    if(DEBUG) USLOSS_Console("dispatcher(): Current Process: %d\n",pid);
    if(pid < 0) {
        if(DEBUG) USLOSS_Console("dispatcher(): Ready Process: %s %d %d\n",procTable[next->pid].name,next->pid,procTable[next->pid].priority);
    //--Check if 1st or 2nd run
        if(procTable[next->pid].priority == 6) { 
        //--Sentinel process - first run
            if(DEBUG) USLOSS_Console("dispatcher(): First run, dont run Sentinel\n");
            append_list(readyList,next->pid,next->status);
        }
        else {
        //--Second run. Run this process
            if(DEBUG) USLOSS_Console("dispatcher(): Second run, run process %s %d\n",procTable[next->pid].name,next->pid);
            pid = next->pid;
            USLOSS_ContextSwitch(NULL,&procTable[pid].context);
        }
    }
    else {
        if(DEBUG) USLOSS_Console("dispatcher(): Not 1st/2nd run, switching contexts\n");
        USLOSS_ContextSwitch(&procTable[pid].context,&procTable[next->pid].context);
    }
    if(DEBUG) USLOSS_Console("dispatcher(): exiting\n");
}

void P1_DumpProcesses() {
    int i;
    USLOSS_Console("--------------------------");
    USLOSS_Console("------Process Dump--------");
    USLOSS_Console("--------------------------\n");
    USLOSS_Console("%10s%10s%10s%10s%13s%10s%15s\n","Name","PID","PPID","Priority","Status","Children","Clock Cycles");
    for(i = 0; i < P1_MAXPROC; i++) {
        if(procTable[i].status != NO_PROCESS) {
            PCB proc = procTable[i];
            USLOSS_Console("%10s%10d%10d%10d%13s%10d%15d\n",proc.name,pid,proc.parentPid,proc.priority,STATUS[procTable[i].status],-1,proc.cyclesUsed);
        }
    }
    USLOSS_Console("--------------------------");
    USLOSS_Console("--------------------------");
    USLOSS_Console("--------------------------\n");
}



