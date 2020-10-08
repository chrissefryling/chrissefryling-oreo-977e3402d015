/** @file r3.c
*/

#include "r3.h"
#define USER_CLASS 101
#include "r2.h"
#include "mpx_supt.h"
#include <system.h>
#include "procsr3.h"
#include <core/io.h>
#include <core/serial.h>
#include <core/interrupts.h>
#include <string.h>
#include <stdint.h>
#include <core/tables.h>
#include <mem/heap.h>
#include <mem/paging.h>
extern struct queue * ready_q;

/*
syst_call: 
parameter: structure context defined in r3.h
Check to see if sys_call has been called before(i.e if the currently-operating(cop) is NULL, sys_call has not been called):
    If sys_call has not been called, save a reference to old(the caller's) context in a global variable
    If sys_call has been called check params.op code:
           If params.op code == IDLE, save the context(reassign cop's stack top)
           If params.op code == EXIT, free cop
    If there is a ready process: remove it from the ready queue.set state to running.Assign cop Return cop's stack top.

    Otherwise, return the context saved in step 1
*/
char name[16] = "cop";
PCB* cop;
context *temp;
param params;

void infinite_create(){
	loadPCB(r4_infinite, "infinite", 100,2);
}

void r4_infinite(){
	//print("\nInfinite process running.\n");
  	char msg[30];
  	int count=0;
	
	memset( msg, '\0', sizeof(msg));
	strcpy(msg, "INFINITE PROCESS EXECUTING.\n");
	count = strlen(msg);
  
  while(1){
	sys_req( WRITE, DEFAULT_DEVICE, msg, &count);
    	sys_req(IDLE, DEFAULT_DEVICE, NULL, NULL);
  }

}

//yield
void yield(){
	asm volatile("int $60");

}


void loadPCB(void function(), char* name, int class_, unsigned int priority)
{
	if(findPCB(name)!=NULL){
		print("\n Process already exists.");
	}
	else{
		
		PCB * new_pcb = createPCB(name,class_, priority);
		context * cp = (context*)(new_pcb->stacktop);
		memset(cp,0,sizeof(context));
		cp->fs=0x10;
		cp->gs=0x10;
		cp->ds=0x10;
		cp->es=0x10;
		cp->cs=0x8;
		cp->ebp=(u32int)(new_pcb->stackbase);
		cp->esp=(u32int)(new_pcb->stacktop);
		cp->eip=(u32int)function;
		cp->eflags=0x202;
		//new_pcb->isSuspended=0;
	}
	

}
/*
void loadr3(u32int func, char* name, int class, int priority){
	//R3 processes 
	//int stack_size = 1024; 
	// 

}*/



u32int* sys_call(context *registers)
{

     PCB *temp_proc = ready_q->head;
	if(temp_proc->isSuspended==1&&temp_proc->next!=NULL){
		temp_proc=temp_proc->next;
	}

    PCB *temp_proc = ready_q->head;

    if(cop == NULL){
	//cop = createPCB(name, 101, 1);
		temp = registers;
	}
	else{
		if(params.op_code == IDLE){
			//print("idle here");
			cop->stacktop = (unsigned char*) registers;
			//change cop's state to ready, then insert cop to end of readyqueue
				
            		cop->state=READY;
			insertPCB(cop);
			//cop->previous = ready_q->tail;
			//ready_q->tail->next = cop;
			//ready_q->tail = cop;
			}
		if(params.op_code == EXIT){
            	freePCB(cop);
			cop = NULL;

		   }
		}

	if(temp_proc!=NULL&&temp_proc->isSuspended!=1){

	if(temp_proc!=NULL){
	 remotes/origin/master
		cop=temp_proc;
		removePCB(cop);
		//cop->next->previous = NULL;
		//ready_q->head = cop->next;
		//cop->next = NULL;
		cop -> state = RUNNING;
		 return (u32int *)cop -> stacktop;
		}
	else{
                
	    return (u32int *)temp;
		}
}


void priority_l()
{
	loadPCB( proc1, "process 1", 100, 3);
	loadPCB( proc2, "process 2", 100,  2);
	loadPCB( proc3, "process 3", 100, 4);
	loadPCB( proc4, "process 4", 100, 6);
	loadPCB( proc5, "process 5", 100, 4);

}










