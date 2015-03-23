#include <stdio.h>	
#include "phase1.h"
#include <malloc.h>

/* Test Case to test Basic Interrupt Handling, Disk interrupt Handler, Disk Device Semaphore using Disk Seek

Output:
	
---------Starting Test 14 ----------
status from WaitDevice is 0
Request Completed Succesfully 
 ---------Ending Test 14 ----------

*/ 

	
int P2_Startup(void *arg) {
	
	USLOSS_Console(" \n---------Starting Test 14 ----------\n");
	
	// To Seek to a particular track in the  disk
	int status;
	
	USLOSS_DeviceRequest *req = (USLOSS_DeviceRequest *)malloc(sizeof(USLOSS_DeviceRequest)); //Needed to pass to USLOSS_DeviceOutput
	req->opr = USLOSS_DISK_SEEK; // Fill the request indicating the operation to be done on the disk
	req->reg1 = 2; // Track to be moved. This assumes the disk has atleast 3 tracks.
	USLOSS_DeviceOutput(USLOSS_DISK_DEV, 1, req); // This assumes Disk1 has been created as indicated in the comment section above.
								//	Move to Disk, unit 1, Track 2, Request obj..that contains all the information needed to be done on the Disk.
	P1_WaitDevice(USLOSS_DISK_DEV, 1, &status); // Wait for the interrupt to occur
	USLOSS_Console("status from WaitDevice is %d \n", status);// Retrieve the status
	if(status == USLOSS_DEV_READY) // For Disk device, this implies the request completed
		USLOSS_Console("Request Completed Succesfully \n");

	USLOSS_Console(" ---------Ending Test 14 ----------\n");
	P1_Quit(0);
}
