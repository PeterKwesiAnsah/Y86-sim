#include <stdio.h>
#include <stdbool.h>
#include <memory.h>
#include "memory.h"
#include "tools.h"

static unsigned int memory[MEMSIZE];


/**
 * [clearMemory description]
 */
void clearMemory() {
    memset(memory, 0, MEMSIZE * sizeof(*memory));
}

/**
 * [store description]
 * @param address  [description]
 * @param value    [description]
 * @param memError [description]
 */
void store(int address, unsigned int value, bool * memError) {
    if (address < 0 || address > 1024) {
        (*memError) = true;
    } else {
        (*memError) = false;
        memory[address] = value;
    }
}

/**
 * [fetch description]
 * @param  address  [description]
 * @param  memError [description]
 * @return          [description]
 */
unsigned int fetch(int address, bool * memError) {
    if (address < 0 || address > 1024) {
        (*memError) = true;
    } else {
        (*memError) = false;
        return memory[address];
    }
}

/**
 * [getByte description]
 * @param  address  [description]
 * @param  memError [description]
 * @return          [description]
 */
unsigned char getByte(int address, bool * memError) {
    if (address < 0 || address > 4095) {
        (*memError) = true;
        return 0;
    } else {
        (*memError) = false;
        int w_addr = fetch((address / 4), memError);
        char * addr = (char*)&w_addr;
        char retn = addr[(address % 4)];
        return retn;
    }
}

/**
 * [putByte description]
 * @param address  [description]
 * @param value    [description]
 * @param memError [description]
 */
void putByte(int address, unsigned char value, bool * memError) {
    if (address < 0 || address > 4095) {
        (*memError) = true;
    } else {
        (*memError) = false;
        int w_addr = fetch((address / 4), memError);
        char * addr = (char*)&w_addr;
        addr[(address % 4)] = value;
        store(address / 4, w_addr, memError);
    }
}

/**
 * [getWord description]
 * @param  address  [description]
 * @param  memError [description]
 * @return          [description]
 */
unsigned int getWord(int address, bool * memError) {
    if (address < 0 || address > 4095 || (address % 4) != 0) {
        (*memError) = true;
        return 0;
    } else {
        (*memError) = false;
        return fetch(address / 4, memError);
    }
}

/**
 * [putWord description]
 * @param address  [description]
 * @param value    [description]
 * @param memError [description]
 */
void putWord(int address, unsigned int value, bool * memError) {
    if (address < 0 || address > 4095 || (address % 4) != 0) {
        (*memError) = true;
    } else {
        (*memError) = false;
        store((address / 4), value, memError);
    }
}
