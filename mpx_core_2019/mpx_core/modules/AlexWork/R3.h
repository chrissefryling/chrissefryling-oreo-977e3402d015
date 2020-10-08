/*
	Created by Alex Wilson 4/7/2020
	Used to replace R2, R3, and R4 code that is not working
*/
#include "PCB_QUEUE.h"
#include "system.h"
#include "../mpx_supt.h"

#ifndef R3
#define R3

struct Context{
	u32int gs,fs,es,ds;
	u32int edi,esi,ebp,esp,ebx,edx,ecx,eax;
	u32int eip,cs,eflags;
};

typedef struct Context context;

u32int* sys_call(context*);

#endif
