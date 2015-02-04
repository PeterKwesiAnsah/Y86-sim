#include <stdio.h>
#include <stdbool.h>
#include "tools.h"
#include "memory.h"
#include "dump.h"

void initialize();

int main(int argv, char * args[])
{
    bool memError;
    unsigned int memValue;

    //*******code to test initialize and get routines
    initialize();
    dumpProgramRegisters();
    dumpProcessorRegisters();
    dumpMemory();

    //*******code to test register routines 
    setRegister(0, 0x11111111);
    setRegister(1, 0x11111112);
    setRegister(2, 0x11111113);
    setRegister(3, 0x11111114);
    setRegister(4, 0x11111115);
    setRegister(5, 0x11111116);
    setRegister(6, 0x11111117);
    setRegister(7, 0x11111118);
    dumpProgramRegisters();

    printf("\n");
    printf("getRegister(0) = %x\n", getRegister(0));
    printf("getRegister(1) = %x\n", getRegister(1));
    printf("getRegister(2) = %x\n", getRegister(2));
    printf("getRegister(3) = %x\n", getRegister(3));
    printf("getRegister(4) = %x\n", getRegister(4));
    printf("getRegister(5) = %x\n", getRegister(5));
    printf("getRegister(6) = %x\n", getRegister(6));
    printf("getRegister(7) = %x\n", getRegister(7));

    //your program should check to make sure the register number is legal
    setRegister(8, 0x88888888);
    
    //*********code to test memory routines 
    store(1, 0x12345678, &memError);
    memValue = fetch(1, &memError);
    printf("fetch(1) = %x, memory error: %d\n", memValue, memError);
    memValue = getWord(4, &memError);
    printf("getWord(4) = %x, memory error: %d\n", memValue, memError);

    putWord(8, 0x87654321, &memError);
    memValue = getWord(8, &memError);
    printf("getWord(8) = %x, memory error: %d\n", memValue, memError);
    putByte(8, 0xff, &memError);
    memValue = getWord(8, &memError);
    printf("getWord(8) = %x, memory error: %d\n", memValue, memError);
    putByte(9, 0xff, &memError);
    memValue = getWord(8, &memError);
    printf("getWord(8) = %x, memory error: %d\n", memValue, memError);
    putByte(10, 0xff, &memError);
    memValue = getWord(8, &memError);
    printf("getWord(8) = %x, memory error: %d\n", memValue, memError);
    putByte(11, 0xff, &memError);
    memValue = getWord(8, &memError);
    printf("getWord(8) = %x, memory error: %d\n", memValue, memError);
    putWord(8, 0x87654321, &memError);
    memValue = getWord(8, &memError);
    printf("getWord(8) = %x, memory error: %d\n", memValue,
           memError);
    int negOne = 0xffffffff;
    putByte(11, negOne, &memError);      //should only use low order byte
    memValue = getWord(8, &memError);
    printf("getWord(8) = %x, memory error: %d\n", memValue, memError);
    putByte(10, negOne, &memError);     //should only use low order byte
    memValue = getWord(8, &memError);
    printf("getWord(8) = %x, memory error: %d\n", memValue, memError);
    putByte(9, negOne, &memError);      //should only use low order byte
    memValue = getWord(8, &memError);
    printf("getWord(8) = %x, memory error: %d\n", memValue, memError);
    putByte(8, negOne, &memError);      //should only use low order byte
    memValue = getWord(8, &memError);
    printf("getWord(8) = %x, memory error: %d\n", memValue, memError);
    dumpMemory();
     
    initialize();
    //putWord error testing
    //out of bounds
    putWord(0x1000, 0x87654321, &memError);
    printf("memory error: %d\n", memError);
    //this one should be good
    memValue = getWord(8, &memError);
    printf("getWord(8) = %x, memory error: %d\n", memValue, memError);
    //out of bounds address
    putWord(-3, 0x87654321, &memError);
    printf("memory error: %d\n", memError);
    //this one should be good
    memValue = getWord(8, &memError);
    printf("getWord(8) = %x, memory error: %d\n", memValue, memError);
    //address of word is odd
    putWord(3, 0x87654321, &memError);
    printf("memory error: %d\n", memError);

    //getWord error testing
    //this one should be good
    memValue = getWord(8, &memError);
    printf("getWord(8) = %x, memory error: %d\n", memValue, memError);
    //address is odd
    memValue = getWord(9, &memError);
    printf("getWord(9) = %x, memory error: %d\n", memValue, memError);
    //this one should be good
    memValue = getWord(8, &memError);
    printf("getWord(8) = %x, memory error: %d\n", memValue, memError);
    //address is out of bounds
    memValue = getWord(0x1000, &memError);
    printf("getWord(0x1000) = %x, memory error: %d\n", memValue, memError);
    //this one should be good
    memValue = getWord(8, &memError);
    printf("getWord(8) = %x, memory error: %d\n", memValue, memError);
    //address is out of bounds
    memValue = getWord(-9, &memError);
    printf("getWord(-9) = %x, memory error: %d\n", memValue, memError);

    //getByte error testing 
    //this one should be good
    memValue = getWord(8, &memError);
    printf("getWord(8) = %x, memory error: %d\n", memValue, memError);
    //address is out of bounds
    memValue = getByte(-10, &memError);
    printf("getByte(-10) = %x, memory error: %d\n", memValue, memError);
    //this one should be good
    memValue = getWord(8, &memError);
    printf("getWord(8) = %x, memory error: %d\n", memValue, memError);
    //address is out of bounds
    memValue = getByte(0x4000, &memError);
    printf("getByte(0x4000) = %x, memory error: %d\n", memValue, memError);

    //putByte error testing
    //out of bounds
    putByte(0x4000, 0x87, &memError);
    printf("memory error: %d\n", memError);
    //this one should be good
    memValue = getWord(8, &memError);
    printf("getWord(8) = %x, memory error: %d\n", memValue, memError);
    //out of bounds address
    putByte(-3, 0x87, &memError);
    printf("memory error: %d\n", memError);
    //this one should be good
    memValue = getWord(8, &memError);
    printf("getWord(8) = %x, memory error: %d\n", memValue, memError);
}

void initialize()
{
    clearMemory();
    clearRegisters();
    clearFregister();
    clearDregister();
    clearEregister();
    clearMregister();
    clearWregister();
}


