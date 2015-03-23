/*
test case for P1_Kill() on a blocked process
output should be:    (PIDs may different, which depends on implementation)

---------Starting Test 9 ----------
Child PID created 2 
Child PID created 3 
start child_p
start killer
kill success
end killer
end first 2 (should be 2)        status 444 (should be 444)
end second 3 (should be 3)        status -4 (should be -4)
---------Ending Test 9 ----------

*/
#include "phase1.h"
#include <stdio.h>

P1_Semaphore sem;

int child_p(void *arg){
	USLOSS_Console("start child_p\n");

	P1_P(sem);
	//P1_DumpProcesses();	
	USLOSS_Console("end child_p\n");	
	return 666;
}


int killer(void *arg){
	USLOSS_Console("start killer\n");
	//P1_DumpProcesses();
	int *pid = (int *)arg;
	int status = P1_Kill(*pid, 444);
	if(status == 0){
			USLOSS_Console("kill success\n");
	}else{
		USLOSS_Console("Kill failed!!\n");
		//USLOSS_Halt(1);
	}
	//P1_DumpProcesses();
	USLOSS_Console("end killer\n");
	return -4;
}


int P2_Startup(void *arg){
	
	USLOSS_Console(" \n---------Starting Test 9 ----------\n");
		
	sem = P1_SemCreate(0);  
	
	int childpid1, child_status1, exit_childpid1;
	int childpid2, child_status2, exit_childpid2;


	childpid1 = P1_Fork("child_p", child_p, NULL, USLOSS_MIN_STACK * 4, 2);
  	USLOSS_Console("Child PID created %d \n", childpid1);	

	childpid2 = P1_Fork("killer", killer, &childpid1, USLOSS_MIN_STACK * 4, 3);
  	USLOSS_Console("Child PID created %d \n", childpid2);

	exit_childpid1 = P1_Join(&child_status1);	

	exit_childpid2 = P1_Join(&child_status2);


	USLOSS_Console("end first %d (should be %d)        status %d (should be 444)\nend second %d (should be %d)        status %d (should be -4)\n",
			exit_childpid1,childpid1,child_status1,exit_childpid2,childpid2, child_status2);
		

	USLOSS_Console(" ---------Ending Test 9 ----------\n");
	return 0;
}
