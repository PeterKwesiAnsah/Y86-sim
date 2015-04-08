#include <stdio.h>
#include <memory.h>
#include "registers.h"
#include "tools.h"

static unsigned int registers[REGSIZE];
static unsigned int cc;

/**
 * [setCC description]
 * @param bitNumber [description]
 * @param value     [description]
 */
void setCC(unsigned int bitNumber, unsigned int value) {
    if (bitNumber == OF || bitNumber == SF || bitNumber == ZF) {
        cc = assignOneBit(bitNumber, value, cc);
    }
}

/**
 * [getCC description]
 * @param  bitNumber [description]
 * @return           [description]
 */
unsigned int getCC(unsigned int bitNumber) {
    if (bitNumber == OF || bitNumber == SF || bitNumber == ZF) {
        return getBits(bitNumber, bitNumber, cc);
    }
}

/**
 * [clearRegisters description]
 */
void clearRegisters() {
    memset(registers, 0, REGSIZE);
}

/**
 * [clearCC description]
 */
void clearCC() {
    setCC(OF, 0);
    setCC(ZF, 0);
    setCC(SF, 0);
}

/**
 * [getRegister description]
 * @param  regNum [description]
 * @return        [description]
 */
unsigned int getRegister(int regNum) {
    if (regNum >= 0 && regNum < REGSIZE) {
        return registers[regNum];
    }
}

/**
 * [setRegister description]
 * @param regNum   [description]
 * @param regValue [description]
 */
void setRegister(int regNum, unsigned int regValue) {
    if (regNum >= 0 && regNum < REGSIZE) {
        registers[regNum] = regValue;
    }
}
