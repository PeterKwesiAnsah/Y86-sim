#include <stdio.h>
#include <stdbool.h>

#include "forwarding.h"
#include "instructions.h"
#include "registers.h"
#include "memory.h"
#include "tools.h"

#include "memoryStage.h"
#include "writebackStage.h"


static mregister M;
static bool canUpdateMemory = true;


/**
 * Get the state of the memory stage struct.
 * @return The state of the memory stage struct
 */
mregister getMregister() {
    return M;
}

/**
 * Reset the memory stage struct to a default state.
 */
void clearMregister() {
    canUpdateMemory = true;
    M.stat = S_AOK;
    M.icode = I_NOP;
    M.Cnd = 0;
    M.valE = 0;
    M.valA = 0;
    M.dstE = RNONE;
    M.dstM = RNONE;
}

/**
 * Update the memory stage with the given parameters.
 * @param stat  Status code
 * @param icode Instruction code
 * @param Cnd   Condition code
 * @param valE  Value E
 * @param valA  Value A
 * @param dstE  Destination E
 * @param dstM  Destination M
 */
void updateMregister(
    unsigned int stat, unsigned int icode, unsigned int Cnd, unsigned int valE,
    unsigned int valA, unsigned int dstE, unsigned int dstM
) {
    M.stat = stat;
    M.icode = icode;
    M.Cnd = Cnd;
    M.valE = valE;
    M.valA = valA;
    M.dstE = dstE;
    M.dstM = dstM;
}

/**
 * Determines if the writeback stage needs to be stalled.
 * @param  FORW A pointer to the forwarding struct `forwarding.h`
 * @return      True if the writeback stage needs to be stalled, otherwise false
 */
bool stall_W(forwardType * FORW) {
    return (
        FORW->W_stat == S_ADR || FORW->W_stat == S_INS || FORW->W_stat == S_HLT
    );
}

/**
 * Controls memory access if an instruction requires a read or write.
 * @param read  Flag to be set to true if allowed to read
 * @param write Flag to be set to true if allowed to write
 */
void mem_ctrl(bool * read, bool * write) {
    if (M.icode == I_MRMOVL || M.icode == I_POPL || M.icode == I_RET) {
        *read = true;
    }
    if (M.icode == I_RMMOVL || M.icode == I_PUSHL || M.icode == I_CALL) {
        *write = true;
    }
}

/**
 * Extract the memory address to read from or write to.
 * @return Required address number
 */
unsigned int mem_addr() {
    if (
        M.icode == I_RMMOVL || M.icode == I_PUSHL ||
        M.icode == I_CALL || M.icode == I_MRMOVL
    ) {
        return M.valE;
    } else if (M.icode == I_POPL || M.icode == I_RET) {
        return M.valA;
    } else {
        return 0;
    }
}

/**
 * Execute the memory stage.
 * @param FORW A pointer to the forwarding struct `forwarding.h`
 */
void memoryStage(forwardType * FORW) {
    if (M.stat == S_INS || M.stat == S_ADR || M.stat == S_HLT) {
        canUpdateMemory = false;
    }
    unsigned int valM = M.valA;
    unsigned int stat = M.stat;

    bool read = false;
    bool write = false;
    bool memError = false;
    mem_ctrl(&read, &write);
    unsigned int memAddr = mem_addr();
    if (read) {
        valM = getWord(memAddr, &memError);
    }
    if (write && canUpdateMemory) {
        putWord(memAddr, M.valA, &memError);
    }
    if (memError) {
        stat = S_ADR;
        canUpdateMemory = false;
    }

    FORW->M_valE = M.valE;
    FORW->m_valM = valM;
    FORW->M_dstE = M.dstE;
    FORW->M_dstM = M.dstM;
    FORW->M_Cnd = M.Cnd;
    FORW->M_icode = M.icode;
    FORW->M_valA = M.valA;
    FORW->m_stat = stat;

    if (!stall_W(FORW)) {
        updateWregister(stat, M.icode, M.valE, valM, M.dstE, M.dstM);
    }
}
