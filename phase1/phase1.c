#include <stddef.h>
#include <stdio.h>
#include "usloss.h"
#include "phase1.h"

typedef struct PCB {
    USLOSS_Context context;     /*Context*/
    int (*startFunc)(void *);   /*Starting Function*/
    void *startArg              /*Argument to starting function*/
    unsigned int priority;      /*Priority at which process runs*/
    unsigned int start_time;    /*Time when the process started*/
    unsigned int cycles_used;   /*Number of CPU Clock Cycles Used*/
    unsigned int status;        /*Current status of process (running, blocked, etc)*/
} PCB;

void startup() {
//--Init Process Table
//--Init Ready List, Blocked list, etc.
//--Init Interrup Vector
//--Init Semaphores
//--Init Sentinel Process
//--Start P2_Startup


}
void finish() {

}
