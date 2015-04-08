#include <stdio.h>
#include "executeStage.h"
#include "memoryStage.h"
#include "forwarding.h"
#include "registers.h"
#include "instructions.h"
#include "tools.h"

static int (*funcptr[16])();
static eregister E;
bool cnd;
static bool canUpdateCC;

int perform_dump() {return E.valC;}
int perform_none() {return 0;}

/**
 * [initExecFuncs description]
 */
void initExecFuncs() {
    funcptr[I_HALT] = &perform_none;
    funcptr[I_NOP] = &perform_none;
    funcptr[I_RRMOVL] = &perform_rrmovl;
    funcptr[I_RMMOVL] = &perform_rmmovl;
    funcptr[I_IRMOVL] = &perform_irmovl;
    funcptr[I_MRMOVL] = &perform_mrmovl;
    funcptr[I_OPL] = &perform_opl;
    funcptr[I_JXX] = &perform_jxx;
    funcptr[I_CMOVXX] = &perform_rrmovl;
    funcptr[I_CALL] = &perform_call;
    funcptr[I_RET] = &perform_ret;
    funcptr[I_PUSHL] = &perform_push;
    funcptr[I_POPL] = &perform_pop;
    funcptr[I_DUMP] = &perform_dump;
}

/**
 * [getEregister description]
 * @return [description]
 */
eregister getEregister() {
    return E;
}

/**
 * [clearEregister description]
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
 * [bubbleM description]
 * @param  FORW [description]
 * @return      [description]
 */
bool bubbleM(forwardType * FORW) {
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
 * [stallM description]
 * @return [description]
 */
bool stallM() {
    return false;
}

/**
 * [perform_irmovl description]
 * @return [description]
 */
int perform_irmovl() {
    return E.valC;
}

/**
 * [perform_rrmovl description]
 * @return [description]
 */
int perform_rrmovl() {
    setCnd(E.ifun);
    return E.valA;
}

/**
 * [perform_rmmovl description]
 * @return [description]
 */
int perform_rmmovl() {
    return E.valC + E.valB;
}

/**
 * [perform_mrmovl description]
 * @return [description]
 */
int perform_mrmovl() {
    return E.valC + E.valB;
}

/**
 * [perform_jxx description]
 * @return [description]
 */
int perform_jxx() {
    setCnd(E.ifun);
    return E.valA;
}

/**
 * [perform_call description]
 * @return [description]
 */
int perform_call() {
    return E.valB - 4;
}

/**
 * [perform_ret description]
 * @return [description]
 */
int perform_ret() {
    return E.valB + 4;
}

/**
 * [perform_push description]
 * @return [description]
 */
int perform_push() {
    cnd = 0;
    return -4 + E.valB;
}

/**
 * [perform_pop description]
 * @return [description]
 */
int perform_pop() {
    cnd = 0;
    return 4 + E.valB;
}

/**
 * [perform_opl description]
 * @return [description]
 */
int perform_opl() {
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
 * [updateCC description]
 * @param val [description]
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
 * [setCnd description]
 * @param code [description]
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
 * [updateEregister description]
 * @param stat  [description]
 * @param icode [description]
 * @param ifun  [description]
 * @param valC  [description]
 * @param valA  [description]
 * @param valB  [description]
 * @param dstE  [description]
 * @param dstM  [description]
 * @param srcA  [description]
 * @param srcB  [description]
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
 * [executeStage description]
 * @param FORW [description]
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

    if (bubbleM(FORW)) {
        clearMregister();
        if (canUpdateCC) {
            clearCC();
        }
    } else if (!stallM()) {
        updateMregister(E.stat, E.icode, cnd, valE, E.valA, dstE, E.dstM);
    }

    // printf(
    //     "EXECUTE\t\t<stat=0x%.2x, icode=0x%.2x, ifun=0x%.2x, valC=0x%.2x, valA=0x%.2x, valB=0x%.2x, dstE=0x%.2x, dstM=0x%.2x, srcA=0x%.2x, srcB=0x%.2x>\n",
    //     E.stat, E.icode, E.ifun, E.valC, E.valA, E.valB, E.dstE, E.dstM, E.srcA, E.srcB
    // );
}