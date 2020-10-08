/** @file menu.c
*/


#include "menu.h"
#include "r1.h"
#include "r5.h"
#include "AlexWork/PCB_QUEUE.h"
#include "AlexWork/R3.h"
#include <stdbool.h>
#include "polling.h"


bool quit = false;
 char ali[30][200];
 char com[30][200];
int i = 0;

//char array that connects nickname to official function, then have choice cycle look for alias and then call actual choice
void alias(){
	
	char choice[30]={'0'};
	char n_name[30]={'0'};
	

	int count = sizeof(choice);
    	int *iptr  = &count;
    	print("\nWhat choice would you like to create an alias for?");
	print("\n1. help		10. loadr3			19. create pcb");
	print("\n2. get date		11. unblock pcb			20. delete pcb");
	print("\n3. set date		12. suspend			21. block pcb");
	print("\n4. get time		13. resume			22. initialize heap");
	print("\n5. set time		14. set priority		23. free memory");
	print("\n6. version		15. show pcb			24. allocate memory");
	print("\n7. shutdown		16. show all processes		25. show free memory");
	print("\n8. change color	17. show ready processes 	26. show allocated memory");
    	print("\n9. clear		18. show blocked processes 	27. alias");
	print("\nPlease type exact command seen above (include spaces) in all lowercase:\n");
    	serial_input(choice,iptr);

	//determine if command is legit
	//if legit then put command in half of 2d and then alias in other half
    	if(strcmp(choice,"help") == 0 || strcmp(choice,"get date") == 0 || strcmp(choice,"set date") == 0 || strcmp(choice,"get time") == 0 || strcmp(choice,"set time") == 0 || strcmp(choice,"version") == 0 || strcmp(choice,"shutdown") == 0 || strcmp(choice,"change color") == 0 || strcmp(choice,"clear") == 0 || strcmp(choice,"loadr3") == 0 || strcmp(choice,"unblock pcb") == 0 || strcmp(choice,"suspend") == 0 || strcmp(choice,"resume") == 0 || strcmp(choice,"set priority") == 0 || strcmp(choice,"show pcb") == 0 || strcmp(choice,"show all processes") == 0 || strcmp(choice,"show ready process") == 0 || strcmp(choice,"show blocked processes") == 0 || strcmp(choice,"create pcb") == 0 || strcmp(choice,"delete pcb") == 0 || strcmp(choice,"block pcb") == 0 || strcmp(choice,"initialize heap") == 0 || strcmp(choice,"free memory") == 0 || strcmp(choice,"allocate memory") == 0 || strcmp(choice,"show free memory") == 0 || strcmp(choice,"show allocated memory") == 0 || strcmp(choice,"alias") == 0)
    	{	

		strcpy(com[i],choice);
		int count2 = sizeof(n_name);
    		int *iptr2  = &count2;
		print("enter an alias for the command:\n");
		serial_input(n_name,iptr2);
		strcpy(ali[i],n_name);
		i++;
	}
	else{
		print("please enter an actual command to alias");
	}
	
}
	char new_n[30]={'0'};
//look thorugh 2D array to see if the alias is in it; if yes then look for the matching command
//search("s") looks through array for s and finds it matches to suspend
char* search(char* command){

	//goes into search each time but does not make it into if statement
	//name now contains h or the alias
	//
	char* name = command;
	//char arr[30];
	int i;
	
	for(i = 0; i < 30; i++){
		//gets into for loop but not into if
		//ali[i] should contain all the aliases
		//strcpy(arr, ali[i]);
		//print(arr);

		//for some reason ali[i] now prints symbols

		if(strcmp(name, ali[i]) == 0){
			strcpy(new_n, com[i]);

			return new_n;
		}
		
			//print("this is not an alias");
	}
	print(name);
	return name;
}


//PCB menu
void pcb_menu(){
	print(color_yellow "\n PCB MENU");
	print(color_blue "\n Please type in lower case.");
	print("\n 1.	Suspend");
	print("\n 2.	Resume");
	print("\n 3.	Set Priority");
	print("\n 4.	Show PCB");
	print("\n 5.	Show All Processes");
	print("\n 6.	Show Ready Processes");
	print("\n 7.	Show Blocked Processes");
	print("\n 8.	Create PCB");
	print("\n 9.	Delete PCB");
	print("\n 10.	Block PCB");
	print("\n 11.	Unblock PCB");
	print("\n");
	print(color_clear "");
}

// Menu Method
void print_menu()
{
	clean_screen();
	print(color_clear "");
       	print("        _.:::::._\n");
    	print("      .:::'_|_':::.\n");
    	print("      ::' --|-- '::..  \n ");
   	print("    :` .---`---. ':|  \n");
   	print("    |: ( O R E O ) :| \n");
  	print("    |:: `-------' ::| \n");
        print("    |:::.......:::  | \n");
        print("      .:::::::::::'\n");
        print("        `'```'`\n");
	print(color_red "\n");
	print("\n               			Oreo   OS                     ");
	print("\n               			Main Menu                     ");
	print("\n	-----------------------------------------------------------------"); 
	print("\n1. help		10. loadr3			19. create pcb");
	print("\n2. get date		11. unblock pcb			20. delete pcb");
	print("\n3. set date		12. suspend			21. block pcb");
	print("\n4. get time		13. resume			22. initialize heap");
	print("\n5. set time		14. set priority		23. free memory");
	print("\n6. version		15. show pcb			24. allocate memory");
	print("\n7. shutdown		16. show all processes		25. show free memory");
	print("\n8. change color	17. show ready processes 	26. show allocated memory");
    	print("\n9. clear		18. show blocked processes 	27. alias");
	print("\nPlease type exact command seen above (include spaces) in all lowercase:\n");
	print(color_clear "");
	//choices();
}

void choices(){
	print_menu();

	while(!quit){

		char choice[30] = {'0'}; //user's choice
	 	int count = sizeof(choice); // length of command
	    	int *iptr  = &count; //pointer to command
	    	serial_input(choice,iptr); //calls input

		
		//search first to see if input is an alias
		strcpy(choice,search(choice));
	    	if(strcmp(choice,"help") == 0) // help menu
	   	{
			help();
	   	}
		else if(strcmp(choice,"initialize heap")==0){ //prints menu again
			int_mem();
		}
		else if(strcmp(choice, "allocate memory")==0){
			allocate_mem(200);
		}
		else if(strcmp(choice,"free memory")==0){ //prints menu again
			freeMem(200);
		}
		else if(strcmp(choice,"show free memory")==0){ //prints menu again
			showFreeMem();
		}
		else if(strcmp(choice,"show allocated memory")==0){ //prints menu again
			showAllocatedMem();
		}
		else if(strcmp(choice, "isEmpty")==0 || strcmp(choice, "isempty")==0){
			isEmpty();
		}
		else if(strcmp(choice,"menu")==0){ //prints menu again
			print_menu();
		}
		else if(strcmp(choice,"get date") == 0 || strcmp(choice,"getdate") == 0) // get date
		{
			getDate();
            		
		}
		else if(strcmp(choice,"set date") == 0 || strcmp(choice, "setdate") == 0) // set date
		{
			setDate();
            		
		}
	else if(strcmp(choice, "alias") == 0){ // alias(bonus points)
			alias();
           		
		}
		else if(strcmp(choice, "get time") == 0 || strcmp(choice, "gettime") == 0) // get time
		{
			getTime();
            		
		}
		else if(strcmp(choice, "set time") == 0 || strcmp(choice, "settime") == 0) // set time
		{
			setTime();
            	
		}
		else if(strcmp(choice, "version") == 0) // version
		{
			version();
            		
		}
		else if(strcmp(choice, "shutdown") == 0 || strcmp(choice, "quit") == 0) // shutdown menu
		{
				shutdown(&quit);
		}
		else if(strcmp(choice, "change color") == 0) // change colour
		{
			change_color();
            		
		}
		else if(strcmp(choice, "clear") == 0) // clear screen
		{
		   	clean_screen();
		}
		else if(strcmp(choice, "pcb menu") == 0){ // PCB menu
			pcb_menu();
           		
		}
		else if(strcmp(choice, "suspend") == 0){ // suspend PCB
		    	char name[64] = {0}; // char array for name
		    	int count=sizeof(name); // count is length of name
		    	int *iptr = &count; // pointer to count
		    	print("\nplease enter the name of process you want to suspend\n");
		    	serial_input(name,iptr); // calls polling function
		    	if(strcmp(name,"idle")==0 || strcmp(name,"commhand")==0){
				print("system processes cannot be suspended");
			}
			else{
		    		suspendPCB(name); 
		    	}
		    	
		}
		else if(strcmp(choice, "resume") == 0){ // resume PCB
		    	char name[64] = {0}; // name char array
		    	int count=sizeof(name); // count is length of name 
		    	int *iptr = &count; // pointer to count
		    	print("\nplease enter the name of process you want to resume\n"); 
		    	serial_input(name,iptr); // calls polling function
		    	resumePCB(name);
		    	
		}
		else if(strcmp(choice, "set priority") == 0){ // set priority PCB
			char name[64] = {0}; // name char array 
			char new_priority[64] = {0}; // name for new priority
			int count=sizeof(name); // count is size of name 
			int *iptr = &count; // pointer to count
			print("\nplease enter the name of process you want to set priority\n");
			serial_input(name,iptr); // calls polling function
			*iptr = sizeof(new_priority); // pointer to size of priority for buffer
			if(strcmp(name,"idle")==0 || strcmp(name,"commhand")==0){
				print("system processes cannot be edited.");
			}
			else{
				print("\nplease enter the new priority of the process(must be integer from 0 to 9)\n");
				serial_input(new_priority,iptr); // calls polling function 
				setPriority(name,atoi(new_priority));
			}
			
		}
		else if(strcmp(choice, "show pcb") == 0){ // show PCB
		    	char name[64] = {0}; 
		    	int count=sizeof(name);
		    	int *iptr = &count;
		    	print("\nplease enter the name of process you want to show\n");
			serial_input(name,iptr);
		    	showPCB(name);
		    	
		}
		else if(strcmp(choice, "show all processes") == 0){ // show all PCB processes 
		    	print("\nthe detials of all processes in both ready and block queue are:\n");
		    	showAll();
		    	
		}
		else if(strcmp(choice, "show ready processes") == 0){ // show all ready processes PCB
            		print("\nthe processes in ready queue are:\n");
            		showReady();
            		
		}
		else if(strcmp(choice, "show blocked processes") == 0){ // show all blocked processes PCB
           	 	print("\nthe processes in blocked queue are:\n");
            		showBlocked();
            	
		}
		else if(strcmp(choice, "create pcb") == 0){ // create PCB
		    	char name[64] = {0};
		   	char priority[64]= {0};
		    	char classtype[64] = {0};
		    	int count=sizeof(name);
		    	int *iptr = &count;
		    	print("\nplease enter the name of the new process you want to create\n");
		    	serial_input(name,iptr);
		    	*iptr = sizeof(priority);
		    	print("\nplease enter the priority of the new process you want to create(must be 0-9)\n");
		    	serial_input(priority,iptr);
		    	*iptr = sizeof(classtype);
		    	print("\nplease enter the class of the new process you want to create(100 is system, 101 is user)\n");
		    	serial_input(classtype,iptr);
		    	SetupPCB(name,atoi(classtype),atoi(priority));
		    	
	    	}
		else if(strcmp(choice, "delete pcb") == 0){ // delete PCB
            		char name[64] = {0};
            		int count=sizeof(name);
            		int *iptr = &count;
            		print("\nplease enter the name of process you want to delete\n");
            		serial_input(name,iptr);
			PCB * p = FindPCB(name);
            		RemovePCB(p); 
            	
		}
		else if(strcmp(choice, "block pcb") == 0){ // block PCB
            		char name[64] = {0};
            		int count=sizeof(name);
            		int *iptr = &count;
            		print("\nplease enter the name of process you want to block\n");
            		serial_input(name,iptr);
            		blockPCB(name);
            		
		}
		else if(strcmp(choice, "unblock pcb") == 0){ // unblock PCB
		    	char name[64] = {0};
		    	int count=sizeof(name);
		    	int *iptr = &count;
		    	print("\nplease enter the name of process you want to unblock\n");
		    	serial_input(name,iptr);
		    	unblockPBC(name);  
		    	
		}
        else if(strcmp(choice,"loadr3") == 0 )
        {
                loadR3();
        }
        else if(strcmp(choice,"yield") == 0)
        {
                asm volatile ("int $60");
        }
		else // if the command handler does not recognise a command
		{
			print("\n Please type a correct command.\n");
            		
		}

	sys_req(IDLE, DEFAULT_DEVICE, NULL, NULL);
	}
	

}
