/*
  Test case for Semaphores. Tests synchronization combinations using semaphores
  
 Should print:
 
---------Starting Test 6 ----------
start phase2_start
join kernel_start (1)
start XXp1
exit status1 = -3
join kernel_start (2)
start XXp2
join XXp2
start XXp3
exit status3 = 0
exit status2 = 5
P(sem2)
start XXp4
V(sem2)
V(sem1)
join kernel_start (3)
P(sem1)
exit status4 = 7
 ---------Ending Test 6 ----------
	
*/
  


#include "phase1.h"
#include <stdio.h>


void XXp1(void *), XXp2(void *), XXp3(void *), XXp4(void *);
char buf[256];

P1_Semaphore sem1, sem2;

int P2_Startup(void *arg) {

  USLOSS_Console(" \n---------Starting Test 6 ----------\n");
  int status, pid1, pid2,i=0;

  USLOSS_Console("start phase2_start\n");
  sem1 = P1_SemCreate(0);
  sem2 = P1_SemCreate(0);
  //while(i<5){
    //i++;
    pid1 = P1_Fork("XXp1",XXp1, "XXp1", USLOSS_MIN_STACK * 4,1);
    /*print_proc(2);*/
  //}
  pid2 = P1_Fork("XXp2",XXp2, "XXp2", USLOSS_MIN_STACK * 4,2);
  USLOSS_Console("join kernel_start (1)\n");
  //while(i+3){
    //i--;
    P1_Join(&status);
  //}
  sprintf(buf,"exit status1 = %d\n", status); USLOSS_Console(buf);
  USLOSS_Console("join kernel_start (2)\n");
  P1_Join(&status);
  sprintf(buf,"exit status2 = %d\n", status); USLOSS_Console(buf);
  pid1 = P1_Fork("XXp4",XXp4, "XXp4", USLOSS_MIN_STACK * 4, 3);
  USLOSS_Console("P(sem2)\n");
  P1_P(sem2);
  USLOSS_Console("V(sem1)\n");
  P1_V(sem1);
  USLOSS_Console("join kernel_start (3)\n");
  P1_Join(&status);
  sprintf(buf,"exit status4 = %d\n", status); USLOSS_Console(buf);
  
  USLOSS_Console(" ---------Ending Test 6 ----------\n");
  P1_Quit(0);
}

void XXp1(void *arg)
{
  int i;

  USLOSS_Console("start %s\n", (char *) arg);
  for(i=0; i<100; i++) ;
  P1_Quit(-3);
}

void XXp2(void *arg)
{
  int status, pid;

  USLOSS_Console("start %s\n", (char *) arg);
  pid = P1_Fork( "XXp3", XXp3, "XXp3", USLOSS_MIN_STACK * 4,4);  
  USLOSS_Console("join XXp2\n");
  P1_Join(&status);
  sprintf(buf,"exit status3 = %d\n", status); USLOSS_Console(buf);
  //P1_Join(&status);
  //sprintf(buf,"exit status3 = %d\n", status); USLOSS_Console(buf);
  P1_Quit(5);
}

void XXp3(void *arg)
{
  int i;
  USLOSS_Console("start %s\n", (char *) arg);
  for(i=0; i<5000000; i++) ;
  P1_Quit(0);
}

void XXp4(void *arg)
{
  USLOSS_Console("start %s\n", (char *) arg);
  USLOSS_Console("V(sem2)\n");
  P1_V(sem2);
  USLOSS_Console("P(sem1)\n");
  P1_P(sem1);
  P1_Quit(7);
}
