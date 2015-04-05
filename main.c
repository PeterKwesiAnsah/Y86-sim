#include <stdio.h>
#include <stdbool.h>
#include "tools.h"
#include "memory.h"
#include "dump.h"
#include "loader.h"
#include "forwarding.h"

void initialize();

int main(int argv, char * args[]) {
    forwardType FORW;
    unsigned int clockCount = 0;
    initialize();
    if (!load(argv, args)) {
        dumpMemory();
        exit(1);
    } else {
        bool dead = false;
        while (!dead) {
            dead = writebackStage(&FORW);
            memoryStage(&FORW);
            executeStage(&FORW);
            decodeStage(&FORW);
            fetchStage(&FORW);
            // getchar();
            clockCount++;
        }
        printf("\nTotal clock cycles = %d\n", clockCount);
    }
}

void initialize() {
    clearMemory();
    clearRegisters();
    clearFregister();
    clearDregister();
    clearEregister();
    initExecFuncs();
    clearMregister();
    clearWregister();
}


