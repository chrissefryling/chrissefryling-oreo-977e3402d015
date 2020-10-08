/*
	Created by Alex Wilson 4/7/2020
	Used to replace R2, R3, and R4 code that is not working
*/
#include "PCB_QUEUE.h"
#include "R3.h"

#include <stdint.h>
#include <string.h>
#include <system.h>

#include <core/io.h>

#include <core/serial.h>
#include <core/tables.h>
#include <core/interrupts.h>
#include <mem/heap.h>
#include <mem/paging.h>
#include "../mpx_supt.h"
#include "../procsr3.h"

int len2 = 240;

void setupSystem()
{
	ready = CreateQueue();
	blocked = CreateQueue();
	suspendedReady = CreateQueue();
	suspendedBlocked = CreateQueue();
}

PCB* AllocatePCB(){//Creates pcb
	PCB* pcb = sys_alloc_mem(sizeof(PCB));
	return pcb;
}

int FreePCB(PCB* pcb){//Frees the pcb
	return sys_free_mem(pcb);
}

PCB* SetupPCB(char* name, int class, int priority){//Sets data to pcb
	PCB* pcb = AllocatePCB();
	strcpy(pcb -> processName,name);
	pcb -> processClass = class;
	pcb -> priority = priority;
	pcb -> state = 0;
	pcb -> suspended = 0;
	pcb -> topOfStack = pcb -> bottomOfStack + 1024 - sizeof(struct Context);
	return pcb;
}

PCB* FindPCB(char* name){//Searches for pcb in both queues

	PCB* pcb = ready -> head;
	PCB* temp = pcb;
	while(temp != NULL){
		if(!strcmp(temp -> processName,name)){
			return temp;
		}
		temp = temp -> next;
	}
	pcb = blocked -> head;
	temp = pcb;
	while(temp != NULL){
		if(!strcmp(temp -> processName,name)){
			return temp;
		}
		temp = temp -> next;
	}
	pcb = suspendedBlocked -> head;
	temp = pcb;
	while(temp != NULL){
		if(!strcmp(temp -> processName,name)){
			return temp;
		}
		temp = temp -> next;
	}
	pcb = suspendedReady -> head;
	temp = pcb;
	while(temp != NULL){
		if(!strcmp(temp -> processName,name)){
			return temp;
		}
		temp = temp -> next;
	}
	return NULL;
}
void InsertPCB(PCB* pcb){//Inserts a pcb in the appropriate queue and change queues
	if(pcb -> suspended && !(pcb -> state)){
		EnqueueFIFO(suspendedReady,pcb);
	}
	else if(pcb -> suspended && pcb -> state){
		EnqueueFIFO(suspendedBlocked,pcb);
	}
	else if(pcb -> state){
		EnqueueFIFO(blocked,pcb);
	}
	else{
		EnqueuePriority(ready,pcb);
	}
}

void RemovePCB(PCB* pcb){//Removes the PCB given
	if(pcb == NULL){
		return;
	}
	Queue* queue;
	if(pcb -> suspended && !(pcb -> state)){
		queue = suspendedReady;
	}
	else if(pcb -> suspended && pcb -> state){
		queue = suspendedBlocked;
	}
	else if(pcb -> state){
		queue = blocked;
	}
	else{
		queue = ready;
	}
	if(queue -> count <= 1){
		queue -> head = NULL;
		queue -> tail = NULL;
		pcb -> next = NULL;
		pcb -> prev = NULL;
	}//Removes if only one
	else if(pcb -> next != NULL && pcb -> prev != NULL){
		pcb -> next -> prev = pcb -> prev;
		pcb -> prev -> next = pcb -> next;
		pcb -> next = NULL;
		pcb -> prev = NULL;
	}//Removes if in middle
	else if(pcb -> next == NULL){
		queue -> tail = queue -> tail -> prev;
		queue -> tail -> next = NULL;
	}//Removes at tail
	else{
		queue -> head = queue -> head -> next;
		queue -> head -> prev = NULL;
		pcb -> next = NULL;
		pcb -> prev = NULL;
	}//Removes at head
	queue -> count--;
	return;
}

Queue* CreateQueue(){
	Queue* queue = (Queue*)sys_alloc_mem(sizeof(struct queue));
	queue -> head = NULL;
	queue -> tail = NULL;
	queue -> count = 0;
	return queue;
}
void DestroyQueue(Queue* queue){
	if(queue != NULL){
		PCB* pcb = queue -> head;
		while(pcb != NULL){
			PCB* temp = pcb;
			pcb = pcb -> next;
			RemovePCB(temp);
			FreePCB(temp);
		}
		sys_free_mem(queue);
	}
}

void EnqueueFIFO(Queue* queue , PCB* pcb){
	if(queue != NULL && pcb != NULL){
		if(!(queue -> head)){
			queue -> head = pcb;
			queue -> head -> next = queue -> tail;
			queue -> tail = pcb;
			queue -> count++;
		}
		else{
			pcb -> prev = queue -> tail;
			queue -> tail -> next = pcb;
			queue -> tail = queue -> tail -> next;
			queue -> count++;
		}
	}
}

void EnqueuePriority(Queue* queue ,PCB* pcb){
	if(queue != NULL && pcb != NULL){
		if(queue -> head == NULL) 
		{ 
			pcb -> prev = pcb ->next = NULL;
			queue -> count++;
			queue -> head = pcb; 
			queue -> tail = pcb;
			
		}
		else if(queue -> head -> priority > pcb -> priority){
			pcb -> next = queue -> head;
			queue -> head -> prev = pcb;
			queue -> head = pcb;
			queue -> count++;
		}//Adding at head

		else if(queue -> tail -> priority <= pcb -> priority){
			pcb -> prev = queue -> tail;
			queue -> tail -> next = pcb;
			queue -> tail = pcb;
			queue -> count++;
		}//Adding at tail
		else{
			PCB* trav = queue -> head;
			while(trav -> next != NULL && trav -> priority <= pcb -> priority){
				
				trav = trav -> next;
			}
			trav -> prev -> next= pcb;
			pcb -> prev = trav -> prev;
			pcb -> next = trav;
			trav -> prev = pcb;
			queue -> count++;
		}//Inserting everywhere else
	}
}
PCB* Dequeue(Queue* queue){
	PCB* temp = NULL;
	if(queue != NULL){

		if(queue -> count == 0){
			return NULL;
		}
		else if(queue -> count > 1)//If the queue has more than one
		{
			queue -> count--;
			temp = queue -> head;
			queue -> head = queue -> head -> next;
			queue -> head -> prev = NULL;
		}

		else if (queue -> count == 1)//If the queue has only one
		{
			temp = queue -> head;
			queue -> head  = NULL;
			queue -> count--;
		}
	}
	return temp;
}

int GetQueueSize(Queue* queue){
	int temp = 0;
	if(queue != NULL){
		temp = queue -> count;
	}
	return temp;
}
void showAll(){
	showReady();
	showBlocked();
}
void showReady(){
	PCB* travReady = ready -> head;
	sys_req(WRITE, DEFAULT_DEVICE,"\n************Ready Queue************\n",&len2);
	while(travReady != NULL)
	{
		char x[20];
		strcpy(x, travReady-> processName);
		sys_req(WRITE, DEFAULT_DEVICE, "\nProcess Name: ", &len2);
		sys_req(WRITE, DEFAULT_DEVICE, x, &len2);

		sys_req(WRITE, DEFAULT_DEVICE, "\nClass: ", &len2);
		char n1[3] = {travReady -> processClass + '0', '\0', '\n'};
		sys_req(WRITE, DEFAULT_DEVICE,n1 , &len2);

		sys_req(WRITE, DEFAULT_DEVICE,"\nState: Ready", &len2);

		sys_req(WRITE, DEFAULT_DEVICE,"\nSuspend Status: Not Suspended", &len2);

		sys_req(WRITE, DEFAULT_DEVICE,"\nPriority: ", &len2);
		char n[3] = {travReady -> priority + '0', '\0', '\n'};
		sys_req(WRITE, DEFAULT_DEVICE,n , &len2);
		sys_req(WRITE, DEFAULT_DEVICE,"\n\n",&len2);
		travReady = travReady -> next;
	}
	sys_req(WRITE,DEFAULT_DEVICE,"\n\n",&len2);
	PCB* travSusReady = suspendedReady -> head;
	sys_req(WRITE, DEFAULT_DEVICE,"\n************Suspended Ready Queue************\n",&len2);
	while(travSusReady != NULL)
	{
		char x[20];
		strcpy(x, travSusReady-> processName);
		sys_req(WRITE, DEFAULT_DEVICE, "\nProcess Name: ", &len2);
		sys_req(WRITE, DEFAULT_DEVICE, x, &len2);

		sys_req(WRITE, DEFAULT_DEVICE, "\nClass: ", &len2);
		char n1[3] = {travSusReady -> processClass + '0', '\0', '\n'};
		sys_req(WRITE, DEFAULT_DEVICE,n1 , &len2);

		sys_req(WRITE, DEFAULT_DEVICE,"\nState: Ready", &len2);

		sys_req(WRITE, DEFAULT_DEVICE,"\nSuspend Status: Suspended", &len2);

		sys_req(WRITE, DEFAULT_DEVICE,"\nPriority: ", &len2);
		char n[3] = {travSusReady -> priority + '0', '\0', '\n'};
		sys_req(WRITE, DEFAULT_DEVICE,n , &len2);
		sys_req(WRITE, DEFAULT_DEVICE,"\n\n",&len2);
		travSusReady = travSusReady -> next;
	}
	sys_req(WRITE,DEFAULT_DEVICE,"\n\n",&len2);
}
void showBlocked(){
	PCB* travBlocked = blocked -> head;

	sys_req(WRITE, DEFAULT_DEVICE,"\n************Blocked Queue************\n",&len2);

	while(travBlocked != NULL)
	{
		char x[20];
		strcpy(x, travBlocked-> processName);
		sys_req(WRITE, DEFAULT_DEVICE, "\nProcess Name: ", &len2);
		sys_req(WRITE, DEFAULT_DEVICE, x, &len2);

		sys_req(WRITE, DEFAULT_DEVICE, "\nClass: ", &len2);
		char n1[3] = {travBlocked -> processClass + '0', '\0', '\n'};
		sys_req(WRITE, DEFAULT_DEVICE,n1 , &len2);

		sys_req(WRITE, DEFAULT_DEVICE,"\nState: Blocked", &len2);

		sys_req(WRITE, DEFAULT_DEVICE,"\nSuspend Status: Not Suspended", &len2);

		sys_req(WRITE, DEFAULT_DEVICE,"\nPriority: ", &len2);
		char n[3] = {travBlocked -> priority + '0', '\0', '\n'};
		sys_req(WRITE, DEFAULT_DEVICE,n , &len2);
		sys_req(WRITE, DEFAULT_DEVICE,"\n\n",&len2);
		travBlocked = travBlocked -> next;
	}
	sys_req(WRITE,DEFAULT_DEVICE,"\n\n",&len2);
	PCB* travSusBlocked = suspendedBlocked -> head;

	sys_req(WRITE, DEFAULT_DEVICE,"\n************Suspended Blocked Queue************\n",&len2);


	while(travSusBlocked != NULL)
	{
		char x[20];
		strcpy(x, travSusBlocked-> processName);
		sys_req(WRITE, DEFAULT_DEVICE, "\nProcess Name: ", &len2);
		sys_req(WRITE, DEFAULT_DEVICE, x, &len2);

		sys_req(WRITE, DEFAULT_DEVICE, "\nClass: ", &len2);
		char n1[3] = {travSusBlocked -> processClass + '0', '\0', '\n'};
		sys_req(WRITE, DEFAULT_DEVICE,n1 , &len2);

		sys_req(WRITE, DEFAULT_DEVICE,"\nState: Blocked", &len2);

		sys_req(WRITE, DEFAULT_DEVICE,"\nSuspend Status: Suspended", &len2);

		sys_req(WRITE, DEFAULT_DEVICE,"\nPriority: ", &len2);
		char n[3] = {travSusBlocked -> priority + '0', '\0', '\n'};
		sys_req(WRITE, DEFAULT_DEVICE,n , &len2);
		sys_req(WRITE, DEFAULT_DEVICE,"\n\n",&len2);
		travSusBlocked = travSusBlocked -> next;
	}
	sys_req(WRITE,DEFAULT_DEVICE,"\n\n",&len2);
}

void blockPCB(char* name){
	PCB* pcb = FindPCB(name);
	if(pcb != NULL){
		RemovePCB(pcb);		
		pcb -> state = 1;
		InsertPCB(pcb);
	}

	else
	{
		sys_req(WRITE, DEFAULT_DEVICE, "\n Invalid PCB name for block\n", &len2);
	}
}
void resumePCB(char* name){
	PCB* pcb = FindPCB(name);
	if(pcb != NULL){
		RemovePCB(pcb);		
		pcb -> suspended = 0;
		InsertPCB(pcb);
	}

	else
	{
		sys_req(WRITE, DEFAULT_DEVICE, "\n Invalid PCB name for resume\n", &len2);
	}
}
void suspendPCB(char* name){
	PCB* pcb = FindPCB(name);
	if(pcb != NULL){
		RemovePCB(pcb);		
		pcb -> suspended = 1;
		InsertPCB(pcb);
	}

	else
	{
		sys_req(WRITE, DEFAULT_DEVICE, "\n Invalid PCB name for suspend\n", &len2);
	}
}
void unblockPBC(char* name){
	PCB* pcb = FindPCB(name);
	if(pcb != NULL){
		RemovePCB(pcb);		
		pcb -> state = 0;
		InsertPCB(pcb);
	}

	else
	{
		sys_req(WRITE, DEFAULT_DEVICE, "\n Invalid PCB name for unblock\n", &len2);
	}
}
void setPriority(char* name,int priority){
	PCB* pcb = FindPCB(name);
	if(pcb != NULL){
		RemovePCB(pcb);		
		pcb -> priority = priority;
		InsertPCB(pcb);
	}

	else
	{
		sys_req(WRITE, DEFAULT_DEVICE, "\n Invalid PCB name for unblock\n", &len2);
	}
}
void loadR3(){
	loadProc(&proc1,"proc1",2);
	suspendPCB("proc1");
	loadProc(&proc2,"proc2",2);
	suspendPCB("proc2");
	loadProc(&proc3,"proc3",2);
	suspendPCB("proc3");
	loadProc(&proc4,"proc4",2);
	suspendPCB("proc4");
	loadProc(&proc5,"proc5",2);
	suspendPCB("proc5");
}
void showPCB(char* name){
	PCB* pcb = FindPCB(name);
	if(pcb != NULL)
	{
		char x[20];
		strcpy(x, pcb-> processName);
		sys_req(WRITE, DEFAULT_DEVICE, "\nProcess Name: ", &len2);
		sys_req(WRITE, DEFAULT_DEVICE, x, &len2);

		sys_req(WRITE, DEFAULT_DEVICE, "\nClass: ", &len2);
		char n1[3] = {pcb -> processClass + '0', '\0', '\n'};
		sys_req(WRITE, DEFAULT_DEVICE,n1 , &len2);

		sys_req(WRITE, DEFAULT_DEVICE,"\nState: Ready", &len2);

		sys_req(WRITE, DEFAULT_DEVICE,"\nSuspend Status: Suspended", &len2);

		sys_req(WRITE, DEFAULT_DEVICE,"\nPriority: ", &len2);
		char n[3] = {pcb -> priority + '0', '\0', '\n'};
		sys_req(WRITE, DEFAULT_DEVICE,n , &len2);
	}
	sys_req(WRITE,DEFAULT_DEVICE,"\n\n",&len2);
}
void loadProc(void (*func)(void), char* name, int priority){
	PCB* new_pcb = SetupPCB(name, 1, priority);
	new_pcb -> state = 0;
	new_pcb -> suspended = 0;
	context* cp = (context*)(new_pcb->topOfStack);
	memset(cp,0,sizeof(context));
	cp->fs=0x10;
	cp->gs=0x10;
	cp->ds=0x10;
	cp->es=0x10;
	cp->cs=0x8;
	cp->ebp=(u32int)(new_pcb->bottomOfStack);
	cp->esp=(u32int)(new_pcb->topOfStack);
	cp->eip=(u32int)func;
	cp->eflags=0x202;
	
	InsertPCB(new_pcb);
}
void shutdownQueues(){
	DestroyQueue(ready);
	DestroyQueue(blocked);
	DestroyQueue(suspendedBlocked);
	DestroyQueue(suspendedReady);
}
