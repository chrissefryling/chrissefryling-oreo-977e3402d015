/** @file r1.h
*/
#pragma once

#include "menu.h"
#include <system.h>
#include <string.h>
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

//functions 
void help();
void version();
void change_color();
void shutdown();
char* itoa(int num, char* str, int base);
void reverse(char str[], int length);
void swap(char *str_x,char *str_y);
void clean_screen();
void getDate();
void getTime();
void setTime ();
void setDate();
void date_validation(int type, char *p, int min, int max);
void date_update(int byte, int value);
void print();
int hexConvert(int hexIndecimal);
