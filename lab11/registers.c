#include <stdio.h>
#include <memory.h>

#include "memory.h"
#include "tools.h"

#include "registers.h"


static unsigned int registers[REGSIZE];
static unsigned int cc;


/**
 * Set a given condition flag to a given value.
 * @param bitNumber Condition flag
 * @param value     Value to be set
 */
void setCC(unsigned int bitNumber, unsigned int value) {
    if (bitNumber == OF || bitNumber == SF || bitNumber == ZF) {
        cc = assignOneBit(bitNumber, value, cc);
    }
}

/**
 * Get the current state of a given condition flag.
 * @param  bitNumber Condition flag
 * @return           Current state of the given flag
 */
unsigned int getCC(unsigned int bitNumber) {
    if (bitNumber == OF || bitNumber == SF || bitNumber == ZF) {
        return getBits(bitNumber, bitNumber, cc);
    }
}

/**
 * Reset all registers to their default state.
 */
void clearRegisters() {
    memset(registers, 0, REGSIZE);
}

/**
 * Reset all condition codes to their default state.
 */
void clearCC() {
    setCC(OF, 0);
    setCC(ZF, 0);
    setCC(SF, 0);
}

/**
 * Get a given register's current value.
 * @param  regNum Register to retrieve
 * @return        Current value of the given register
 */
unsigned int getRegister(int regNum) {
    if (regNum >= 0 && regNum < REGSIZE) {
        return registers[regNum];
    }
}

/**
 * Set a given register's value to a given value.
 * @param regNum   Register to set
 * @param regValue Value to set
 */
void setRegister(int regNum, unsigned int regValue) {
    if (regNum >= 0 && regNum < REGSIZE) {
        registers[regNum] = regValue;
    }
}
