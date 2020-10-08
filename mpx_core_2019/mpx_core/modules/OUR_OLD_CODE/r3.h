/** @file r3.h
*/

#pragma once
#include"r2.h"

typedef struct context{
    u32int gs,fs,es,ds;
    u32int edi,esi,ebp,esp,ebx,edx,ecx,eax;
    u32int eip,cs,eflags;
}context;


u32int* sys_call(context *registers);
void yield();
void loadr3();
void infinite_create();
void priority_l();
void loadPCB(void function(), char*, int, unsigned int);
void r4_infinite();
