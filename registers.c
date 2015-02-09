#include <stdio.h>
#include <memory.h>
#include "registers.h"
#include "tools.h"

static unsigned int registers[REGSIZE];
static unsigned int cc = 0x0;

void setCC(unsigned int bitNumber, unsigned int value) {
    if (bitNumber == OF || bitNumber == SF || bitNumber == ZF) {
        cc = setBits(bitNumber, bitNumber, cc);
    }
}

unsigned int getCC(unsigned int bitNumber) {
    if (bitNumber == OF || bitNumber == SF || bitNumber == ZF) {
        return getBits(bitNumber, bitNumber, cc);
    }
}

void clearRegisters() {
    memset(registers, 0, REGSIZE);
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
