/*------------------------------------------------------------------------
 * CS452 - Phase 1 Operating System
 * Author: N R Callahan, Noah Sleiman
 *
 * ----------------------------------------------------------------------*/


#include <stddef.h>
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

/* -------------------------- Globals ------------------------------------- */

typedef struct {
    USLOSS_Context      context;                /* State of process */
    int                 (*startFunc)(void *);   /* Starting Function */
    void                *startArg;              /* Argument to starting function */
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
    int newPid;
 //--Check if in Kernel Mode
 //--Disable interrupts
 //--Validate input
 //----Valid Priority
 //----Valid Stack Size
 //----Enough space for new process
 //----(name/func cant be null??)
 
 //--Find empty space in process table
 //--Initialize PCB
 //--Update Parent PCB to reflect new child
 //--Initialize context
 //--Add to readylist
 //--call dispatcher
 //--Enable interrupts
 //--Return PID
    return newPid;
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
//--Check in kernel mode
//--Disable interrupts
//--Iterate through children
//----Set parent PID to empty if running
//----Delete child if quit
//--Clean up proc table entry
//--Unblock parent (if blocked)
//--Call dispatcher
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
    int returnCode;

    return returnCode;
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
    int PID;
//--Check in kernel mode
//--Disable interrupts
//--Validate input
//----Make sure process has kids
//--Get !first! quit child
//----If no quit child, block until child returns
//--Clean up child process
//--Enable interrupts
//--Return child PID
    return PID;
}
/* ------------------------------------------------------------------------
 * Name:        P1_getPID 
 * Purpose:     Returns the PID of the currently running process
 * Parameter:   none
 * Returns:     int pid - PID of current process
 * Side Effects:none
 * ----------------------------------------------------------------------- */
int P1_GetPID() {
    int PID;

    return PID;
}
/* ------------------------------------------------------------------------
 * Name:        P1_DumpProcesses 
 * Purpose:     Prints all processes' PCB in a readable format
 * Parameter:   none
 * Returns:     none
 * Side Effects:none
 * ----------------------------------------------------------------------- */
void P1_DumpProcesses() {

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
 * Name: 
 * Purpose:
 * Parameter:
 * Returns:
 * Side Effects:
 * ----------------------------------------------------------------------- */



