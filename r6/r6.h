#pragma once
#include<stdio.h>


typedef struct directoryEntry{
    char fileName[8];
    char ext[3];
    int attribute;
    int fileSize;
    int firstCluster;
    uint16_t reserved;
    uint16_t creationTime;
    uint16_t creationDate;
    uint16_t lastAccessDate;
    uint16_t ignore;
    uint16_t lastWriteTime;
    uint16_t lastWriteDate;
}directoryEntry;

typedef struct bootSector{
	uint8_t ignore[11];
	uint16_t bytesPerSector;	
	uint8_t sectorsPerCluster;
	uint16_t numOfReservedSectors;
	uint8_t numOfFatCopies;
	uint16_t maxNumberOfRootDirect;
	uint16_t totalNumOfSectors;
	uint8_t ignore2[1];
	uint16_t numOfSectorsPerFat;
	uint16_t sectorsPerTrack;
	uint16_t numOfHeads;
	uint32_t numOfHiddenSectors;
	uint32_t totalSectorCount;
	uint8_t ignore3[2];
	uint32_t totalSCforF32;
	uint8_t ignore4[2];
	uint8_t bootSignature;
	uint32_t volumeID;
	unsigned char volumeLabel[12];
	unsigned char fileSysType[9];
	uint8_t ignore5[450];

}bootSector;


FILE* fpointer;
int fatTale;
directoryEntry root[224];
directoryEntry* curr;
int sizeOfCurr;
int startOfCurr;

//colours
#define color_red "\x1b[31m"
#define color_green "\x1b[32m"
#define color_blue "\x1b[34m"
#define color_yellow "\x1b[33m"
#define color_clear "\x1b[0m" //resets the color

int getInt(int sizeOfbytes);
void moveToSector(int sector);
void printRoot(void);
void printDir(directoryEntry *p, int num, char* name, char* type);
void printEntry(directoryEntry entry, char* name, char* type);
void type(char* name, char* ext);
void init();
void Type(char* name, char*ext);
int numberOfSectors(int startingSector);
void locateDirectory(directoryEntry* directory,int numberOfentries,int firstSector);

void interface(FILE*);
void command_line(FILE*);
void help();
void printBootSector();
int renameFile();
int Equals();
void change_directory(char* name);
void list_directory();

int findFile(FILE* source, int cluster, const char* file);




