#include <stdbool.h>

#define MEMSIZE 1024     //1024 words of memory
#define MEMBYTE 4095
#ifndef MEMORY_H
#define MEMORY_H
unsigned char getByte(int address, bool * memError);
void putByte(int address, unsigned char value, bool * memError);
void clearMemory();
unsigned int getWord(int address, bool * memError);
void putWord(int address, unsigned int value, bool * memError);
#endif

