/*------------------------------------------------------------------------
 * CS452 - Phase 1 Operating System
 * Author: N R Callahan, Noah Sleiman
 *
 * ----------------------------------------------------------------------*/


#include <stddef.h>
#include <stdlib.h>
#include "usloss.h"
#include "phase1.h"
#include "linked_list.h"

/* ----------------------------Constants----------------------------------- */
#define DEBUG       3   /* 0: No debug statements
                           1: Some debug statements
                           2: Most debug statements
                           3: All debug statements */

#define MINPRIORITY 6
#define MAXPRIORITY 1

#define NO_PROCESS  0
#define READY       1
#define RUNNING     2
#define FINISHED    3

/* ----------------------Function Defnitions------------------------------- */
int sentinel(void * notused);
int isKernel();
void dispatcher();
/* -------------------------- Globals ------------------------------------- */

typedef struct {
    USLOSS_Context      context;                /* State of process */
    int                 (*startFunc)(void *);   /* Starting Function */
    void                *startArg;              /* Argument to starting function */
    char                *name;                  /* Process Name */
    int                 PID;                    /* Process Identifier */
    int                 priority;               /* Process Priority */
    char                *stack;                 /* Process Stack */
    int                 status;                 /* Current Status of Process */
    int                 return_status;          /* Return Status */
    int                 time_start;             /* Start time of process */
    int                 cyclesUsed;             /* Clock cycles process has run */
    int                 parentPID;              /* PID of Parent Process */
    List                *children;              /* List of Child Processes */
} PCB;


//--Process Table
    PCB procTable[P1_MAXPROC];
//--Current PID
    int currentPID = -1;        /* Intial PID (prior to any process) is -1 */
//--Number of Process
    int numberProcs = 0;        /* Number of process running */
//--ReadyList
    List *readyList;
//--BlockedList
    List *blockedList;

/* -------------------------- Functions ----------------------------------- */
/* ------------------------------------------------------------------------
 * Name:        startup
 * Purpose:     Required by USLOSS
 *              Initializes semaphores, process table, interupts
 *              Starts the sentinel process and calls P2_Startup
 * Parameter:   none
 * Returns:     nothing
 * Side Effects:Starts System
 * ----------------------------------------------------------------------- */
void startup() {
    if(DEBUG == 3) USLOSS_Console("startup(): Entered startup\n");
//--Initialize Process Table
    if(DEBUG == 3) USLOSS_Console("startup(): Initializing Process Table\n");
    int i = 0;
    for(i = 0; i < P1_MAXPROC; i++) {
        procTable[i].status=NO_PROCESS;
    }
    USLOSS_Console("procTable[0].status = %d\n",procTable[i].status);
//--Initialize Ready and Blocked lists
    if(DEBUG == 3) USLOSS_Console("startup(): Initializing Ready and Blocked Lists\n");
    readyList   = create_list();
    blockedList = create_list();
//--Setup sentinel process
    if(DEBUG >= 2) USLOSS_Console("startup(): Forking Sentinel\n");
    P1_Fork("sentinel",sentinel,NULL,USLOSS_MIN_STACK, MINPRIORITY);
//--Setup start process
    //if(DEBUG >= 2) USLOSS_Console("startup(): Forking P2_Startup\n");
    //P1_Fork("P2_Startup",P2_Startup,NULL,4*USLOSS_MIN_STACK,1);
    P1_DumpProcesses();
    if(DEBUG == 3) USLOSS_Console("startup(): End of startup\n");
    return;
}
/* ------------------------------------------------------------------------
 * Name:        finish 
 * Purpose:     Required by USLOSS
 * Parameter:   none
 * Returns:     nothing
 * Side Effects:None
 * ----------------------------------------------------------------------- */
void finish() {
//--Really nothing to do there, just insert a print statement
}
/* ------------------------------------------------------------------------
 * Name:        launch() 
 * Purpose:     Dummy function to enable interrupts and launch a process
 * Parameter:   none
 * Returns:     nothing
 * Side Effects:enable interrupts
 * ----------------------------------------------------------------------- */
void launch(void) {
    //--I have no idea whats going on here. Copy from skeleton file
    int pid = P1_GetPID();
    if(DEBUG == 3) USLOSS_Console("launch(): Entered launch()\n");
    int rc;
    USLOSS_PsrSet(USLOSS_PsrGet() | USLOSS_PSR_CURRENT_INT);
    rc = procTable[pid].startFunc(procTable[pid].startArg);
    P1_Quit(rc);
}


/* ------------------------------------------------------------------------
 * Name:        P1_Fork 
 * Purpose:     Creates and initializes a new process.  Updates globals to reflect new state
 * Parameter:   char *name - Name of the new function
 *              int (*func)(void *) - The first function the new process executes
 *              void *arg - Arguments for the first function
 *              int stackSize - size of the process stack [Must be > USLOSS MIN STACK SIZE]
 *              int priority - Priority of the new process. [Must be > parent priority (M1)]
 * Returns:     int pid - 
 *                  -3: Invalid Priority
 *                  -2: Stacksize less than USLOSS_MIN_STACK
 *                  -1: No more processes
 *                 >=0: PID of created process
 * Side Effects:ReadyList Modified  
 *              Process Added to process table
 *              Current Process (may) change
 * ----------------------------------------------------------------------- */
int P1_Fork(char *name, int(*f)(void *), void *arg, int stacksize, int priority) {
    if(DEBUG ==3) USLOSS_Console("P1_Fork(): Entered P1_Fork\n");
    int newPID = -1;
 //--Check if in Kernel Mode
    if(DEBUG >= 2) USLOSS_Console("P1_Fork(): Check for kernel mode\n");
    if(!isKernel()) {
        USLOSS_Console("ERROR: Not in kernel mode. Exiting...\n");
        USLOSS_Halt(1);
    }
 //--Disable interrupts
 //--Validate input
 //----Valid Priority
    if(DEBUG == 3) USLOSS_Console("P1_Fork(): Checking priority\n");
    if(priority < MAXPRIORITY || priority > MINPRIORITY)
        return -3;
 //----Valid Stack Size
    if(DEBUG == 3) USLOSS_Console("P1_Fork(): Checking Stack Size\n");
    if(stacksize < USLOSS_MIN_STACK) 
        return -2;
 //----Enough space for new process
 //----Iterate through process table looking for empty spot
    int i;
    for(i = 0; i < P1_MAXPROC; i++) {
     //--If a spot is empty, set that spot to new PID and exit loop
        if(procTable[i].status == NO_PROCESS) {
            newPID = i;
            break;
        }
    }
    if(newPID == -1) 
        return -1;
    if(DEBUG == 3) USLOSS_Console("P1_Fork(): New pid is: %d\n",newPID);
 //----(name/func cant be null??)
    
 //--Initialize PCB
    if(DEBUG >= 2) USLOSS_Console("P1_Fork(): Intializing new PCB\n");
 //--Update Parent PCB to reflect new child
    procTable[newPID].startFunc     = f;
    procTable[newPID].startArg      = arg;
    procTable[newPID].name          = name;
    procTable[newPID].PID           = newPID;
    procTable[newPID].status        = READY;
    procTable[newPID].priority      = priority;
    procTable[newPID].time_start    = 0;
    procTable[newPID].cyclesUsed    = 0;
    procTable[newPID].parentPID     = P1_GetPID();
    procTable[newPID].children      = create_list();
 //--Allocate Stack
    if(DEBUG >= 2) USLOSS_Console("P1_Fork(): Allocating space for stack\n");
    procTable[newPID].stack = malloc(stacksize);
 //--Initialize context
    if(DEBUG >= 2) USLOSS_Console("P1_Fork(): Calling USLOSS_ContextInit()\n");
    USLOSS_ContextInit(&(procTable[newPID].context), USLOSS_PsrGet(), procTable[newPID].stack, stacksize, launch);
 //--Add to parents child list
    if(DEBUG == 3) USLOSS_Console("P1_Fork(): Adding new process to parent's child list\n");
    if(P1_GetPID() > -1)
        append_list(procTable[P1_GetPID()].children, newPID);
 //--Add to readylist
    if(DEBUG == 3) USLOSS_Console("P1_Fork(): Adding new process to ready list\n");
    insert(readyList,newPID, priority);
 //--call dispatcher
    if(DEBUG >= 2) USLOSS_Console("P1_Fork(): Calling dispatcher()\n");
    dispatcher();
 //--Enable interrupts
 //--Return PID
    return newPID;
}
/* ------------------------------------------------------------------------
 * Name:        P1_Quit 
 * Purpose:     Called by a process on itself.  Kills itself and returns status to parent.
 * Parameter:   int status - The return status of the process
 * Returns:     none
 * Side Effects:ReadyList modified
 *              Process Table modified
 *              Current Process changes
 * ----------------------------------------------------------------------- */
void P1_Quit(int status) {
    if(DEBUG == 3) USLOSS_Console("P1_Quit(): Entered P1_Quit\n");
//--Check in kernel mode
    if(!isKernel()) {
        USLOSS_Console("ERROR: Not in kernel mode!\n");
        USLOSS_Halt(1);
    }
//--Disable interrupts
//--Iterate through children
    if(DEBUG == 3) USLOSS_Console("P1_Quit(): Setting all children's parent pid to -1\n");
    Node *node = procTable[P1_GetPID()].children->first;
    while(node->next != NULL) {
//----Set parent PID to empty
        procTable[node->pid].parentPID = -1;
    }
//--Clean up proc table entry
    if(DEBUG >= 2) USLOSS_Console("P1_Quit(): Process finished withs status: %d\n",status);
    procTable[P1_GetPID()].return_status = status;
    procTable[P1_GetPID()].status = FINISHED;

    int currentProcParentPID = procTable[P1_GetPID()].parentPID;
//--Remove then append process to parent's child block, making it so that the first process to 
//--quit will be the first process with a status of quit it finds. 

    remove_node(procTable[currentProcParentPID].children, P1_GetPID());
    append_list(procTable[currentProcParentPID].children, P1_GetPID());

//--Unblock parent (if blocked)
    if(procTable[currentProcParentPID].status == BLOCKED){
        procTable[currentProcParentPID].status = READY;
        insert(readyList,currentProcParentPID, procTable[currentProcParentPID].priority);}
//--Call dispatcher
    if(DEBUG >= 2) USLOSS_Console("P1_Quit(): Calling dispatcher()\n");
    dispatcher();
//--Enable interrupts
}
/* ------------------------------------------------------------------------
 * Name:        P1_Kill 
 * Purpose:     Used by a process to kill another process
 * Parameter:   int PID - Process to be killed
 *              int status - return status of the killed process
 * Returns:     int result - 
 *                  -2: PID belongs to current process
 *                  -1: Invalid PID
 *                   0: Successfull Kill
 * Side Effects:ReadyList, Process Table modified
 * ----------------------------------------------------------------------- */
int P1_Kill(int PID, int status) {

    if(DEBUG == 3) USLOSS_Console("P1_Kill(): Entered P1_Kill\n");
//--Check in kernel mode
    if(!isKernel()) {
        USLOSS_Console("ERROR: Not in kernel mode!\n");
        USLOSS_Halt(1);
    }

//--Check if PID is Current Process
    if(PID == P1_GetPID()){
        return -2;
    }

//--Check if PID is valid
    if(procTable[PID].status == INVALID){
        return -1;
    }

//--Disable interrupts
//--Iterate through children
    if(DEBUG == 3) USLOSS_Console("P1_Kill(): Setting all children's parent pid to -1\n");

    Node *node = procTable[PID].children->first;

    while(node->next != NULL) {
//----Set parent PID to empty
        procTable[node->pid].parentPID = -1;
    }
//--Clean up proc table entry
    if(DEBUG >= 2) USLOSS_Console("P1_Kill(): Process finished withs status: %d\n",status);
    procTable[PID].return_status = status;
    procTable[PID].status = FINISHED;

    int currentProcParentPID = procTable[P1_GetPID()].parentPID;
//--Remove then append process to parent's child block, making it so that the first process to 
//--quit will be the first process with a status of quit it finds. 

    remove_node(procTable[currentProcParentPID].children, P1_GetPID());
    append_list(procTable[currentProcParentPID].children, P1_GetPID());

//--Unblock parent (if blocked)
    if(currentProcParentPID >= 0){
        if(procTable[currentProcParentPID].status == BLOCKED){
            procTable[currentProcParentPID].status = READY;
            insert(readyList,currentProcParentPID, procTable[currentProcParentPID].priority);}
    }
//--Call dispatcher
    if(DEBUG >= 2) USLOSS_Console("P1_Kill(): Calling dispatcher()\n");
    dispatcher();

    return 0;
}
/* ------------------------------------------------------------------------
 * Name:        P1_Join
 * Purpose:     Syncronizes termination of a child with its parent.  When called the parent
 *                  is blocked until one of its children exits.
 * Parameter:   int *status - Location to store return status of child
 * Returns:     int pid:
 *                  -1: Process has no children
 *                 >=0: PID of child that quit
 * Side Effects:*status is updated to reflect child return status
 *              parent process is blocked
 * ----------------------------------------------------------------------- */
int P1_Join(int *status) {
 
    if(DEBUG == 3) USLOSS_Console("P1_Join(): Entered P1_Join\n");
//--Check in kernel mode
    if(!isKernel()) {
        USLOSS_Console("ERROR: Not in kernel mode!\n");
        USLOSS_Halt(1);
    }
//--Disable interrupts
//--Validate input
//----Make sure process has kids
    Node *node = procTable[P1_GetPID()].children->first;

    if(node->next == NULL){
        procTable[P1_GetPID()].status = BLOCKED;
        return -1;
    }
    else{
        //--Get !first! quit child
        while(node->next != NULL){
            if(procTable[node->pid].status == FINISHED){
                //Not sure what to do, but this is when it finds its first quit child
                //Something about quitting the child or parent or both, I'm not sure.
                status = procTable[node->pid].return_status;
                return node->pid;
            }
            node = node->next;
        }
        //----If no quit child, block until child returns
        procTable[P1_GetPID()].status = BLOCKED;

    }

//--Clean up child process
//--Enable interrupts
//--Return child PID
    return -1;
}
/* ------------------------------------------------------------------------
 * Name:        P1_getPID 
 * Purpose:     Returns the PID of the currently running process
 * Parameter:   none
 * Returns:     int pid - PID of current process
 * Side Effects:none
 * ----------------------------------------------------------------------- */
int P1_GetPID() { return currentPID; }
/* ------------------------------------------------------------------------
 * Name:        P1_DumpProcesses 
 * Purpose:     Prints all processes' PCB in a readable format
 * Parameter:   none
 * Returns:     none
 * Side Effects:none
 * ----------------------------------------------------------------------- */
void P1_DumpProcesses() {
    if(DEBUG == 3) USLOSS_Console("P1_DumpProcesses(): Entering DumpProcesses\n");
    int i;
    int runningProcesses = 0;

    USLOSS_Console("------------------------------Process Dump---------------------------------\n");
    USLOSS_Console("%10s%10s%10s%10s%10s%10s%15s\n","Name","PID","PPID","Priority","Status","Children","Clock Cycles");
    for(i = 0; i < P1_MAXPROC; i++) {
        if(procTable[i].status != NO_PROCESS) {
            runningProcesses++;
            PCB proc = procTable[i];
            USLOSS_Console("%10s%10d%10d%10d%10d%10d%15d\n",proc.name,proc.PID,proc.parentPID,proc.priority,proc.status,-1,proc.cyclesUsed);
        }
    }
     USLOSS_Console("---------------------------------------------------------------------------\n");   
     USLOSS_Console("There are %d running processes.\n",runningProcesses);

     if(DEBUG == 3) USLOSS_Console("P1_DumpProcesses(): Leaving DumpProcessesi\n");
}
/* ------------------------------------------------------------------------
 * Name:        sentinel
 * Purpose:     Serves as holding pattern for OS when no other processes to run
 *              Also detects and report simple deadlock states
 * Parameter:   None
 * Returns:     Nothing
 * Side Effects:If in deadlock, halt
 * ----------------------------------------------------------------------- */
int sentinel(void *notused) {
    while(numberProcs > 1) {
    //--Check for deadlock (NOT IMPLEMENTED)
    //--Commented out since interrupts are not implemented
        //USLOSS_WaitInt();
    }
    USLOSS_Halt(0);
    return(0);
}

/* ------------------------------------------------------------------------
 * Name:        isKernel 
 * Purpose:     Detects if the OS is in Kernel Mode or User Mode
 * Parameter:   None
 * Returns:     0: OS is in kernel mode
 *             !0: OS is in user mode
 * Side Effects:None
 * ----------------------------------------------------------------------- */
int isKernel() {
    return (USLOSS_PSR_CURRENT_MODE & USLOSS_PsrGet());
}




/* ------------------------------------------------------------------------
 * Name: 
 * Purpose:
 * Parameter:
 * Returns:
 * Side Effects:
 * ----------------------------------------------------------------------- */


void dispatcher() {

    if(DEBUG == 3) USLOSS_Console("dispatcher(): Entered Dispatcher\n");

    if(!isKernel()) {
      USLOSS_Console("ERROR: Not in kernel mode. Exiting...\n");
      USLOSS_Halt(1);
    }

    node *next = pop(readyList);

    /* First process */
    if(currentPID < 0){
      currentPID = next->pid;
      USLOSS_ContextSwitch(NULL, &procTable[currentPID].context);
    }
    /* Sentinel process */
    else if(procTable[next->pid].priority == 6){
        insert(readyList,next->pid, 6);
    }
    /*Current process is not yet finished, but dispatcher wants to run new process*/
    else if(procTable[currentPID].status == READY){

        /*Current process is lower priority than next process*/
        if(procTable[currentPID].priority > next->priority){
            insert(readyList,currentPID, procTable[currentPID].priority);//Add current process back to readyList
            USLOSS_ContextSwitch(&procTable[currentPID].context, &procTable[next->pid].context); //switch context
            currentPID = next->pid; //set pid to new running pid
        }
        /*Current process is higher priority than next process*/
        else{
            insert(readyList, next->pid, next->priority);
        }
    }
    /*Current process is no longer READY*/
    else{
      USLOSS_ContextSwitch(&procTable[currentPID].context, &procTable[next->pid].context); //switch context
      currentPID = next->pid; //set pid to new running pid
    }

}
