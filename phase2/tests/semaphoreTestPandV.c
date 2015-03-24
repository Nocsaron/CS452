/*
 * semaphoreTestPandV.c
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
    
    Sys_SemP(handle);
    
    USLOSS_Console("PatientProc1 clears P on semaphore\n");
    
    Sys_SemV(handle);
    
    USLOSS_Console("PatientProc1 V's semaphore\n");
    return 5;
}

int PatientProc2(void *arg) {
    int handle = *((int *) arg);
    USLOSS_Console("PatientProc2 created\n");
    
    Sys_SemP(handle);
    
    USLOSS_Console("PatientProc2 clears P on semaphore with initial value 2\n");
    USLOSS_Console("PatientProc2 DOES NOT V on semaphore with initial value 2\n");
    
    return 5;
}


int P3_Startup(void *arg) {
    // Test 1
	USLOSS_Console("Testing usage of semaphore\n");
    USLOSS_Console("------------------------\n");
    USLOSS_Console("Testing P3_Startup by itself\n");
    
    int semaphore; // Necessary only because of args specified by function prototype
    int handle = Sys_SemCreate(1, &semaphore);
    assert(semaphore >= 0);
    assert(handle == 0);
    USLOSS_Console("Semaphore created with initial value 1\n");
    
    int result = Sys_SemP(semaphore);
    assert(result == 0);
    USLOSS_Console("P3_Startup clears P on semaphore\n");
    
    result = Sys_SemV(semaphore);
    assert(result == 0);
    USLOSS_Console("P3_Startup V's semaphore\n");
    USLOSS_Console("Test cleared!\n");
    USLOSS_Console("------------------------\n");
    
    // Test 2
    // Note that spawned proc has higher priority than P3_Startup
    // to ensure that it P's and V's on the same semaphore
    USLOSS_Console("Testing P3_Startup on mutex with another proc\n");
    int pid;
    result = Sys_Spawn("PatientProc1", PatientProc1, (void *) &semaphore, USLOSS_MIN_STACK, 2, &pid);
    assert(pid != -1);
    assert(result == 0);
    
    result = Sys_SemP(semaphore);
    assert(result == 0);
    USLOSS_Console("P3_Startup clears P on semaphore again\n");
    
    result = Sys_SemV(semaphore);
    assert(result == 0);

    USLOSS_Console("P3_Startup V's semaphore\n");
    USLOSS_Console("Test cleared!\n");
    USLOSS_Console("------------------------\n");
    
    // Test 3
    USLOSS_Console("Testing P3_Startup on semaphore with initial value 2 with another proc\n");
    handle = Sys_SemCreate(2, &semaphore);
    assert(semaphore >= 0);
    assert(handle == 0);
    USLOSS_Console("Semaphore created with initial value 2\n");
    
    result = Sys_Spawn("PatientProc2", PatientProc2, (void *) &semaphore, USLOSS_MIN_STACK, 2, &pid);
    assert(pid != -1);
    assert(result == 0);
    
    result = Sys_SemP(semaphore);
    assert(result == 0);

    USLOSS_Console("P3_Startup clears P on semaphore with initial value 2\n");
    
    result = Sys_SemV(semaphore);
    result = Sys_SemV(semaphore);
    assert(result == 0);
    USLOSS_Console("P3_Startup V's semaphore with initial value 2 twice\n");
    USLOSS_Console("Test cleared!\n");
    USLOSS_Console("------------------------\n");
    
	USLOSS_Console("You passed the test! Treat yourself to a cookie!\n");
	return 7;
}
