/*
 * semaphoreTestInvalidUsage.c
 *
 *  Created on: Mar 22, 2015
 *      Author: tishihara
 */

#include <string.h>
#include <stdlib.h>
#include <usloss.h>
#include <phase1.h>
#include <phase2.h>
#include <assert.h>
#include <libuser.h>

int PatientProc1(void *arg) {
    int handle = *((int *) arg);
    USLOSS_Console("PatientProc1 created\n");
    
    int result = Sys_SemP(handle);
    assert(result == 0);
    
    USLOSS_Console("PatientProc1 clears P on semaphore with initial value 1\n");
    USLOSS_Console("PatientProc1 DOES NOT V on semaphore with initial value 1\n");
    
    return 5;
}

int PatientProc2(void *arg) {
    int handle = *((int *) arg);
    USLOSS_Console("PatientProc2 created\n");
    USLOSS_Console("PatientProc2 waits on semaphore with initial value 1\n");
    
    int result = Sys_SemP(handle);
    assert(result == 0);
    USLOSS_Console("PatientProc2 clears P on semaphore with initial value 1\n");
    
    result = Sys_SemV(handle);
    assert(result == 0);
    USLOSS_Console("PatientProc2 V's semaphore with initial value 1\n");
    
    return 5;
}

int P3_Startup(void *arg) {
	USLOSS_Console("Testing usage of semaphore\n");
    USLOSS_Console("------------------------\n");
    USLOSS_Console("Testing creation of semaphore with negative initial value\n");
    
    int semaphore;
    int handle = Sys_SemCreate(-5, &semaphore);
    assert(handle == -1);
    
    USLOSS_Console("Assertion passed!\n");
    USLOSS_Console("------------------------\n");
    
    handle = Sys_SemCreate(1, &semaphore);
    assert(semaphore >= 0);
    assert(handle == 0);
    
    USLOSS_Console("Binary semaphore created for testing\n");
    
    int pid;
    int result;
    result = Sys_Spawn("PatientProc1", PatientProc1, (void *) &semaphore, USLOSS_MIN_STACK, 2, &pid);
    assert(pid != -1);
    assert(result == 0);
    
    result = Sys_Spawn("PatientProc2", PatientProc2, (void *) &semaphore, USLOSS_MIN_STACK, 2, &pid);
    assert(pid != -1);
    assert(result == 0);
    
    int freeResult = Sys_SemFree(semaphore);
    assert(freeResult == 1);
    
    USLOSS_Console("Returned appropriate value when trying to free semaphore with blocked procs\n");
    
    int vResult = Sys_SemV(semaphore);
    assert(vResult == 0);
    
    USLOSS_Console("P3_Startup V's semaphore once to clear PatientProc1\n");
    Sys_Wait(&vResult, &freeResult);
    Sys_Wait(&vResult, &freeResult);
    freeResult = Sys_SemFree(semaphore);
    USLOSS_Console("%d\n", freeResult);
    assert(freeResult == 0);
    
    USLOSS_Console("Semaphore freed properly when no procs waiting\n");
    
    result = Sys_SemFree(500);
    assert(result == -1);
    
    USLOSS_Console("Returned appropriate value when given invalid semaphore handle\n");
    
	USLOSS_Console("You passed the test! Treat yourself to a cookie!\n");
	return 7;
}
