#include <stdlib.h>
#include <usloss.h>
#include <phase1.h>
#include <phase2.h>

/*
 * CSC452 - Operating Systems
 * Author: N R Callahan, Noah Sleiman
 * Phase2
 */


/*Milestone Requirements  
 *  Item                        Status
 *  Clock Device Driver         Incomplete (total)
 *  Sys_Spawn                   Incomplete (total)
 *  Mailbox System Calls        Incomplete (total)
 */
int P2_Startup(void *arg) {

}



///////////////////////////////////////////////////////
//                 2.1   Clock Driver                //
///////////////////////////////////////////////////////

int P2_Sleep(int seconds) {

}


///////////////////////////////////////////////////////
//                 2.2  Terminal Driver              //
///////////////////////////////////////////////////////

int P2_TermRead(int unit, int size, char *buffer) {

}

int P2_TermWrite(int unit, int size, char *text) {

}

///////////////////////////////////////////////////////
//                 2.3   Disk Driver                 //
///////////////////////////////////////////////////////

int P2_DiskRead(int unit, int track, int first, int sectors, void *buffer) {

}

int P2_DiskWrite(int unit, int track, int first, int sectors, void *buffer) {

}

int P2_DiskSize(int unit, int *sector, int *track, int*disk) {

}

///////////////////////////////////////////////////////
//                 3.x  System Calls                 //
///////////////////////////////////////////////////////

//3.1   Sys_TermRead(syscall 1)
//3.2   Sys_TermWrite(syscall 2)
//3.3   Sys_Spawn(syscall 3)
//3.4   Sys_Wait(syscall 4)
//3.5   Sys_Terminate(syscall 5)
//3.6   Sys_MboxCreate(syscall 6)
//3.7   Sys_MboxRelease(syscall 7)
//3.8   Sys_MboxSend(syscall 8)
//3.9   Sys_MboxReceive(syscall 9)
//3.10  Sys_MboxCondSend(syscall 10)
//3.11  Sys_MboxCondReceive(syscall 11)
//3.12  Sys_Sleep(syscall 12)
//3.13  Sys_DiskRead(syscall 13)
//3.14  Sys_DiskWrite(syscall 14)
//3.15  Sys_DiskSize(syscall 15)
//3.16  Sys_SemCreate(syscall 16)
//3.17  Sys_SemP(syscall 17)
//3.18  Sys_SemV(syscall 18)
//3.19  Sys_SemFree(syscall 19)
//3.20  Sys_GetTimeofDay(syscall 20)
//3.21  Sys_CPUTime(syscall 21)
//3.22  Sys_GetPID(syscall 22)
