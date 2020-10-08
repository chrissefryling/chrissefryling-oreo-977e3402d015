/** @file r1.c
*/
#include "polling.h"
#include <system.h>
//#include "core/kmain.c"
#include <core/serial.h>
#include "r1.h"
#include "AlexWork/PCB_QUEUE.h"
#include "AlexWork/R3.h"
#define SEC 0x00
#define MIN 0x02
#define HOU 0x04
#define DAY 0x07
#define MON 0x08
#define YEA 0x09
#define WEK 0x06
//colours
#define color_red "\x1b[31m"
#define color_green "\x1b[32m"
#define color_blue "\x1b[34m"
#define color_yellow "\x1b[33m"
#define color_clear "\x1b[0m" //resets the color




//global variables

//char serial_input(char * buffer, int * count) ;// CURRENTLY ONLY WORKS FOR SINGLE NUM INPUT
//char serial_input(char * buffer, int * count);



int hexConvert(int hexIndecimal)
{
    int decInhex;
    int unitPlace = hexIndecimal / 1 % 10;
    int tenPlace  = hexIndecimal / 10 %10;

    decInhex = (unitPlace * 1) + (tenPlace * 16);
    return decInhex;
}


//formatIntTime is uesd for format second and mintues and hours, to keep it 2 digits while itself lower than 10.
/*
\fn
*/
char *formatIntTime(char *time,char *time_format_buffer)
{
    strcat(time_format_buffer,"0");
    strcat(time_format_buffer,time);
    //serial_print(time_format);
    return time_format_buffer;
}
//getDate function: to get the current Date with the format of 
//(month-day-year weekday)

void getDate()
{
    char buffer[64]={0};
    char day[64]={0};
    char month[64]={0};
    char year[64]={0};
    char weekday[64]={0};
    int  day_num,mon_num,yea_num,wek_num=0;
    char time[64] = {0};
    char time_format_buffer[64]={0};
    
    //get day:
    outb(0x70,DAY);
    day_num = inb(0x71);
    if(day_num < 10)
    {
          strcpy(day,formatIntTime(itoa(day_num,buffer,16),time_format_buffer));
          strcpy(buffer,"");
          strcpy(time_format_buffer,"");
    }

    strcpy(day,itoa(day_num,buffer,16));
    strcpy(buffer,"");
    strcpy(time_format_buffer,"");

    //get mon:
    outb(0x70,MON);
    mon_num = inb(0x71);
    switch(mon_num)
    {
       case 0x01:
        strcpy(month,"Jan");
        break;
       case 0x02:
        strcpy(month,"Feb");
        break;
       case 0x03:
        strcpy(month,"Mar");
        break;
       case 0x04: 
        strcpy(month,"Apr");
        break;
       case 0x05: 
        strcpy(month,"May");
        break;
       case 0x06:        
        strcpy(month,"Jun");
        break;
       case 0x07:        
        strcpy(month,"July");
        break;
       case 0x08:        
        strcpy(month,"Aug");
        break;
       case 0x09:        
        strcpy(month,"Sep");
        break;
       case 0x10:        
        strcpy(month,"Oct");
        break;
       case 0x11:        
        strcpy(month,"Nov");
        break;
       case 0x12:        
        strcpy(month,"Dec");
        break;

    }

//get year:
    outb(0x70,YEA);
    yea_num = inb(0x71);
    strcat(year,"20");
    strcat(year,itoa(yea_num,buffer,16));
    strcpy(buffer,"");

    //get weekday:
    outb(0x70,WEK);
    wek_num = inb(0x71);
    strcpy(weekday,itoa(wek_num,buffer,16));

    strcpy(buffer,"");
    strcat(time,"Date is ");
    strcat(time,month);
    strcat(time,"-");
    strcat(time,day);
    strcat(time,"-");
    strcat(time,year);
    print(time);
    // brings up input again to allow typing
	print("\n");
    //choices();
    
}

void getTime()
{
    char buffer[4096]={0};
    int sec_num,min_num,hou_num=0;
    char second[1024]={0};
    char minute[1024]={0};
    char hour[1024]={0};
    char time[1024] = {0};
    char time_format_buffer[64]={0};
    //char* isAfternoon="am";


 //get second:
    outb(0x70,SEC);
    sec_num = inb(0x71);
    if(sec_num < 10)
    {
        strcpy(second,formatIntTime(itoa(sec_num,buffer,16),time_format_buffer));
        strcpy(buffer,"");
        strcpy(time_format_buffer,"");
    }
    else strcpy(second,itoa(sec_num,buffer,16));
    //get min:
    outb(0x70,MIN);
    min_num = inb(0x71);
    if(min_num < 10)
    {
        strcpy(minute,formatIntTime(itoa(min_num,buffer,16),time_format_buffer)); 
        strcpy(buffer,"");
        strcpy(time_format_buffer,"");
    }
    else strcpy(minute,itoa(min_num,buffer,16));
    //get hour:
    outb(0x70,HOU);
    hou_num = inb(0x71);
 //   if(hou_num >0x12)
 //   {
 //       
 //       hou_num = hou_num - 0x12;
 //       if(hou_num == 0x0a|| hou_num == 0x0b || hou_num ==0x0c || 
 //           hou_num == 0x0d|| hou_num == 0x0e|| hou_num == 0x0f )
 //       {
 //           hou_num = hou_num - 0x06;
 //       }
 //       strcpy(isAfternoon,"pm");

 //   }

 //   hou_num -= 0x05;
 //   if(hou_num == 0x0a|| hou_num == 0x0b || hou_num ==0x0c || 
 //         hou_num == 0x0d|| hou_num == 0x0e|| hou_num == 0x0f )
 //   {
 //       hou_num = hou_num - 0x06;
 //   }

    
    strcpy(hour,itoa(hou_num,buffer,16));

    strcat(time,"Time is ");
    strcat(time,hour);
    strcat(time," :");
    strcat(time,minute);
    strcat(time," :");
    strcat(time,second);
    strcat(time,"   ");
//    strcat(time,isAfternoon);
    print(time);
    print("\n");
	
   // brings up input again to allow typing
   //choices();

} 

//clean_screen function: to clean context on the console.
void clean_screen()
{
 print("\e[1;1H\e[2J");
}

//change color
void change_color(void){

	print("\nWhat color would you like?");
	print("\n1. Blue");
	print("\n2. Red");
	print("\n3. Green");
	print("\n4. Yellow");
	print("\n5. Reset color");
	char choice = NULL; 
	char buff[2]; // character buffer for input - currently set for only one number input ** will change later
	int count = 2; //buffer count
		serial_input(buff,(int*)count); // calls serial input, our polling function
		choice = buff[0];
	if(choice == '1'){
		print(color_blue "");
	}
	else if(choice == '2'){
		print(color_red "");
	}
	else if(choice == '3'){
		print(color_green "");
	}

	else if(choice == '4'){
		print(color_yellow "");
	}
	else if(choice == '5'){
		print(color_clear "");
	}
	else{
		print("Please choose 1-5");
	}
	print_menu(); //print menu again when done

}

//swap function:swap two character.
void swap(char* str_x, char* str_y)
{
    *str_x^=*str_y;
    *str_y^=*str_x;
    *str_x^=*str_y;

}

//reverse function: to reverse a string.
void reverse(char str[], int length)
{
    int start = 0;
    int end = length-1;
    while(start < end)
    {
        swap(&str[start],&str[end]);
        start++;
        end--;
    }
}
//itoa function 
char* itoa(int num, char* str, int base)
//paramaters: num, the number you want transform.
//            str, the return string variable you have decleared.
//            base,transform the string to decimal, hex or binary.

{
    int i =0;
    bool isNegative = false;

    if(num == 0)
    {
        str[i++] = '0';
        str[i]   ='\0';
        return str;
    }

    if(num <0 && base == 10)
    {
        isNegative = true;
        num = -num;
    }

    while(num != 0)
    {
        int rem = num % base;
        str[i++] = (rem>9) ? (rem-10) +'a' : rem + '0';
        num = num/base;
    }

    if (isNegative)
    {
        str[i++] = '-';
    }
    
    str[i] = '\0';

    reverse(str,i);
    return str;

}

void date_update(int byte, int value)
{
char buffer[1024];
cli();
outb(0x70,byte);
print(itoa(value,buffer,16));
outb(0x71,value);
sti();

/**
cli();
outb(0x70,HOU);
outb(0x71,0x16);
sti();
*/
}

// Method To Set Date ( )

void setDate()
{
date_validation(YEA,"Enter year 00-99", 0,99);
date_validation(MON,"Enter Month 01-12", 1,12);
date_validation(DAY,"Enter DAY 01-31", 1,31);
}

void setTime(){


date_validation(HOU,"Enter Hours 00-23", 0,23);
date_validation(MIN,"Enter Minutes 00-59", 0,59);
date_validation(SEC,"Enter Second 00-59", 0,59);
}

void date_validation (int type, char *p, int min, int max)
{
	int number;
    print(p);
    while(1)
    {   
        char input[5] = {'0'};
	    int count=sizeof(input);
	    int *iptr = &count;
        serial_input(input,iptr);
        number=atoi(input);       
        if(number>=0 && number<=99) 
        {
             if(number>=min && number<= max) 
            { 
                break;
            }
            else 
            {
                print("valid input: must be 0-23");
                print("\n");
            }      // else
        }
        else 
        print("valid input: number must be 0 - 99\n");
    }
    number = hexConvert(number);
    date_update(type,number);
    print("\n");
    if(type == SEC)
    {
        getTime();
    }
    if(type == DAY)
    {
         getDate();
    } 
}



//method to write
void print(char* str)
{
    char buffer[1400];
    int size = sizeof(buffer);
    int *ptr = &size;
    strcpy(buffer,str);
    sys_req(WRITE,0x3f8,buffer,ptr);
}
// Shutdown Method

void shutdown(bool *ptr)
{
    char letter[10]={'0'};
    int count = sizeof(letter);
    int *iptr  = &count;
    print("\nAre you sure to shut down the system?  (yes/y,no/n)");
    serial_input(letter,iptr);
    //print(letter);
    if(strcmp(letter,"yes") == 0 || strcmp(letter,"y") == 0)
    {
        *ptr = true;
	shutdownQueues();
	sys_req(EXIT, DEFAULT_DEVICE, NULL, NULL);
    }
    else if(strcmp(letter, "no") == 0 ||strcmp(letter, "n") == 0)
    {
        print_menu();
    }
    else
    {
        print("You have input a invalid number, please press \"Enter\" to select again");
    }
    
}



// Help Method 

void help()
{
	print(color_clear "\n ........................................................");
	print("\n		Welcome to Oreo Operating System");
	print("\n ........................................................");
	print("\n NOTE: all commands are lowercase, include spaces for some.");
	print("\n If you'd like to see the commands only, type: menu");
	print(color_green "\nUser's Manual ");
	print("\nCommands that can be selected");

	print("\nsdvSD");
	
	print(color_clear "\n 1. help");
	print("\nWhen help is entered into the system, an explanation of each command will be displayed. The information will consist of what each command will perform and how to select the command. ");
	
	print(color_green "\n 2. get date");
	print("\nWhen getdate or get date is entered into the system, the command get date is selected. This command will return the date.");

	print(color_clear "\n3. set date");
	print("\nWhen setdate or set date is entered into the system, the command set date is selected. This command allows the user to set a value for the date");

	print(color_green "\n4. get time");
	print("\nWhen gettime or get time is entered into the system, the command get time is selected. This command will return the time.");

	print(color_clear "\n5. set time");
	print("\nWhen settime or set time is entered into the system, the command set time is selected. This command allows the user to set a value for the time");
	print("\n The Date Format:");
	print("\n YEAR	   __ 2 digit 00-99");
	print("\n MONTH	   __ 2 digit 01-12");
	print("\n DAY is   __ 2 digit 01-31");
	print("\n HOUR is  __ 2 digit 00-23");
	print("\n MINUTES  __ 2 digit 00-59");
	print("\n SECONDS  __ 2 digit 00-59");
 
	print(color_green "\n6. version");
	print("\nWhen version is entered into the system, the command version is selected. This command will display the current version the system s on and the date that it was completed.");
	
	print(color_clear "\n7. shutdown");
	print("\nWhen shutdown is entered into the system, the command shutdown is selected. This command will then question if the user really wants to shutdown and if yes, will then proceed to shutdown and if not will return user to menu.\n");
	
	print(color_green "\n8. change color");
	print("\nWhen change color is entered into the system, the command change color is selected. This command will allow the user to choose new colors for the terminal output.");

	print(color_clear "\n9. clear");
	print("\nWhen clear is entered into the system, the terminal is cleared.");

	print(color_green "\n10. loadr3");
	print("\nWhen loadr3 is typed into the system, it will load module 3 processes (from ecampus).");
	
	print(color_clear "\n11. yield");
	print("\nWhen yield is typed into the system, it will load all non-suspended processes.");

	print(color_green "\n12. suspend");
	print("\nWhen suspend is entered into the system, it will let you suspend any pcb you choose.");
	
	print(color_clear "\n13. resume");
	print("\nWhen resume is entered into the system, it will let you resume any pcb you choose.");
	
	print(color_green "\n14. set priority");
	print("\nWhen set priority is typed into the system, it will let you set a priority for a pcb you choose.");

	print(color_clear "\n15. show pcb");
	print("\nWhen show pcb is typed into the system, it will present you the selected pcb's fields.");

	print(color_green "\n16 - 18. showing processes");
	print("\nWhen show all/ready/blocked processes is typed into the system, it will present you the pcbs in the state that you choose, or all pcbs if you choose show all processes.");

	print(color_clear "\n19. create pcb");
	print("\nwhen create pcb is typed into the system, the user can create a pcb.");

	print(color_green "\n20. delete pcb");
	print("\nWhen delete pcb is typed into the system, the user can delete a pcb.");

	print(color_clear "\n21. block pcb");
	print("\nWhen block pcb is typed into the system, the user can block a pcb.");

	print(color_green "\n22. unblock pcb");
	print("\nWhen unblock pcb is typed into the system, the user can unblock a pcb.\n");

	print(color_clear "\n23. initialize heap");
	print("\nWhen initialize heap is typed into the system, the user initializes the heap.\n");

	print(color_green "\n23. free memory");
	print("\nWhen free memory is typed into the system, the user will be told if memory is sucessfullu freed or not.\n");

	print(color_clear "\n24. allocate memory");
	print("\nWhen allocate is typed into the system, the user is told the address of the space allocated memory and whether allocation fails or not.\n");

	print(color_green "\n25. show free memory");
	print("\nWhen show free memory is typed into the system, the user is shown the free memory spaces such as the address and size of the block.\n");

	print(color_clear "\n26. show allocated memory");
	print("\nWhen show allocated memory is typed into the system, the user is shown the address and size of the blocks of memory allocated.\n");

	print(color_green "\n27. alias");
	print("\nWhen alias is typed into the system, the user can create an alias for a command to create a shortcut of sorts. \n");
    // brings up input again to allow typing
	//choices();

}	

//Version Method
void version()
{
	print(color_blue "\nOreo Operating System 5.0\n");
// brings up input again to allow typing
	//choices();


}



