/** @file procsr3.c
*/


#include "../include/system.h"
#include "../include/core/serial.h"

#include "../modules/mpx_supt.h"
#include "procsr3.h"



char* msg11 = "proc1 dispatched";
char* msg21 = "proc2 dispatched";
char* msg31 = "proc3 dispatched";
char* msg41 = "proc4 dispatched";
char* msg51 = "proc5 dispatched";
int msgSize1 = 17;

char* er11 = "proc1 ran after it was terminated";
char* er21 = "proc2 ran after it was terminated";
char* er31 = "proc3 ran after it was terminated";
char* er41 = "proc4 ran after it was terminated";
char* er51 = "proc5 ran after it was terminated";
int erSize1 = 34;

void proc1()
{
  int i;

  // repeat forever if termination fails
  while(1){
    for(i=0; i<RC_1; i++){
  //    serial_println("proc1 dispatched");
	sys_req(WRITE, DEFAULT_DEVICE, msg11, &msgSize1);
      sys_req(IDLE, DEFAULT_DEVICE, NULL, NULL);
    }
      sys_req(EXIT, DEFAULT_DEVICE, NULL, NULL);
    //serial_println("proc1 ran after it was terminated");
	sys_req(WRITE, DEFAULT_DEVICE, er11, &erSize1);
  }
}

void proc2()
{
  int i;

  // repeat forever if termination fails
  while(1){
    for(i=0; i<RC_2; i++){
      sys_req(WRITE, DEFAULT_DEVICE, msg21, &msgSize1);
      sys_req(IDLE, DEFAULT_DEVICE, NULL, NULL);
    }
      sys_req(EXIT, DEFAULT_DEVICE, NULL, NULL);
    sys_req(WRITE, DEFAULT_DEVICE, er21, &erSize1);
  }
}

void proc3()
{
  int i;

  // repeat forever if termination fails
  while(1){
    for(i=0; i<RC_3; i++){
      sys_req(WRITE, DEFAULT_DEVICE, msg31, &msgSize1);
      sys_req(IDLE, DEFAULT_DEVICE, NULL, NULL);
    }
      sys_req(EXIT, DEFAULT_DEVICE, NULL, NULL);
    sys_req(WRITE, DEFAULT_DEVICE, er31, &erSize1);
  }
}

void proc4()
{
  int i;

  // repeat forever if termination fails
  while(1){
    for(i=0; i<RC_4; i++){
      sys_req(WRITE, DEFAULT_DEVICE, msg41, &msgSize1);
      sys_req(IDLE, DEFAULT_DEVICE, NULL, NULL);
    }
      sys_req(EXIT, DEFAULT_DEVICE, NULL, NULL);
    sys_req(WRITE, DEFAULT_DEVICE, er41, &erSize1);
  }
}

void proc5()
{
  int i;

  // repeat forever if termination fails
  while(1){
    for(i=0; i<RC_5; i++){
      sys_req(WRITE, DEFAULT_DEVICE, msg51, &msgSize1);
      sys_req(IDLE, DEFAULT_DEVICE, NULL, NULL);
    }
      sys_req(EXIT, DEFAULT_DEVICE, NULL, NULL);
    sys_req(WRITE, DEFAULT_DEVICE, er51, &erSize1);
  }
}
