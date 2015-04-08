#include <stdio.h>
#include "forwarding.h"
#include "memoryStage.h"
#include "instructions.h"
#include "registers.h"
#include "tools.h"

static mregister M;
static bool canUpdateMemory = true;

/**
 * [getMregister description]
 * @return [description]
 */
mregister getMregister() {
    return M;
}

/**
 * [clearMregister description]
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

bool stallW(forwardType * FORW) {
    return (
        FORW->W_stat == S_ADR || FORW->W_stat == S_INS || FORW->W_stat == S_HLT
    );
}

void memoryControl(bool * read, bool * write) {
    if (M.icode == I_MRMOVL || M.icode == I_POPL || M.icode == I_RET) {
        *read = true;
    }
    if (M.icode == I_RMMOVL || M.icode == I_PUSHL || M.icode == I_CALL) {
        *write = true;
    }
}

unsigned int memoryAddr() {
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

void memoryStage(forwardType * FORW) {
    if (M.stat == S_INS || M.stat == S_ADR || M.stat == S_HLT) {
        canUpdateMemory = false;
    }
    unsigned int valM = M.valA;
    unsigned int stat = M.stat;

    bool read = false;
    bool write = false;
    bool memError = false;
    memoryControl(&read, &write);
    unsigned int memAddr = memoryAddr();
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

    if (!stallW(FORW)) {
        updateWregister(stat, M.icode, M.valE, valM, M.dstE, M.dstM);
    }

    // printf(
    //     "MEMORY\t\t<stat=0x%.2x, icode=0x%.2x, Cnd=0x%.2x, valE=0x%.2x, valM=0x%.2x, dstE=0x%.2x, dstM=0x%.2x>\n",
    //     M.stat, M.icode, M.Cnd, M.valE, M.valA, M.dstE, M.dstM
    // );
}
