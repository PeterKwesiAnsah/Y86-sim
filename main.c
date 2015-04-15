#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "forwarding.h"
#include "registers.h"
#include "loader.h"
#include "memory.h"
#include "dump.h"
#include "tools.h"

#include "fetchStage.h"
#include "decodeStage.h"
#include "executeStage.h"
#include "memoryStage.h"
#include "writebackStage.h"


static forwardType FORW;
void initialize();


/**
 * Main function to start and execute processor.
 * @param  argv Count of cammand-line arguments
 * @param  args Command-line arguments
 * @return      Exit status
 */
int main(int argv, char * args[]) {
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
            clockCount++;
        }
        printf("\nTotal clock cycles = %d\n", clockCount);
    }
}

/**
 * Initialize the processor before executing stages.
 */
void initialize() {
    clearMemory();
    clearRegisters();
    clearCC();
    clearFregister();
    clearDregister();
    clearEregister();
    initExecFuncs();
    clearMregister();
    clearWregister();
}
