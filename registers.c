#include <stdio.h>
#include "registers.h"

static unsigned int registers[REGSIZE];

void clearRegisters() {
    registers[REGSIZE] = {0};
}

unsigned int getRegister(int regNum) {
    if (regNum >= 0 && regNum < REGSIZE) {
        return registers[regNum];
    }
}

void setRegister(int regNum, unsigned int regValue) {
    if (regNum >= 0 && regNum < REGSIZE) {
        registers[regNum] = regValue;
    }
}
