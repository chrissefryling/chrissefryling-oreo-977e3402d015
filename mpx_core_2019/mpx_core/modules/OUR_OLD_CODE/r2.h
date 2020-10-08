/** @file r2.h
*/
#pragma once

#include "r1.h"
#include "r3.h"
#include "mpx_supt.h"
#include "polling.h"
#include <system.h>
#include <stdbool.h>
/*
process state:
- 2 is represented running
- 1 is represented ready
- 0 is represented block
*/
#define RUNNING 2
#define READY   1
#define BLOCKED 0

/*
process class:
- 100 application
- 101 system
*/
#define APP 100
#define SYS 101

#define SUSPENDED 1
#define NOT_SUSPENDED 0

//main queue 
typedef struct queue {
	int count;
	struct PCB *head;
	struct PCB *tail;
}queue;

//PCB 
typedef struct PCB{
	char name[16];
	unsigned int classtype;
	unsigned int priority;
	unsigned int state;
    bool isSuspended;
	unsigned char *stacktop;
    unsigned char stackbase[2048];
	struct PCB *next;
	struct PCB *previous;
}PCB;


//struct queue *ready_q, *block_q;


PCB* allocatePCB();
int freePCB(PCB*);
PCB* setupPCB(char*,unsigned int, unsigned int);
PCB* findPCB(char*);
void insertPCB(PCB*);
int removePCB(PCB*);
PCB *createPCB(char*,unsigned int, unsigned int);
void deletePCB(char*);
void delete_inf(char *name);
void blockPCB(char*);
void unblockPCB(char*);
void suspendPCB(char*);
void resumePCB(char*);
void setPriority(char*,unsigned int);
void showPCB(char*);
void showReady(void);
void showBlocked(void);
void showAll(void);
