#include <stdio.h>
#include <stdbool.h>

#include "forwarding.h"
#include "instructions.h"
#include "registers.h"
#include "tools.h"

#include "executeStage.h"
#include "memoryStage.h"


static int (*funcptr[16])();
static eregister E;
static bool cnd;
static bool canUpdateCC;


/**
 * Initialize the execution function array.
 */
void initExecFuncs() {
    funcptr[I_HALT] = &p__none;
    funcptr[I_NOP] = &p__none;
    funcptr[I_RRMOVL] = &p__rrmovl;
    funcptr[I_RMMOVL] = &p__rmmovl;
    funcptr[I_IRMOVL] = &p__irmovl;
    funcptr[I_MRMOVL] = &p__mrmovl;
    funcptr[I_OPL] = &p__opl;
    funcptr[I_JXX] = &p__jxx;
    funcptr[I_CMOVXX] = &p__rrmovl;
    funcptr[I_CALL] = &p__call;
    funcptr[I_RET] = &p__ret;
    funcptr[I_PUSHL] = &p__push;
    funcptr[I_POPL] = &p__pop;
    funcptr[I_DUMP] = &p__dump;
}

/**
 * Get the state of the execute stage struct.
 * @return The state of the execute stage struct
 */
eregister getEregister() {
    return E;
}

/**
 * Reset the execute stage struct to a default state.
 */
void clearEregister() {
    canUpdateCC = true;
    E.stat = S_AOK;
    E.icode = I_NOP;
    E.ifun = 0;
    E.valC = 0;
    E.valA = 0;
    E.valB = 0;
    E.dstM = RNONE;
    E.dstE = RNONE;
    E.srcA = RNONE;
    E.srcB = RNONE;
}

/**
 * Determines if the memory stage needs to be stalled.
 * @return False
 */
bool stall_M() {
    return false;
}

/**
 * Determines if the memory stage needs to be bubbled
 * @param  FORW A pointer to the forwarding struct `forwarding.h`
 * @return      True if the memeory stage needs to be bubbled, otherwise false
 */
bool bubble_M(forwardType * FORW) {
    return (
        (
            FORW->m_stat == S_ADR || FORW->m_stat == S_INS ||
            FORW->m_stat == S_HLT
        ) || (
            FORW->W_stat == S_ADR || FORW->W_stat == S_INS ||
            FORW->W_stat == S_HLT
        )
    );
}

/**
 * Perform a dump instruction.
 * @return Value C
 */
int p__dump() {
    return E.valC;
}

/**
 * Perform nothing.
 * @return 0
 */
int p__none() {
    return 0;
}

/**
 * Perform an irmovl instruction.
 * @return Value C
 */
int p__irmovl() {
    return E.valC;
}

/**
 * Perform an rrmovl instruction.
 * @return Value A
 */
int p__rrmovl() {
    setCnd(E.ifun);
    return E.valA;
}

/**
 * Perform an rmmovl instruction.
 * @return Value C + Value B
 */
int p__rmmovl() {
    return E.valC + E.valB;
}

/**
 * Perform an mrmovl instruction.
 * @return Value C + Value B
 */
int p__mrmovl() {
    return E.valC + E.valB;
}

/**
 * Perform a jump instruction.
 * @return Value A
 */
int p__jxx() {
    setCnd(E.ifun);
    return E.valA;
}

/**
 * Perform a call instruction.
 * @return Value B - 4
 */
int p__call() {
    return E.valB - 4;
}

/**
 * Perform a return isntruction.
 * @return Value B + 4
 */
int p__ret() {
    return E.valB + 4;
}

/**
 * Perform a push instruction.
 * @return -4 + Value B
 */
int p__push() {
    cnd = 0;
    return -4 + E.valB;
}

/**
 * Perform a pop instruction.
 * @return 4 + Value B
 */
int p__pop() {
    cnd = 0;
    return 4 + E.valB;
}

/**
 * Perform an arithmetic instruction.
 * @return Calculated arithmetic function
 */
int p__opl() {
    int retn = 0;
    switch(E.ifun) {
        case OP_ADDL:
            retn = E.valB + E.valA;
            if (
                canUpdateCC && ((
                    isNegative(E.valB) && isNegative(E.valA) &&
                    !isNegative(retn)
                ) || (
                    !isNegative(E.valB) && !isNegative(E.valA) &&
                    isNegative(retn)
                ))
            ) {
                setCC(OF, 1);
            } else if (canUpdateCC) {
                setCC(OF, 0);
            }
            break;
        case OP_SUBL:
            retn = E.valB - E.valA;
            if (
                canUpdateCC && ((
                    isNegative(E.valB) && !isNegative(E.valA) &&
                    !isNegative(retn)
                ) || (
                    !isNegative(E.valB) && isNegative(E.valA) &&
                    isNegative(retn)
                ))
            ) {
                setCC(OF, 1);
            } else if (canUpdateCC) {
                setCC(OF, 0);
            }
            break;
        case OP_ANDL:
            if (canUpdateCC) {
                setCC(OF, 0);
            }
            retn = E.valB & E.valA;
            break;
        case OP_XORL:
            if (canUpdateCC) {
                setCC(OF, 0);
            }
            retn = E.valB ^ E.valA;
            break;
        default:
            E.stat = S_INS;
            break;
    }
    updateCC(retn);
    return retn;
}

/**
 * Update the condition codes.
 * @param val Calculated value from arithmetic instructions.
 */
void updateCC(int val) {
    if (!canUpdateCC) {
        return;
    }
    if (val == 0) {
        setCC(ZF, 1);
        setCC(SF, 0);
    } else if (isNegative(val)) {
        setCC(ZF, 0);
        setCC(SF, 1);
    } else {
        setCC(ZF, 0);
        setCC(SF, 0);
    }
}

/**
 * Set the condition flag states.
 * @param code Flag representing <=, <, ==, !=, >=, >
 */
void setCnd(unsigned int code) {
    switch (code) {
        case 0:
            cnd = true;
            break;
        case CLE:
            cnd = (
                (getCC(SF) && !getCC(OF)) ||
                (!getCC(SF) && getCC(OF)) ||
                getCC(ZF)
            );
            break;
        case CL:
            cnd = (getCC(SF) && !getCC(OF)) || (!getCC(SF) && getCC(OF));
            break;
        case CE:
            cnd = getCC(ZF);
            break;
        case CNE:
            cnd = !getCC(ZF);
            break;
        case CGE:
            cnd = (getCC(SF) & getCC(OF)) || (!getCC(SF) && !getCC(OF));
            break;
        case CG:
            cnd = (
                (getCC(SF) && getCC(OF) && !getCC(ZF)) ||
                (!getCC(SF) && !getCC(OF) && !getCC(ZF))
            );
            break;
        default:
            cnd = false;
            break;
    }
}

/**
 * Update the execute state with the given parameters
 * @param stat  Status code
 * @param icode Instruction code
 * @param ifun  Instruction function
 * @param valC  Value C
 * @param valA  Value A
 * @param valB  Value B
 * @param dstE  Destination E
 * @param dstM  Destination M
 * @param srcA  Source A
 * @param srcB  Source B
 */
void updateEregister(
    unsigned int stat, unsigned int icode, unsigned int ifun,
    unsigned int valC, unsigned int valA, unsigned int valB,
    unsigned int dstE, unsigned int dstM, unsigned int srcA, unsigned int srcB
) {
    E.stat = stat;
    E.icode = icode;
    E.ifun = ifun;
    E.valC = valC;
    E.valA = valA;
    E.valB = valB;
    E.dstE = dstE;
    E.dstM = dstM;
    E.srcA = srcA;
    E.srcB = srcB;
}

/**
 * Execute the execute stage.
 * @param FORW A pointer to the forwarding struct `forwarding.h`
 */
void executeStage(forwardType * FORW) {
    cnd = false;
    int valE = 0;
    int dstE = E.dstE;

    if (E.stat == S_INS || E.stat == S_ADR || E.stat == S_HLT) {
        canUpdateCC = false;
    }
    if (E.stat != S_INS) {
        valE = (*funcptr[E.icode])();
    }
    if (E.icode == I_RRMOVL && !cnd) {
        dstE = RNONE;
    }

    FORW->e_dstE = dstE;
    FORW->e_valE = valE;
    FORW->E_dstM = E.dstM;
    FORW->e_Cnd = cnd;
    FORW->E_icode = E.icode;

    if (bubble_M(FORW)) {
        clearMregister();
        if (canUpdateCC) {
            clearCC();
        }
    } else if (!stall_M()) {
        updateMregister(E.stat, E.icode, cnd, valE, E.valA, dstE, E.dstM);
    }
}
