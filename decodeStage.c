#include <stdio.h>
#include "decodeStage.h"
#include "registers.h"
#include "instructions.h"
#include "tools.h"
#include "executeStage.h"
#include "writebackStage.h"
#include "memoryStage.h"
#include "forwarding.h"

static dregister D;

/**
 * [getDregister description]
 * @return [description]
 */
dregister getDregister() {
    return D;
}

/**
 * [clearDregister description]
 */
void clearDregister() {
    clearBuffer((char *) &D, sizeof(D));
}

/**
 * [updateDregister description]
 */
void updateDregister(
    unsigned int stat, unsigned int icode, unsigned int ifun,
    unsigned int rA, unsigned int rB, unsigned int valC, unsigned int valP
) {
    D.stat = stat;
    D.icode = icode;
    D.ifun = ifun;
    D.rA = rA;
    D.rB = rB;
    D.valC = valC;
    D.valP = valP;
}

/**
 * [getSrcA description]
 * @return [description]
 */
unsigned int getSrcA() {
    if (
        D.icode == I_RRMOVL || D.icode == I_RMMOVL ||
        D.icode == I_OPL || D.icode == I_PUSHL
    ) {
        return D.rA;
    } else if (D.icode == I_POPL || D.icode == I_RET) {
        return ESP;
    } else {
        return RNONE;
    }
}

/**
 * [getSrcB description]
 * @return [description]
 */
unsigned int getSrcB() {
    if (D.icode == I_OPL || D.icode == I_RMMOVL || D.icode == I_MRMOVL) {
        return D.rB;
    } else if (
        D.icode == I_PUSHL || D.icode == I_POPL ||
        D.icode == I_CALL || D.icode == I_RET
    ) {
        return ESP;
    } else {
        return RNONE;
    }
}

/**
 * [getDstE description]
 * @return [description]
 */
unsigned int getDstE() {
    if (D.icode == I_RRMOVL || D.icode == I_IRMOVL || D.icode == I_OPL) {
        return D.rB;
    } else if (
        D.icode == I_PUSHL || D.icode == I_POPL ||
        D.icode == I_CALL || D.icode == I_RET
    ) {
        return ESP;
    } else {
        return RNONE;
    }
}

/**
 * [getDstM description]
 * @return [description]
 */
unsigned int getDstM() {
    if (D.icode == I_MRMOVL || D.icode == I_POPL) {
        return D.rA;
    } else {
        return RNONE;
    }
}

/**
 * [selFwdA description]
 * @param  srcA [description]
 * @param  FORW [description]
 * @return      [description]
 */
unsigned int selFwdA(unsigned int srcA, forwardType * FORW) {
    if (D.icode == I_CALL || D.icode == I_JXX) {
        return D.valP;
    } else if (srcA == RNONE) {
        return 0;
    } else if (srcA == FORW->e_dstE) {
        return FORW->e_valE;
    } else if (srcA == FORW->M_dstM) {
        return FORW->m_valM;
    } else if (srcA == FORW->M_dstE) {
        return FORW->M_valE;
    } else if (srcA == FORW->W_dstM) {
        return FORW->W_valM;
    } else if (srcA == FORW->W_dstE) {
        return FORW->W_valE;
    } else {
        return getRegister(srcA);
    }
}

/**
 * [fwdB description]
 * @param  srcB [description]
 * @param  FORW [description]
 * @return      [description]
 */
unsigned int fwdB(unsigned int srcB, forwardType * FORW) {
    if (srcB == RNONE) {
        return 0;
    } else if (srcB == FORW->e_dstE) {
        return FORW->e_valE;
    } else if (srcB == FORW->M_dstM) {
        return FORW->m_valM;
    } else if (srcB == FORW->M_dstE) {
        return FORW->M_valE;
    } else if (srcB == FORW->W_dstM) {
        return FORW->W_valM;
    } else if (srcB == FORW->W_dstE) {
        return FORW->W_valE;
    } else {
        return getRegister(srcB);
    }
}

/**
 * [bubbleE description]
 * @param  d_srcA [description]
 * @param  d_srcB [description]
 * @param  FORW   [description]
 * @return        [description]
 */
bool bubbleE(unsigned int d_srcA, unsigned int d_srcB, forwardType * FORW) {
    return (
        (FORW->E_icode == I_JXX && !(FORW->e_Cnd)) ||
        (
            (FORW->E_icode == I_MRMOVL || FORW->E_icode == I_POPL) &&
            (FORW->E_dstM == d_srcA || FORW->E_dstM == d_srcB)
        )
    );
}

/**
 * [stallE description]
 * @return [description]
 */
bool stallE() {
    return false;
}

/**
 * [decodeStage description]
 * @param FORW [description]
 */
void decodeStage(forwardType * FORW) {

    unsigned int dstE = getDstE();
    unsigned int dstM = getDstM();
    unsigned int srcA = getSrcA();
    unsigned int srcB = getSrcB();

    FORW->d_srcA = srcA;
    FORW->d_srcB = srcB;
    FORW->D_icode = D.icode;

    unsigned int valA = selFwdA(srcA, FORW);
    unsigned int valB = fwdB(srcB, FORW);

    if (bubbleE(srcA, srcB, FORW)) {
        clearEregister();
    } else if (!stallE()) {
        updateEregister(
            D.stat, D.icode, D.ifun, D.valC, valA, valB, dstE, dstM, srcA, srcB
        );
    }

    // printf(
    //     "DECODE\t\t<stat=0x%.2x, icode=0x%.2x, ifun=0x%.2x, rA=0x%.2x, rB=0x%.2x, valC=0x%.2x, valP=0x%.2x>\n",
    //     D.stat, D.icode, D.ifun, D.rA, D.rB, D.valC, D.valP
    // );
}
