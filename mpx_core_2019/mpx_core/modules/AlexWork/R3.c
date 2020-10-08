/*
	Created by Alex Wilson 4/7/2020
	Used to replace R2, R3, and R4 code that is not working
*/
#include "R3.h"

//Global Variables
PCB* cop;
context* caller;
extern param params;

u32int* sys_call(context* registers){
	PCB* temp = Dequeue(ready);
	if(cop == NULL){
		caller = registers;
	}
	else if(params.op_code == IDLE){
		cop -> topOfStack = (unsigned char*) registers;
		InsertPCB(cop);
	}
	else if(params.op_code == EXIT){
		FreePCB(cop);
	}

	if(ready -> count != 0){
		cop = temp;
		return (u32int*)cop -> topOfStack;
	}
	return (u32int*)caller;
}
