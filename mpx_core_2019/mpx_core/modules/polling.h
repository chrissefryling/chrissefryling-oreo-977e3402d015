#include <string.h>
#include <stdint.h>
#include <system.h>

#include <core/io.h>
#include <core/serial.h>
#include <core/tables.h>
#include <core/interrupts.h>
#include <mem/heap.h>
#include <mem/paging.h>
#include <stdbool.h>

#include "mpx_supt.h"

//procedure: polling 
char serial_input(char * buffer, int * count);
