#include "r5.h"
#include "mpx_supt.h"

CMCB * free_head, *alloc_head;
int *begin_address;

//BEGIN TEMP COMMANDS

/*
	isEmpty:
	checks the allocated memory head to see if it is null.
	if the head is null, the allocated memory is empty.
*/
int isEmpty(){
	if(alloc_head==NULL){
		char str[543] = "";
		char temp1[20] = {"0"};
		strcat(str, "\n TRUE ");
		strcat(str,temp1);
		print(str);
	}
	else{
		char str[543] = "";
		char temp1[20] = {"0"};
		strcat(str, "\n FALSE ");
		strcat(str,temp1);
		print(str);
	}
   return (alloc_head == NULL);
}

/*
	showAllocatedMem:
	shows the allocated memory spaces
*/
void showAllocatedMem(){
	CMCB * t = alloc_head; // sets the CMCB t to the allocated head
	if(t == NULL){ // if there is nothing allocated
		char * str = "\nNo memory currently allocated.";
		int s = strlen(str);
		sys_req(WRITE, DEFAULT_DEVICE, str, &s);
	}
	else{	// if head is not null print out the address
		while(t!= NULL){
			char str[543] = "";
			char temp[20] = {"0"};
			char temp1[20] = {"0"};
			strcat(str, "\n Address of the block: ");
			itoa(t->address, temp1, 10);
			strcat(str,temp1);
			strcat(str, "Size of the block: ");
			itoa(t->size, temp, 10);
			strcat(str, temp);
			print(str);
			t = t -> next;
		}
	}
}

/*
	int_mem:
	checks initialize heap. tells the user if it failed or not. (checks 200 bytes as directed
	from r5 detailed pdf).
*/
void int_mem(){
   if(initializeHeap(200)==200){ // tests for 200 bytes
      char str[543] = "";
		char temp1[20] = {"0"};
		strcat(str, "\n Heap Initalized ");
		strcat(str,temp1);
		print(str);
   }
   else{ //if not sucessful
     char str[543] = "";
		char temp1[20] = {"0"};
		strcat(str, "\n Error: Heap not initalized. ");
		strcat(str,temp1);
		print(str);
   }
}

/*
	showFreeMem()
	shows the free memory spaces.
*/

void showFreeMem(){
	CMCB * t = free_head; // sets the CMCB t to free memory head
	if(t == NULL){ // if t is null there is no free memory
		char *str = "No memory currently free.";
		int s = strlen(str);
		sys_req(WRITE, DEFAULT_DEVICE, str, &s);
	}
	else{	// if free memory exists
		while(t!= NULL){
			char str[543] = "";
			char temp[20] = {"0"};
			char temp1[20] = {"0"};
			strcat(str, "\n Address of the block: ");
			itoa(t->address, temp1, 10);
			strcat(str,temp1);
			strcat(str, "Size of the block: ");
			itoa(t->size, temp, 10);
			strcat(str, temp);
			print(str);
			t = t -> next;
		}
	}
}

/*
	allocate_mem:
	takes a void * block of space and allocates the memory. used for testing purposes,
	it tells the user when the allocation failed. it also tells the user what the address 
	space allocated was. 
*/

void * allocate_mem(int size){
   void * space;
	if((space = (void*)our_alloc_mem((u32int)size)) != NULL){
		char * str1 = "\nMemory successfully allocated.";
		int s = strlen(str1);
		sys_req(WRITE, DEFAULT_DEVICE, str1, &s);
		//char str[543] = "";
		//char temp1[20] = {"0"};
		//strcat(str, "\n Address of the block: ");
		//itoa(space->address, temp1, 10);  <--- liye idk how to get the address to print LOL
		//strcat(str,temp1);
		//print(str);
		return space;
	}
	else{
		char str[543] = "";
		char temp1[20] = {"0"};
		strcat(str, "\n Memory failed while allocating. ");
		strcat(str,temp1);
		print(str);
	}
	return NULL;
}

// BEGIN MEMORY MANAGEMENT FUNCTIONS

/*
	our_alloc_mem:
	takes the t CMCB and sets it to the free memory head. 
	while the head is not null, it looks for a size that fits the bytes.
	returns the current allocated space.
*/

u32int our_alloc_mem(u32int size){
   CMCB* t = free_head; //sets t to free's head

   if(t==NULL){ //no free mem left
	return NULL;
   }

   while(t != NULL){ // while t is not null it searches for space to fit 
      if(t->size >= (int)size){ // if the current size is appropirate, allocate
         //ALLOCATE BLOCK HERE
	return (u32int)t+sizeof(CMCB);//(void*)
	
      }
      t = t->next;
   }
   return NULL;
}


/*
	initializeHeap;
	initializes the heap. returns size of heap. returns 0 if failed.
*/
int initializeHeap(int size){
   int *begin_address = (int*)kmalloc(size + sizeof(CMCB) + sizeof(LMCB)); // sets beginning address to CMCB + LMCB
   // initialize CMCB c and LMCB l
   CMCB *c = (CMCB*)begin_address;
   LMCB *l = (LMCB*)((void*)begin_address + size + sizeof(CMCB));
   // set variables for c
   c->type = TYPE_FREE;
   c->size = size;
   c->address = 0;
   c->name = "head";
   c->next = NULL;
   c->previous = NULL;
  // set variables for l
   l->type = TYPE_FREE;
   l->size = size;
   // sets free_head to c & alloc_head to null 
   free_head = c;
   alloc_head = NULL; // if the heap was just inialized there should be nothing allocated

   // check to make sure this bad boy was initialized yeehaw
   if(begin_address==NULL){
	return 0;
	}

   return size;
}

/*
	our_free_mem:
	frees a space of allocated memory. returns 0 if failed, 1 if successful.
	DOES NOT MERGE... we need to figure out an algorithm for that :( 
*/
int our_free_mem(void *space){
   if(space != NULL){
		CMCB * c = (CMCB*)((void*)space - sizeof(CMCB)); // create CMCB c to be freed & removed
		c->type = TYPE_ALLOCATE; //sets this as allocated space so that remove block knows
		//REMOVE ALLOCATED SPACE
		removeBlock(c);
		c->type = TYPE_FREE;
		// ADD HEAD TO FREE BLOCK HERE
		addBlock(c);
		return 1; // 
	}
   return 0;
}

/*
	freeMem:
	checks our our_free_mem function. tells the user if it fails or passes. 
*/

void freeMem(int address){
   CMCB * t = alloc_head;
   while(t != NULL){
      if(t->address == address){
		our_free_mem(t);
        	char * str1 = "\nMemory successfully freed.";
		int s = strlen(str1);
		sys_req(WRITE, DEFAULT_DEVICE, str1, &s);
		char str[543] = "";
		char temp1[20] = {"0"};
		strcat(str, "\n Address of the block: ");
		itoa(t->address, temp1, 10);
		strcat(str,temp1);
		print(str);
		return;
      }
      t = t->next;
   }
   		char str[543] = "";
		char temp1[20] = {"0"};
		strcat(str, "\n Address not freed.");
		itoa(t->address, temp1, 10);
		strcat(str,temp1);
		print(str);
}



/*
	removeBlock:
	removes allocated or free space of memory.
*/

void removeBlock(CMCB *t){

	if(t->type == TYPE_ALLOCATE){ //if t is allocated
		//if t's next is null, set t->previous->next to null and t->previous to null
		if(t->next == NULL){
			t->previous->next = NULL; //detaches previous to t
			t->previous = NULL; // detaches t from previous
		}
		//if t's previous is null, alloc_head will be t's next & alloc_head->previous null
		else if(t->previous == NULL){
			alloc_head=t->next; // sets allocated head to next
			alloc_head->previous = NULL; //detaches previous 
			t->next = NULL; //detaches t from next
		}
		else{
			t->next->previous=t->previous; //sets t's next->previous to the one before t
			t->previous->next = t->next; //sets t's previous' next to t->next to skip t
			t->next= NULL; // detach t
			t->previous = NULL; // detach t
		}
	}
	else if(t->type == TYPE_FREE){ //if t is free
		//if t's next is null, set t->previous->next to null and t->previous to null
		if(t->next == NULL){
			t->previous->next = NULL; //detaches previous to t
			t->previous = NULL; // detaches t from previous
		}
		//if t's previous is null, alloc_head will be t's next & alloc_head->previous null
		//if the previous is null then t is the ALLOC_HEAD
		else if(t->previous == NULL){
			free_head=t->next; // sets allocated head to next
			free_head->previous = NULL; //detaches previous 
			t->next = NULL; //detaches t from next
		}
		else{
			t->next->previous=t->previous; //sets t's next->previous to the one before t
			t->previous->next = t->next; //sets t's previous' next to t->next to skip t
			t->next= NULL; // detach t
			t->previous = NULL; // detach t
		}
	}
}


/*
	addBlock:
	adds allocated or free space of memory.
*/

void addBlock(CMCB *t){
	if(t->type == TYPE_ALLOCATE){ //if t is ALLOCATED
		if(alloc_head == NULL){ // if the alloc_head is null the t is the head
			alloc_head=t; // sets alloc_head to t
			t->next = NULL; // sets t's next to null
			t->previous= NULL; // sets t's previous to null
			alloc_head->next = NULL; // sets alloc_head next to null
			alloc_head->previous = NULL; //sets alloc_head previous to null
		}
		else if(alloc_head->next == NULL){ // if the next is null
			alloc_head->next=t;
			t->previous = alloc_head;
		}	
	}
	else if(t->type == TYPE_FREE){ //if t is FREE
		if(free_head == NULL){ // if the free is null the t is the head
			free_head=t; // sets free to t
			t->next = NULL; // sets t's next to null
			t->previous= NULL; // sets t's previous to null
			free_head->next = NULL; // sets free next to null
			free_head->previous = NULL; //sets free previous to null
		}
		else if(free_head->next == NULL){ // if the next is null
			free_head->next=t;
			t->previous = free_head;
		}
	}
}



