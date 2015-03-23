#include <stdio.h>	
#include "phase1.h"
#include <malloc.h>

/* This should print the first letter in the terminal file term0.in 

If term0.in had the word "hello" in it, then the O/P should resemble

---------Starting Test 18 ----------
h 
---------Ending Test 18 ----------

*/
	
int P2_Startup(void *arg) {
	
	USLOSS_Console(" \n---------Starting Test 18 ----------\n");
	int	status;
	char ch;
	
    status = USLOSS_DeviceOutput(USLOSS_TERM_DEV, 0, (void *) USLOSS_TERM_CTRL_RECV_INT(0));
 
    P1_WaitDevice(USLOSS_TERM_DEV, 0, &status); // Wait for the interrupt to occur
    if (USLOSS_TERM_STAT_RECV(status) == USLOSS_DEV_BUSY) {
		ch = USLOSS_TERM_STAT_CHAR(status);	// Get the character
		USLOSS_Console("%c \n", ch);
    }
	
	USLOSS_Console(" ---------Ending Test 18 ----------\n");
	P1_Quit(0);
}


