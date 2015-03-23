/*
 * These are the definitions for phase1 of the project (the kernel).
 */

#ifndef _PHASE1_H
#define _PHASE1_H

#include "usloss.h"
#include "usyscall.h"

/*
 * Maximum number of processes. 
 */

#define P1_MAXPROC	50

/*
 * Maximum number of semaphores.
 */

#define P1_MAXSEM	800

typedef void *P1_Semaphore;

#define READY 0
#define FINISHED 1
#define RUNNING 2
#define NO_PROCESS 3


/* 
 * Function prototypes for this phase.
 */

extern  int             P1_Fork(char *name, int(*func)(void *), void *arg, 
        int stackSize, int priority);
extern	int		P1_Join(int *status);
extern	void		P1_Quit(int status);
extern  int		P1_Kill(int pid, int status);

extern	P1_Semaphore	P1_SemCreate(unsigned int value);
extern	void		P1_SemFree(P1_Semaphore sem);
extern	void		P1_P(P1_Semaphore sem);
extern	void		P1_V(P1_Semaphore sem);

extern	int		P1_WaitDevice(int type, int unit, int *status);
extern	int		P1_ReadTime(void);

extern	int		P1_GetPID(void);
extern  void		P1_DumpProcesses(void);

extern	int		P2_Startup(void *arg);

void dispatcher();
void disableInterrupts();

extern void (*syscalls_vec[USLOSS_MAX_SYSCALLS])(USLOSS_Sysargs *);

#endif /* _PHASE1_H */
