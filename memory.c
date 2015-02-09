#include <stdio.h>
#include <stdbool.h>
#include <memory.h>
#include "memory.h"
#include "tools.h"

static unsigned int memory[MEMSIZE];

void clearMemory() {
    memset(memory, 0, MEMSIZE);
}

void store(int address, unsigned int value, bool * memError) {
    putByte(address, value, &memError);
}

unsigned int fetch(int address, bool * memError) {
    return getByte(address, &memError);
}

unsigned char getByte(int address, bool * memError) {
    if (address >= 0 && address < MEMSIZE) {
        *memError = false;
        return memory[address];
    } else {
        *memError = true;
    }
}

void putByte(int address, unsigned char value, bool * memError) {
    if (address >= 0 && address < MEMSIZE) {
        *memError = false;
        memory[address] = value;
    } else {
        *memError = true;
    }
}

unsigned int getWord(int address, bool * memError) {
    if ((address >= 0 && address < MEMSIZE) && ((address % 4) == 0)) {
        *memError = false;
        unsigned char bytes[4] = {
            memory[address], memory[address+1],
            memory[address+2], memory[address+3]
        };
        return join_bytes(bytes);
    } else {
        *memError = true;
    }
}

void putWord(int address, unsigned int value, bool * memError) {
    if ((address >= 0 && address < MEMSIZE) && ((address % 4) == 0)) {
        *memError = false;
        unsigned char bytes[4];
        get_bytes(value, bytes);
        unsigned int i;
        for (i = 0; i < 4; i++) {
            memory[address+i] = bytes[i];
        }
    } else {
        *memError = true;
    }
}
