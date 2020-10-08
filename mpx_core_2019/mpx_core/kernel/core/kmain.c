
/*! \file kmain.h
  ----- kmain.c -----

  Description..: Kernel main. The first function called after
      the bootloader. Initialization of hardware, system
      structures, devices, and initial processes happens here.
*/

#include <stdint.h>


#include <core/io.h>
#include <core/serial.h>
#include <core/tables.h>
#include <core/interrupts.h>
#include <mem/heap.h>
#include <mem/paging.h>
#define USER_CLASS 101
#include <system.h>
#include <core/io.h>
#include <core/serial.h>
#include <core/interrupts.h>
#include <string.h>
#include <stdint.h>
#include <core/tables.h>
#include <mem/heap.h>
#include <mem/paging.h>
#include "modules/polling.h"
#include "modules/mpx_supt.h"
#include "modules/r1.h"
#include "modules/AlexWork/PCB_QUEUE.h"
#include "modules/AlexWork/R3.h"
#include "modules/menu.h"
#include "modules/r5.h"


void kmain(void)
{
   extern uint32_t magic;
   // Uncomment if you want to access the multiboot header
   // extern void *mbd;
   // char *boot_loader_name = (char*)((long*)mbd)[16];

  
   // 0) Initialize Serial I/O and call mpx_init
	init_serial(0x3f8);
	set_serial_out(0x3f8);
	set_serial_in(0x3f8);
	mpx_init(MEM_MODULE);



   klogv("Starting MPX boot sequence...");
   klogv("Initialized serial I/O on COM1 device...");

   // 1) Initialize the support software by identifying the current
   //     MPX Module.  This will change with each module.
 	
   // 2) Check that the boot was successful and correct when using grub
   // Comment this when booting the kernel directly using QEMU, etc.
   if ( magic != 0x2BADB002 ){
     //kpanic("Boot was not error free. Halting.");
   }
   
   // 3) Descriptor Tables
   klogv("Initializing descriptor tables...");
	init_idt();
	init_gdt();
	init_irq();
	sti();
	init_paging();

   // 4) Virtual Memory
   klogv("Initializing virtual memory...");
	//const int size = 50000;

	u32int(*func)(u32int);
	func=our_alloc_mem;
	sys_set_malloc(func);

	int (*function)(void *);
	function=our_free_mem;
	sys_set_free(function);
	
	initializeHeap(50000);


   // 5) Call YOUR command handler -  interface method
   klogv("Transferring control to commhand..."); 
  /* clean_screen();
   //print_menu(); //prints menu
   loadPCB(idle, "idle", 100, 8);
   loadPCB(choices, "commhand", 100, 0);
   yield();
   //choices();*/
	setupSystem();
	choices();
	loadProc(&choices,"commhand",0);
	//loadProc(&idle,"idle",9);
	asm volatile ("int $60");

   // 6) System Shutdown on return from your command handler
   klogv("Starting system shutdown procedure...");
   
   /* Shutdown Procedure */
   klogv("Shutdown complete. You may now turn off the machine. (QEMU: C-a x)");
   hlt();



}
