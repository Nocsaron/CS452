#include <stdio.h>	
#include "phase1.h"
#include <malloc.h>
#include <fcntl.h>


/*  should print : (Check the term3.out file, it should have 'W' written into it)

---------Starting Test 17 ----------
Xmit status for term 3 is READY --> Its ok now to Send to the terminalXmit status for term 3 is READY --> Check the corresponding terminal file term3.out to see if the I/O succeeded 
 ---------Ending Test 17 ----------
 
 */ 

	
int P2_Startup(void *arg) {
	
	USLOSS_Console(" \n---------Starting Test 17 ----------\n");
	int status;
	int unit = 3;
	USLOSS_DeviceInput(USLOSS_TERM_DEV, unit, &status);
	if(USLOSS_TERM_STAT_XMIT(status) == USLOSS_DEV_READY) {
		USLOSS_Console("Xmit status for term %d is READY --> Its ok now to Send to the terminal", unit); 
	}
	else
		P1_Quit(0);
	
	int ctrl = 0;
	char ch = 'W';
	  
	ctrl = USLOSS_TERM_CTRL_XMIT_INT(ctrl);
	ctrl = USLOSS_TERM_CTRL_CHAR(ctrl, ch);
	ctrl = USLOSS_TERM_CTRL_XMIT_CHAR(ctrl);
	USLOSS_DeviceOutput(USLOSS_TERM_DEV, unit, ctrl);
	P1_WaitDevice(USLOSS_TERM_DEV, unit, &status); // Wait for the interrupt to occur
	if(USLOSS_TERM_STAT_XMIT(status) == USLOSS_DEV_READY) {
		USLOSS_Console("Xmit status for term %d is READY --> Check the corresponding terminal file term3.out to see if the I/O succeeded \n", unit); 	
	}
int    fd = open( "term3.out", O_RDONLY );
    char buf[0];
    read(fd, buf, 1);
    if(buf[0] == 'W')
        USLOSS_Console("W \n");
		USLOSS_Console(" ---------Ending Test 17 ----------\n");
	P1_Quit(0);
}


