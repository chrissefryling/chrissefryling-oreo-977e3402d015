#pragma once
#include "menu.h"
#include <system.h>
#include <mem/heap.h>
#include <string.h>
#include <core/serial.h>

#define TYPE_FREE 0 //CMCB is free
#define TYPE_ALLOCATE 1 //CMCB is allocated

typedef struct CMCB
{
    unsigned int type; // type, either allocated or free
    int address; // beginning address of the block
    int size; // size of the block
    char * name; // the name of the PCB owning the block
    struct CMCB *next; // next memory control block of same type
    struct CMCB *previous; // prev memory control block of same type
}CMCB;

typedef struct LMCB
{
    unsigned int type; // whether it is free or allocated
    int size; // size of block
}LMCB;

/*typedef struct set
{
	struct CMCB *head;
} set;*/


int isEmpty();
void showAllocatedMem();
void int_mem();
void showFreeMem();
void *allocate_mem();
u32int our_alloc_mem(u32int);
int initializeHeap(int);
int our_free_mem(void*);
void freeMem(int);
void removeBlock(CMCB*);
void addBlock(CMCB*);


