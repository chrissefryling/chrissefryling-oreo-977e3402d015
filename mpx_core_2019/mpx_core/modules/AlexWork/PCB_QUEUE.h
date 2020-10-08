/*
	Created by Alex Wilson 4/7/2020
	Used to replace R2, R3, and R4 code that is not working
*/

#ifndef PCB_QUEUE_H
#define PCB_QUEUE_H


typedef struct pcb PCB;
typedef struct queue Queue;

struct pcb
{
	char processName[20];
	int processClass;//1 is application, 0 is system process
	int priority;//0-9, 0 is highest, 9 is lowest
	int state;//0=Ready, 1=Blocked, -1=running
	int suspended;//0 not suspended, 1 suspended
	unsigned char* topOfStack;
	unsigned char bottomOfStack[1024];

	struct pcb* next;
	struct pcb* prev;
};

struct queue
{
	int count;
	PCB* head;
	PCB* tail;
};

Queue* ready;
Queue* blocked;
Queue* suspendedBlocked;
Queue* suspendedReady;

//Makes the four queues in this file
void setupSystem();

//allocates the memory for a PCB
PCB* AllocatePCB();

//Frees a pcb if given a correct pointer to one
int FreePCB(PCB* pcbPointer);

//Creates and sets up a PCB
PCB* SetupPCB(char* name, int processClass, int priority);

//Finds a PCB of a given name
PCB* FindPCB(char* pcbName);

//Inserts a PCB in the appropriate queue depending on its state and suspended properties.
void InsertPCB(PCB* pcbPointer);

//Removes a PCB from the appropriate queue depending on its state and suspended properties.
void RemovePCB(PCB* pcbPointer);

//Finds PCB, removes it, changes state to block, inserts
void blockPCB(char* pcbName);

//Finds PCB, removes it, changes suspended to resume, inserts
void resumePCB(char* pcbName);

//Finds PCB, removes it, changes suspended to suspend, inserts
void suspendPCB(char* pcbName);

//Finds PCB, removes it, changes state to unblocked, inserts
void unblockPBC(char* pcbName);

//Finds PCB, removes it, changes priority, inserts
void setPriority(char* pcbName,int newPriority);

//Calls loadProc for each test process and suspends them
void loadR3();

//Allocates memory for a queue, sets up properties
Queue* CreateQueue();

//Completely frees a queue and everything in it
void DestroyQueue(Queue* queuePointer);

//Inserts pcbPointer into queuePointer based on FIFO order
void EnqueueFIFO(Queue* queuePointer,PCB* pcbPointer);

//Inserts pcbPointer into queuePointer based on Priority FIFO.
void EnqueuePriority(Queue* queuePointer,PCB* pcbPointer);

//Simple dequeue on queuePointer
PCB* Dequeue(Queue* queuePointer);

//Returns queuePointer count
int GetQueueSize(Queue* queuePointer);

//Calls showReady and showBlocked
void showAll();

//Loops through ready and ready suspended queues and displays them
void showReady();

//Loops through blocked and suspended blocked queue and displays them
void showBlocked();

//Given a pcbName, find it's and displays it
void showPCB(char* pcbName);

//Loads a process with a given function, pcbName, and priority. The paramater and return value of the function MUST be void
void loadProc(void (*func)(void), char* pcbName, int priority);

//Calls DestroyQueue on all queues
void shutdownQueues();
#endif
