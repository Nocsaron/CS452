#include <stdio.h>
#include <usloss.h>
#include <phase2.h>
#include <libuser.h>
#include <assert.h>

int child(void *arg) {
   USLOSS_Console("IN CHILD\n");
   return 0;
}
int P3_Startup(void *arg) {
    int pid;
    int return_code = Sys_Spawn("child",child,NULL,USLOSS_MIN_STACK,4,&pid);
    USLOSS_Console("Spawned child\n");
}
