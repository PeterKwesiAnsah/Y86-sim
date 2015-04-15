#include <stdio.h>
#include <stdbool.h>

#include "forwarding.h"
#include "instructions.h"
#include "registers.h"
#include "dump.h"
#include "tools.h"

#include "writebackStage.h"


static wregister W;


/**
 * Get the state of the writeback stage struct.
 * @return The state of the writeback stage struct
 */
wregister getWregister() {
    return W;
}

/**
 * Reset the writeback stage struct to a default state.
 */
void clearWregister() {
    W.stat = S_AOK;
    W.icode = I_NOP;
    W.valE = 0;
    W.valM = 0;
    W.dstE = RNONE;
    W.dstM = RNONE;
}

/**
 * Update the writeback register with the given parameters.
 * @param stat  Status code
 * @param icode Instruction code
 * @param valE  Value E
 * @param valM  Value M
 * @param dstE  Destination E
 * @param dstM  Destination M
 */
void updateWregister(
    unsigned int stat, unsigned int icode, unsigned int valE,
    unsigned int valM, unsigned int dstE, unsigned int dstM
) {
    W.stat = stat;
    W.icode = icode;
    W.valE = valE;
    W.valM = valM;
    W.dstE = dstE;
    W.dstM = dstM;
}

/**
 * Execute the writeback stage.
 * @param  FORW A pointer to the forwarding struct `forwarding.h`
 * @return      True if processor should halt, otherwise false
 */
bool writebackStage(forwardType * FORW) {
    FORW->W_dstE = W.dstE;
    FORW->W_valE = W.valE;
    FORW->W_icode = W.icode;
    FORW->W_valM = W.valM;
    FORW->W_dstM = W.dstM;
    FORW->W_stat = W.stat;

    if (W.stat == S_INS) {
        printf("Invalid instruction\n");
        dumpProgramRegisters();
        dumpProcessorRegisters();
        dumpMemory();
        return true;
    } else if (W.stat == S_ADR) {
        printf("Invalid memory address\n");
        dumpProgramRegisters();
        dumpProcessorRegisters();
        dumpMemory();
        return true;
    } else if (W.stat == S_HLT) {
        return true;
    } else {
        // Status AOK
    }

    setRegister(W.dstE, W.valE);
    setRegister(W.dstM, W.valM);

    if (W.icode == I_DUMP) {
        unsigned int dump_flags = getBits(0, 3, W.valE);
        if (dump_flags & 0x1) {
            dumpProgramRegisters();
        }
        if (dump_flags & 0x2) {
            dumpProcessorRegisters();
        }
        if (dump_flags & 0x4) {
            dumpMemory();
        }
    }
    return false;
}