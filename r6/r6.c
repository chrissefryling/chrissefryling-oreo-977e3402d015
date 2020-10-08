#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "r6.h"
#include <string.h>

#define rootDirectory 19
/*

How to compile:
type: gcc -Wall r6.c <- this compiles it
press enter
then type: ./a.out <- this runs it




// variables for print sector
int bytesPS; // bytes per sector
int sectorPC; //sectors per cluster
int reservedSec; //number of reserved sectors 
int FATCopies; // number of FAT copies
int maxRoot; //maximum number of root directory entries
int totalSectors; //total # of secotrs in file system
int sectorsPF;// num of sectors per FAT
int sectorsPT; //sectors per track
int numHeads; //number of heads
int sectorCount; //total sector count for FAT32
int bootSign; // boot signature
int volumeID; //volume id
char volumeLabel[12]; //volume label
char fileSystem[9]; // file system type 
*/
int FAT[4086];
int directory= rootDirectory;
char current_directory[] = ""; // current directory that the user is in
int quit = 0; // if the user chooses to quit, else this program stays 0 and continues to run
char current_path[] = ""; // current FILE path


bootSector bs;

int main(int argc, char*argv[])
{

// note: argv[0] is ./.a.out and argv[1] is the disk file
	//char * disk_path_input;
	//printf("%d",argc);

	//check arguments
	if(argc < 2 || argc > 3){
		printf("Error. Invalid argument.");
		quit =1;
	}
   	 fpointer = fopen(argv[1],"r+");
    	 init();
   	 //printRoot();
    	//Type("1984    TXT","*");
	
	if(quit!=1){ // only displays once and also only displaces if program will run
		printf("\nWelcome to Module 6 Interface");
		printf("\n--------------------------------------------------");	
		printf("\nPlease type a command:\n");
	}

	while(quit!=1){ // runs program until quit
		interface(fpointer);
	}
}

/*

	Menu prompt:
	Prints menu / commands to user.
	param - void
	return - void
*/
void interface(FILE* fp){

	printf(color_green ">>"); // prints out path similar to linux terminal
	printf(color_clear);
	command_line(fp);
}

/*

	command line:
	takes in input from a user and chooses a function based on it
	param - void
	return - void
*/

void command_line(FILE * fpointer){
	char str[100];
	int k;
	int t = 0;
	int len1;
	int len2;
	scanf("%s", str);
	if(strcmp(str,"help")==0){ //prints the help function
		 help();
	}
	else if(strcmp(str,"cd")==0){ // changes directory
        char name[50];
        scanf("%s",name);
		change_directory(name);
	}
	else if(strcmp(str,"prd")==0){ // prints root directory
		printRoot();
	}
	else if(strcmp(str, "type")==0){ // type command
		char t_name[20];
		char t_ext[9];
		printf("Filename: ");
		scanf("%s",t_name);
		len1 = strlen(t_name);
		printf("Ext or wildcard: ");
		scanf("%s", t_ext);
		len2 = strlen(t_ext);
		t = 11 - (len1+len2);
		for(k = 0; k <t; k++){
			printf("%d", k);
			strcat(t_name," ");

		}
		strcat(t_name,t_ext);
		
		printf("%s", t_name);
		printf("%s", t_ext);
		
		//Type("1984    TXT","TXT");
		Type(t_name,t_ext);
	}
	else if(strcmp(str, "mv")==0){ // rename command
		char old[9];
		char new_name[9];
		printf("What is the name of the file you want to change?");
		scanf("%8s",old);
		printf("What is the new name you'd like to change it to?");
		scanf("%8s", new_name);
		int i=0;
		for (i=0; new_name[i] != '\0'; i++);
			for(; i<9; i++){
			new_name[i]=' ';
			}

			new_name[8]= '\0';
		renameFile(fpointer,old,new_name);

		//renameFile(fpointer,old,new_name);
	}
	else if(strcmp(str,"pbs")==0){ // print boot sector command
		printBootSector();
	}
	else if(strcmp(str,"ls")==0){ // list directory
		list_directory();
	}
	else if(strcmp(str,"quit")==0){
		printf("\nAre you sure you want to quit? y for yes or n for no.\n");
		char str3[40];
		scanf("%s",str3);
		if(strcmp(str3,"y")==0){
			quit=1; //updates quit
			//exit(0); // exits program
		}
		else if(strcmp(str3,"n")==0){
			interface(fpointer);
		}
		else{
			printf("\nUnknown command. Please type y or n.\n");
		}
	}
	else if(strcmp(str, "ls") == 0){
		list_directory();
	}
	else{ //unknown command is typed
		printf("\nUnknown command was typed. Type help for list of commands.\n");
	}	
}

void Type(char* name, char* ext)
{
    char buff[513];
    if(ext == NULL){
        ext = "";
    }

    if(!(strcasecmp(ext,"TXT") == 0 || strcasecmp(ext, "*")==0 || strcasecmp(ext,"BAT")==0 || strcasecmp(ext,"C")==0))
    {
        printf("\nThe file format is incorrect, formats accepted are: TXT, BAT, or C file.\n");
        return;
    }

    buff[512]='\0';

    int i = 0;
    int j = 0;
    int k = 0;

    int indexOfEntry=0;
    int currSector = 0;
    for(i=0 ; i<sizeOfCurr; i++)
    {
        if(((unsigned char)curr[i].fileName[0]) == 0xE5)
        {
            continue;
        }
        else if(((unsigned char)curr[i].fileName[0]) == 0x00)
        {
            break;
        }
	else if((strcmp(name,curr[i].fileName) == 0)&&(strcmp(ext,"*")==0)){ //accepts wildcards
		indexOfEntry = i;
	}
        else if((strcmp(name,curr[i].fileName) == 0)&&(strcmp(ext,curr[i].ext)==0))
        {
            indexOfEntry = i;
        }
    }

    i = indexOfEntry;
    currSector = curr[i].firstCluster;
    moveToSector(currSector+33);
    int sectors = numberOfSectors(currSector);

    while(k < sectors && j < curr[i].fileSize)
    {
        if((curr[i].fileSize-j) > 512)
        {
            fread(buff,1,512,fpointer);
            j+=512;
            printf("%s",buff);
            getchar();
        }
        else
        {
            fread(buff,1,curr[i].fileSize-j,fpointer);
            buff[curr[i].fileSize-j] = '\0';
            printf("%s",buff);
            break;
        }

        currSector = FAT[currSector];
        moveToSector(currSector+33);
        k++;
    }
    printf("\n");


    


}

/* numberOfSectors
get the number of Sector which is how far away from the starting Sector
*/
int numberOfSectors(int startingSector)
{
    int currentSector = startingSector;
    int i =1;
    while(1)
    {
        if(((currentSector>=0xFF8) &&(currentSector<=0xFFF))||(FAT[currentSector] == 0xFF7) || (FAT[currentSector] == 0x00))
        {
        	break;
        }
        i++;
        currentSector = FAT[currentSector];
    }
    return i;
}

/*
init()

do some initialize to the boot and root directory structure
*/

void init()
{  
       
    int i,j;
    fseek(fpointer,11,SEEK_SET);
    bs.bytesPerSector = getInt(2);
    bs.sectorsPerCluster= getInt(1);
    bs.numOfReservedSectors = getInt(2);
    bs.numOfFatCopies = getInt(1);
    bs.maxNumberOfRootDirect   = getInt(2);
    bs.totalNumOfSectors=getInt(2);
    fseek(fpointer,22,SEEK_SET);
    bs.numOfSectorsPerFat = getInt(2);
    bs.sectorsPerTrack = getInt(2);
    bs.numOfHeads   = getInt(2);
    fseek(fpointer,32,SEEK_SET);
    bs.totalSectorCount= getInt(4);
    fseek(fpointer,38,SEEK_SET);
    bs.bootSignature   = getInt(1);
    bs.volumeID   = getInt(4);
    fread(bs.volumeLabel,1,11,fpointer);
    fread(bs.fileSysType,1,8,fpointer);
    
    

    moveToSector(1); //Secotr1 is FAT
    for(i=0; i < 4086; i+=2)
    {
        unsigned char temp[3];
        fread(temp,1,3,fpointer);    // initialize to the FAT
        FAT[i] = ((temp[1] & 0x0f) << 8) +temp[0];
        FAT[i+1] = ((temp[2] &0xf0) <<4 )+((temp[2] & 0x0f)<<4)+ ((temp[1]&0xf0)>>4);
    }

    for(i=0;i<14;i++)            //traverse 14 sectors in the root directory
    {
        moveToSector(19+i);      // move the i th sector root directory
        for(j=0;j<16;j++)        //traverse 16 entrys in each root directory
        {
            fread(root[16*i+j].fileName,1,8,fpointer);
		//file names are displayed w/o ext


            //fread somehow modifies the contents of fileName
            fseek(fpointer,0,SEEK_CUR);
            fread(root[16*i+j].ext,1,3,fpointer);
           

	    root[16*i+j].attribute=getInt(1);

	    root[16*i+j].reserved = getInt(2);
            root[16*i+j].creationTime = getInt(2);
            root[16*i+j].creationDate = getInt(2);
            root[16*i+j].lastAccessDate = getInt(2);
            root[16*i+j].lastWriteTime = getInt(2);
            root[16*i+j].lastWriteDate = getInt(2);
            fseek(fpointer,2,SEEK_CUR);
            root[16*i+j].firstCluster = getInt(2);
            root[16*i+j].fileSize=getInt(4);
 
        }
    }

    curr = root;
    sizeOfCurr = 272;
    startOfCurr = 14;
}

/*
getInt
parameter: get howmuch bytes from entry.
get bytes from entry and conver it to int type.
*/
int getInt(int sizeOfbytes)
{
    int i,j;
    i = 0;
    j = 0;
    unsigned char temp[sizeOfbytes];
    fread(temp,1,sizeOfbytes,fpointer);
    for(i = 0; i < sizeOfbytes; i++)
    {
        j+=((temp[i] & 0xff ) << (8*i));
    }

    return j;
}

/* moveToSecotr 

move to sepcific sector you want
*/
void moveToSector(int sector)
{
    fseek(fpointer, 512*sector, SEEK_SET);
}

void printRoot(void)
{
	char* nullVal = "";
	char* nullVal2 = "";
    printDir(root,272,nullVal,nullVal2);
}


/* 
printDir

parameter: p is the pointher to entry, num is the how much entrys in directory

print the files and subdirectory in currnt direcoty
*/

void printDir(directoryEntry *p, int num, char* name, char* type)
{
    int i;
	
    for(i = 0; i < num; i++) 
    {
	//fileName has ext
	//printf("%s", p[i].fileName);
        if((unsigned char) p[i].fileName[0] == 0x00) //0x00 means all entries are free in current directory then breat the loop.
        {
            break;
        }
        else if((unsigned char) p[i].fileName[0] == 0xE5
                || ((unsigned int) p[i].attribute&0x02) == 0x02)
                //first byte is  0xE5 means is deleted and 0x02 in attribute Hidden then skip current entry
         {
            continue;
         }
         printEntry(p[i],name,type); //call the print current entry
    }

}


/*
printEntry
parameter: the entry to the directoy
an entry will maybe as a file
print entry from parameter
*/
void printEntry(directoryEntry entry, char* name, char* type)
{
if(strcmp(name, "") ==00 && strcmp(type, "") ==0){
    if((entry.attribute & 0x02) != 0x02)
    {
     //   printf("Name: %s\n",entry.fileName);
        if(strcmp("entry.ext","   ")==0)
        {
            printf("   ");

        }//in if
        else
        {
           // printf("Extension: .%s",entry.ext);
        }
	printf("\n");
	printf("Attribute: %d\n" , entry.attribute);
	printf("Reserved: %d\n" , entry.reserved);
	printf("Time of Creation: %d\n" , entry.creationTime);
	printf("Creation Date: %d\n" , entry.creationDate);
	printf("Last Access Date: %d\n" , entry.lastAccessDate);
	printf("last Write Time: %d\n" , entry.lastWriteTime);
	printf("Last Write Date: %d\n" , entry.lastWriteDate);
	printf("First Logical Cluster: %d\n" , entry.firstCluster);
	printf("File size: %d\n" , entry.fileSize);

        //printf("\t%d\t\t%d\n",entry.firstCluster, entry.fileSize);
	printf("\n");
	}//seconf
	
    }
if(strcmp(name, "") ==00){
	//printf("In txt: %s",entry.fileName);
	if(strcasecmp(type,entry.ext) == 0){
		if((entry.attribute & 0x02) != 0x02)
    {
        printf("Name: %s\n",entry.fileName);
        if(strcmp("entry.ext","   ")==0)
        {
            printf("   ");

        }//in if
        else
        {
            printf("Extension: .%s",entry.ext);
        }
	printf("\n");
	printf("Attribute: %d\n" , entry.attribute);
	printf("Reserved: %d\n" , entry.reserved);
	printf("Time of Creation: %d\n" , entry.creationTime);
	printf("Creation Date: %d\n" , entry.creationDate);
	printf("Last Access Date: %d\n" , entry.lastAccessDate);
	printf("last Write Time: %d\n" , entry.lastWriteTime);
	printf("Last Write Date: %d\n" , entry.lastWriteDate);
	printf("First Logical Cluster: %d\n" , entry.firstCluster);
	printf("File size: %d\n" , entry.fileSize);

        //printf("\t%d\t\t%d\n",entry.firstCluster, entry.fileSize);
	printf("\n");
	}//seconf
	}
	

}

if(strcmp(type, "") ==00){

	if(strcasecmp(name,entry.fileName) == 0){
		if((entry.attribute & 0x02) != 0x02)
    {
        printf("Name: %s\n",entry.fileName);
        if(strcmp("entry.ext","   ")==0)
        {
            printf("   ");

        }//in if
        else
        {
            printf("Extension: .%s",entry.ext);
        }
	printf("\n");
	printf("Attribute: %d\n" , entry.attribute);
	printf("Reserved: %d\n" , entry.reserved);
	printf("Time of Creation: %d\n" , entry.creationTime);
	printf("Creation Date: %d\n" , entry.creationDate);
	printf("Last Access Date: %d\n" , entry.lastAccessDate);
	printf("last Write Time: %d\n" , entry.lastWriteTime);
	printf("Last Write Date: %d\n" , entry.lastWriteDate);
	printf("First Logical Cluster: %d\n" , entry.firstCluster);
	printf("File size: %d\n" , entry.fileSize);

        //printf("\t%d\t\t%d\n",entry.firstCluster, entry.fileSize);
	printf("\n");
	}//seconf
	}
	
	

}


}


void printBootSector()
{

	printf("Bytes per sector: %i\n",bs.bytesPerSector);
	printf("Sectores per Cluster: %i\n",bs.sectorsPerCluster);
	printf("number of reserved sectors : %i\n",bs.numOfReservedSectors);
	printf("number of FAT copies : %i\n",bs.numOfFatCopies);
	printf("maximum number of root directory entries : %i\n",bs.maxNumberOfRootDirect);
	printf("total # of secotrs in file system : %i\n",bs.totalNumOfSectors);
	printf("number of sectors per FAT : %i\n",bs.numOfSectorsPerFat);
	printf("sectors per track : %i\n",bs.sectorsPerTrack);
	printf("number of heads : %i\n",bs.numOfHeads);
	//printf("number of hidden sectors : %i\n",numOfHiddenSectors);
	printf("total sector count for FAT32 : %i\n",bs.totalSectorCount);
	printf("boot signature : %i\n",bs.bootSignature);
	printf("volume ID: %i\n" , bs.volumeID);
	printf("volume label : %s\n",bs.volumeLabel);
	printf("File system type: %s \n", bs.fileSysType);
}

int Equals( const char* str1, const char* str2)
{
	for( int i=0; i<8;i++)
	{
		if (str1[i]== '\0' || str2[i]=='\0')
			return 1;
		if (str1[i] != str2[i])
			return 0;
	}
	return 1;
}

int findFile(FILE* source, int cluster, const char* file)
{
	int sector=cluster;
	int i =0;
	while (1)
	{
		for (i=0;i<16;i++){
			fseek(source,sector*512+i*32, SEEK_SET);
			char fileName[9];
			fread(fileName,sizeof(char),8,source);
			fileName[8]='\0';

		if(((unsigned char)fileName[0])==0xe5) { //0xe5 refers to the first character
			continue;
		}else if (fileName[0]== 0x00){ //0x00 first character
			break;
		}
		char e[4];
		fread(e,sizeof(char),3,source);
		e[3]='\0';
		int att=fgetc(source);

		if(att &0x02){ //0x02 is hidden attribute
			continue;
		}
		if(Equals(file,fileName) ==1) {
			fseek(source,sector*512+i*32,SEEK_SET);
			return 0;

		}
		}
	if(sector>=31) {
		sector=FAT[sector-31];
		if(sector>= 0xFF8){ //0xff8 last cluster in the file
			break;
		}
	}else{
		sector++;
		if(sector ==31) {
			break;
		}
	}
	}
	return -1;
}


int renameFile(FILE* source, const char* old, const char* new)
{
	int found= findFile(source, directory, old);
	if (found==0){
		fprintf(source, "%8s", new);
		return 0; 
	}
	else return -1;
}



void change_directory(char* name)
{
    strcat(name,"     ");
    printf("%s",name);
    if(name == NULL)
    {
        if(curr != root)
        {
            free(curr);
        }
        moveToSector(19);
        curr = root;
        sizeOfCurr = 272;
        startOfCurr = 14;
    }
    else
    {
        //char* ext ="";
        int indexOfEntry=-1;
        int currSector = 0;
        int i;
        for(i=0 ; i<sizeOfCurr; i++)
        {
            if(((unsigned char)curr[i].fileName[0]) == 0xE5)
            {
                continue;
            }
            else if(((unsigned char)curr[i].fileName[0]) == 0x00)
            {
                break;
            }
            else if(strcmp(name,curr[i].fileName) == 0)
            {
                indexOfEntry = i;
            }
        }
         printf("%d",indexOfEntry);
        if(indexOfEntry != -1)
        {
             int sectors = numberOfSectors(curr[indexOfEntry].firstCluster);
             int num_entries = sectors * 16;
             directoryEntry *entries = malloc(sizeof(directoryEntry) *num_entries);
             currSector = curr[indexOfEntry].firstCluster;
             locateDirectory(entries, num_entries, currSector);
             startOfCurr = curr[indexOfEntry].firstCluster;
            
             if(curr != root)
             {
                 free(curr);
             }
             curr = entries;
             sizeOfCurr = num_entries;
		printf("\nDirectory changed.\n");

        }
        else
        {
         printf("\nNo such subdirectory.\n");
        }
        
    }
}

void locateDirectory(directoryEntry* directory, int numberOfEntries, int firstSector)
{ 
    int i, currentSector;
    currentSector = firstSector;
    moveToSector(31+firstSector);
    for(i=0; i < numberOfEntries; i++)
    {
        if(i != 0 &&  i%512 ==0)
        {
            currentSector = FAT[currentSector];
            moveToSector(31+ currentSector);
        }
        fread(directory[i].fileName,1,8,fpointer);
            fread(directory[i].ext,1,3,fpointer);
            directory[i].attribute=getInt(1);

	    directory[i].reserved = getInt(2);
            directory[i].creationTime = getInt(2);
            directory[i].creationDate = getInt(2);
            directory[i].lastAccessDate = getInt(2);
            directory[i].lastWriteTime = getInt(2);
            directory[i].lastWriteDate = getInt(2);
            fseek(fpointer,2,SEEK_CUR);
            directory[i].firstCluster = getInt(2);
            directory[i].fileSize=getInt(4);
    }
}



/*void change_directory(){
	char* new_directory;
	//asks user for new directory
	printf("\nWhat is the new directory?\n");
	scanf("%s",&new_directory);
	//checks if found
	if(findfile(fpointer,directory,new_directory)==0){
		//change to new directory
	}
	else{
		printf("\nThat directory does not exist.");
	}

}


Print entire contents of current directory
shows filename, extension, logical file size, and starting starting logical cluster

alternatively the user can specify filename/type with or without asterick wildcards such as:
"list README.BAT" or "list *.txt"
this way only info for that specified file or subset of files/directories will be displayed.


*/
void list_directory(){
	char* nullVal = "";
	char* nullVal2 = "";
	char str5[8];
	char ext[3];
	char nameOfFile[10];
	int len1 = 0;
	int len2 = 0;
	int i;
	int j = 0;
	//int returnInt = 0;
	printf("Do you want to specify a file name or type? (y or n)\n");
	scanf("%s",str5);

	if(strcmp(str5,"y") == 0){
		char str4[40];
		printf("Enter a filename (1984) or type (ex: TXT)\n");
		scanf("%s", str4);
		if(strcasecmp(str4, "TXT") == 0 || strcasecmp(str4, "BAT") == 0 || strcasecmp(str4, "C") == 0 || strcasecmp(str4, "*") == 0){
			printf("TYPE\n");
			//findType(root,272,str4);
			printDir(curr,272,nullVal,str4);
	

		}//inner if
		else{
			len1 = strlen(str4);
			printf("Filename was entered, please enter type for that file (ex:TXT)\n");
			scanf("%s", ext);
			len2 = strlen(ext);

			j = 11 - (len2+len1);
			//printf("%d",j);
			for(i = 0; i <j; i++){
				printf("%d", i);
				strcat(str4," ");

			}

			strcat(str4, ext);
			strncpy(nameOfFile, str4, 13);
			
			printf("%s",nameOfFile);
			/*returnInt = findFile(fpointer, directory, nameOfFile);
			if(returnInt == -1){
				printf("The file you entered does not exist");
			}
			else{
*/
				printf("\nPrinting information from: %s\n", nameOfFile);
				printDir(curr,272,nameOfFile,nullVal2);
			//}
			

		}//inner else
	}//outer if
	
	else{ 
		
		printf("\nPrinting current directory\n");
		printDir(curr,272,nullVal, nullVal2);

	}
	




}

//given type in char find the entries that extension is of that type and print them
/*void findType(char* type){
	char str6[3];
	if(strcasecmp(type, "*.TXT") ==0){
		strncpy(str6,"TXT", 8);
		if(strcmp(root.ext,str6)==0){
			printf("SDVBFV");
		}

		
	}
	else if(strcasecmp(type, "*.BAT") ==0){
		strncpy(str6,"BAT", 8);
	}
	else if(strcasecmp(type, "*.C") ==0){
		strncpy(str6,"C", 8);
	}

}
*/

/*
	help:
	prints out help command to user. i will update this to be more detailed when program is complete
	param - void
	return - void
*/

void help(){
	printf("\n HELP:");
	printf("\n type 'cd' to change directory.");
	printf("\n type 'help' to view help.");
	printf("\n type 'mv' to rename a file");
	printf("\n type 'pbs' to print boot sector");
	printf("\n type 'ls' to list directory");
	printf("\n type 'prd' to print root directory");
	printf("\n type 'type' for type command\n");
}


