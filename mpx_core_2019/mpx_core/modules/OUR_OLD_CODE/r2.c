/** @file r2.c
*/

#include "menu.h"
// implement ready queue and blocked queue
struct queue *ready_q = &(struct queue){.count = 0, .head = NULL, .tail = NULL};
struct queue *block_q = &(struct queue){.count = 0, .head = NULL, .tail = NULL};


/*!
\struct 
Allocate PCB
- uses sys_alloc_mem() to allocate memory for new PCB
- can include using the stack, does not have to
- should return NULL if error
- should return PCB pointer if successful
*/
struct PCB* allocatePCB(){

   PCB *p; // create PCB 
   p = (PCB*)sys_alloc_mem(sizeof(PCB)); // allocate memory for PCB
   memset(p->stackbase,0,sizeof(p->stackbase));
/**
*returns null if not successful 
*/
   if(p==NULL){
	 print("\nan error occured  when allocate process\n");
     return NULL;
   }
   print("\nallocate the process sucessfully\n");
   return p;	
}

/**
*Free PCB
*- uses sys-free-mem() to free all memory associated with given PCB 
*- returns success or error code (0 or 1)
*/
int freePCB(struct PCB *p){
    sys_free_mem(p->stacktop);
    sys_free_mem(p->stackbase);
    int eeeeee = sys_free_mem(p); // frees PCB

    if(eeeeee != -1) // if memory is freed, report to user. if not, report to user the error
    {
        //print("\nfree the process successfully\n");
        return 1;
    }
    else
    {
        //print("\nan error occured when free the process\n");
	    return 0;
    }
}



/**
*SetupPCB 
*- will call allocatePCB() to create an empty PCB
*- returns PCB pointer 
*- returns NULL if an error occurs or an invalid param is given
*/
PCB* setupPCB(char *name, unsigned int class, unsigned int priority){
    
    if(0<sizeof(*name) && sizeof(*name)<16) // checks if size of name is correct
    {
        if(class == 100||class == 101) // checks if class is correct
        {
            if(priority <= 9) // checks if priority is correct
            {
                PCB *p = allocatePCB(); // create PCB and allocate space
                strcpy(p->name,name); // give PCB a name
                p->classtype= class; // give PCB a class
                p->priority=priority; // give PCB a priority 
                p->state = READY; // give PCB a state
                p->isSuspended=false; // give PCB a suspended true/false
                p->next = NULL; // attach NEXT 
                p->previous = NULL; // attach PREVIOUS
                p->stacktop = p->stackbase + 1024 -sizeof(struct context); // attach stack
		//p->stack_top = 1024-sizeof(struct context); //FIX THIS!
                return p; // return new PCB
            }
            else // show error to user
            {
                print("\nthe priority of the process is invalid\n");
                return NULL;
            }
        }
        else
        {
            print("\nthe class of the process is invalid\n");
            return NULL;
        }
    }
    else
    {
        print("\nthe name of the process is invalid\n");
        return NULL;

    }
}




/**
*FindPCB
*- search all queues for a process with a given name
*- returns PCB pointer if found
*- returns NULL if not found
*/
PCB* findPCB(char *name){
    PCB *head_r = ready_q->head; // create head of ready queue
    PCB *head_b = block_q->head; // create head of blocked queue
	
    while(head_r!= NULL) // while the head of ready is not null search through
    {
        if(strcmp(head_r->name,name) == 0) // compare PCB name with input name
        return head_r; // return if found
        head_r = head_r->next; // move to the next if not found
    }

    while(head_b!= NULL) // while the head of blocked is not null search through
    {
        if(strcmp(head_b->name,name) == 0) // compare PCB name with input name
        return head_b; // return if found
        head_b = head_b->next; // move to the next if not found
    }


    return NULL; // if no name matches, return null 
}

/*
InsertPCB
- inserts a PCB into the correct queue
- note: the ready queue is a priority queue and the blocked is FIFO
- returns nothing
*/
void insertPCB(PCB *p){
	if(p->state == READY){
		//if queue is empty then pcb becomes new head 
		if(ready_q->head==NULL||ready_q->tail==NULL){
			ready_q->head = p;
            		ready_q->tail = p;
      
		}
		else{
			//if not empty then it gets tacked onto end of queue 
			p->previous=ready_q->tail;
           		ready_q->tail->next = p;
			ready_q->tail=p;
		}
	}
	else if(p->state == BLOCKED){ //FIFO
		if(block_q->head==NULL||block_q->tail==NULL){ //if queue is empty the pcb becomes new head
			block_q->head = p;
           		block_q->tail = p;
		}
		else{ //if not empty will still become head due to FIFO
			//change old head to new head next
			p->previous=block_q->tail;
            		block_q->tail->next = p;
			block_q->tail=p;
		}
	}
}

/*
RemovePCB
- removes a PCB from the queue in which it is currently stored
- returns success or error code (0 or 1)
*/
int removePCB(PCB *p){

    PCB *temp = NULL; // create temp PCB
    if(p == ready_q->head || p == ready_q->tail || p == block_q->head || p == block_q->head){
      if(ready_q->head == p) // if ready head == p then move head to next
        ready_q->head = p->next;
        ready_q->head->previous= NULL;
      if(ready_q->tail == p) // if ready tail == p then move tail to prev
        ready_q->tail = p->previous;
        ready_q->tail->next= NULL;
      if(block_q->head == p) // if blocked head == p then move head to next
        block_q->head = p->next;
        block_q->head->previous= NULL;
      if(block_q->tail == p) // if blocked tail == p then move tail to next
        block_q->tail = p->previous; 
        block_q->tail->next= NULL;
    }
    else{
      temp = p->previous; // temp is now pcb prev
      p->previous->next = p->next; // pcb's prev's next is now p's next
      p->next->previous = temp; // p's next's prev is now = temp
      if(temp->next == p) // if temp's next = p
	return 1; 
      else return 0; 
    }
    if(p == NULL && findPCB(p->name) == NULL) // if p is null and p is not in any queue return error 
	return 1; 
    else return 0; 
}

/*
CreatePCB
- calls setupPCB() and inserts the PCB in the correct queue
- error checking: must be a valid and unique name, class must be valid, priority must be valid
*/
PCB* createPCB(char *name, unsigned int class, unsigned int priority){
   PCB* p = setupPCB(name,class,priority); // call setup PCB
   if(p != NULL)
   {
        
        print("Process created successfully\n");
        insertPCB(p); // insert the PCB
        return p;
    
   }else{
        print("Process creation failed\n");
        return NULL;
   }
}


/*
DeletePCB
- removes a PCB from the correct queue and frees all associated memory
- needs to find the PCB, unlink it from the queue, then free it
- error checking: must be valid name
*/

void deletePCB(char *name){
	if(findPCB(name)){ // checks if PCB exists 
		PCB *p = findPCB(name); // sets PCB to user's input

		if(removePCB(p) == 0) // if success
        {
            freePCB(p); // free up memory
            print("delete process successful\n");
        }
        else // if error
        {
            print("delete process failed\n");
        }
		   

		removePCB(p); // removes PCB
		freePCB(p); // frees space used

	}
	else{ // if not found
		print("\n Error: incorrect name.");
	}
}

//deletes infinite process
void delete_inf(char *name){
	//suspendPCB(name);
	if(findPCB(name)){ // checks if PCB exists 
		PCB *p = findPCB(name); // sets PCB to user's input
		if(p->isSuspended == 1){//must be suspended to delete
			if(removePCB(p) == 0) // if success
			{
			    freePCB(p); // free up memory
			    print("delete process successful\n");
			}
		}
		else // if error
		{
		    print("delete process failed. infinite was not suspended.\n");
		}
			   

		//removePCB(p); // removes PCB
		//freePCB(p); // frees space used

	}
	else{ // if not found
		print("\n Error: currently there is no infinite process.");
	}
}

/*
BlockPCB
- finds a PCB and sets it's state to blocked
- reinserts it into the blocked queue
- error checking: name must be valid
*/

//uses insertpcb
void blockPCB(char *name){
	if(findPCB(name)){ // search for PCB, if found:
		PCB *p = findPCB(name); // set PCB to user input
		p->state = BLOCKED; // set state to BLOCKED
        if(ready_q->head == p) 
        {
            ready_q->head = p->next; //if ready head is p; set head to next
            p->next = NULL; // set next to null
        }
        p->previous->next = p->next;
        p->next->previous = p->previous;
        p->next = NULL;
        p->previous = NULL;
		insertPCB(p);
	}
	
	else{
		print("\n Error: incorrect name.");
	}
}

/*
UnblockPCB
- places PCB into the unblocked state
- moves to ready queue
- error checking: name must be valid
*/
void unblockPCB(char *name){
	if(findPCB(name)){
		PCB *p = findPCB(name);
		p->state = READY; //if not blocked then ready as it is being moved to ready queue
        if(block_q->head == p)
        {
            block_q->head = p->next;
            p->next = NULL;
        }
        p->previous->next = p->next;
        p->next->previous = p->previous;
        p->next = NULL;
        p->previous = NULL;

		insertPCB(p);
	}
	else{
		print("\n Error: incorrect name.");
	}
}

/*
SuspendPCB
- places PCB in the suspended state and reinserts it into the correct queue
- error checking:name must be valid
*/
void suspendPCB(char *name){
	if(findPCB(name)){
<<<<<<< HEAD
	PCB *p = findPCB(name);
        removePCB(p);
        p->next =NULL;
        p->previous = NULL;
	p->isSuspended=true;
=======
		PCB *p = findPCB(name);
        removePCB(p);
        p->next =NULL;
        p->previous = NULL;
		p->isSuspended=true;
>>>>>>> remotes/origin/master
        insertPCB(p);
	}
	else{
		print("\n Error: incorrect name.");
	}
}

/*
ResumePCB
- places a PCB in the not suspended state and reinserts it into the correct queue
- error checking: name must be valid
*/
void resumePCB(char *name){
			if(findPCB(name)){
			PCB *p = findPCB(name);
            removePCB(p);
			p->isSuspended=false;
            insertPCB(p);
		}
		else{
			print("\n Error: incorrect name.");
		}
}

/*
set PCB priority
- sets the priority and reinserts the process into the correct queue 
- error checking: name must be valid, priority must be valid (0-9)
*/
void setPriority(char *name, unsigned int new_priority){
PCB* p = findPCB(name);
p->priority = new_priority;
}

/*
ShowPCB
- displays the following info:
	process name, class, state, suspended status, priority
- error checking:name must be valid
*/
void showPCB(char *name){
//if found
if(findPCB(name)!=NULL){
PCB* p = findPCB(name);
	char s_temp[20]= {""}; // create output string
	char str[543] ={""}; // create output string

	strcat(str, "\n Name: ");
	strcat(str, p->name);
	strcat(str, "\n Class: ");

	itoa(p->classtype, s_temp, 10);
	strcat(str, s_temp);
	strcat(str, "\n State: ");
	itoa(p->state, s_temp, 10);
	strcat(str, s_temp);
	strcat(str, "\n Suspended Status: ");
	itoa(p->isSuspended, s_temp, 10);
	strcat(str, s_temp);
	strcat(str, "\n Priority: ");
	itoa(p->priority, s_temp, 10);
	strcat(str, s_temp);
	print(str);
	p = p->next;
}
/**if not found*/
else{
	print("\n Error: incorrect name.");
}
}

/*
showReady
- displays the following information for each PCB in the ready queue:
	process name, class, state, suspended status, priority
*/
void showReady(){
PCB* p = ready_q->head;
if(p==NULL){
	print("\n No PCB in ready queue.");
}else{
while(p!=NULL){
	char s_temp[20]= {"0"};
	char str[543] = "";

	strcat(str, "\n Name: ");
	strcat(str, p->name);
	strcat(str, "\n Class: ");

	itoa(p->classtype, s_temp, 10);
	strcat(str, s_temp);
	strcat(str, "\n State: ");
	itoa(p->state, s_temp, 10);
	strcat(str, s_temp);
	strcat(str, "\n Suspended Status: ");
	itoa(p->isSuspended, s_temp, 10);
	strcat(str, s_temp);
	strcat(str, "\n Priority: ");
	itoa(p->priority, s_temp, 10);
	strcat(str, s_temp);
	print(str);
	p = p->next;
}}
}

/*
showBlocked
- displays the following information for each PCB in the blocked queue:
	process name, class, state, suspended status, priority
*/
void showBlocked(){
PCB* p = block_q->head;
if(p==NULL){
print("\n No PCB in blocked queue.");
}
else{
while(p!=NULL){
	char s_temp[20];
	char str[543] = "";

	strcat(str, "\n Name: ");
	strcat(str, p->name);
	strcat(str, "\n Class: ");

	itoa(p->classtype, s_temp, 10);
	strcat(str, s_temp);
	strcat(str, "\n State: ");
	itoa(p->state, s_temp, 10);
	strcat(str, s_temp);
	strcat(str, "\n Suspended Status: ");
	itoa(p->isSuspended, s_temp, 10);
	strcat(str, s_temp);
	strcat(str, "\n Priority: ");
	itoa(p->priority, s_temp, 10);
	strcat(str, s_temp);
	print(str);
	p = p->next;
}}
}

/*
showAll
- displays the following information for each PCB in the ready and blocked queues:
	process name, class, state, suspended status, priority
*/
void showAll(){
	//print("\n Ready Queue:");
	showReady();
	//print("\n Blocked Queue:");
	showBlocked();
}


