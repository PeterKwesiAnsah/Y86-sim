#include <stdio.h>
#include "fetchStage.h"
#include "instructions.h"
#include "registers.h"
#include "memory.h"
#include "tools.h"
#include "forwarding.h"
#include "decodeStage.h"
#include "memoryStage.h"
#include "writebackStage.h"

static fregister F;

/**
 * [getFregister description]
 * @return [description]
 */
fregister getFregister() {
    return F;
}

/**
 * [clearFregister description]
 */
void clearFregister() {
    F.predPC = 0;
}

/**
 * [selectPC description]
 * @param  predPC [description]
 * @param  FORW   [description]
 * @return        [description]
 */
unsigned int selectPC(unsigned int predPC, forwardType * FORW) {
    if (FORW->M_icode == I_JXX && !FORW->M_Cnd) {
        return FORW->M_valA;
    }
    if (FORW->W_icode == I_RET) {
        return FORW->W_valM;
    }
    return predPC;
}

/**
 * [req_regid description]
 * @param  icode [description]
 * @return       [description]
 */
bool req_regid(unsigned int icode) {
    return (
        (icode == I_CMOVXX) || (icode == I_OPL) || (icode == I_PUSHL) ||
        (icode == I_POPL) || (icode == I_IRMOVL) || (icode == I_RMMOVL) ||
        (icode == I_MRMOVL)
    );
}

/**
 * [req_valc description]
 * @param  icode [description]
 * @return       [description]
 */
unsigned int req_valc(unsigned int icode) {
    if (
        (icode == I_JXX) || (icode == I_CALL) || (icode == I_DUMP)
    ) {
        return 1;
    }
    if (
        (icode == I_IRMOVL) || (icode == I_RMMOVL) || (icode == I_MRMOVL)
    ) {
        return 2;
    }
    return 0;
}

/**
 * [bubbleF description]
 * @return [description]
 */
bool bubbleF() {
    return false;
}

/**
 * [stallF description]
 * @param  FORW [description]
 * @return      [description]
 */
bool stallF(forwardType * FORW) {
    return ((
        (FORW->E_icode == I_MRMOVL || FORW->E_icode == I_POPL) &&
        (FORW->E_dstM == FORW->d_srcA || FORW->E_dstM == FORW->d_srcB)
    ) || (
        FORW->D_icode == I_RET || FORW->E_icode == I_RET ||
        FORW->M_icode == I_RET
    ));
}

/**
 * [bubbleD description]
 * @param  FORW [description]
 * @return      [description]
 */
bool bubbleD(forwardType * FORW) {
    return (
        (FORW->E_icode == I_JXX && !FORW->e_Cnd) ||
        (
            !(
                (FORW->E_icode == I_MRMOVL || FORW->E_icode == I_POPL) &&
                (FORW->E_dstM == FORW->d_srcA || FORW->E_dstM == FORW->d_srcB)
            ) && (
                FORW->D_icode == I_RET || FORW->E_icode == I_RET ||
                FORW->M_icode == I_RET
            )
        )
    );
}

/**
 * [stallD description]
 * @param  FORW [description]
 * @return      [description]
 */
bool stallD(forwardType * FORW) {
    return (
        (FORW->E_icode == I_MRMOVL || FORW->E_icode == I_POPL) &&
        (FORW->E_dstM == FORW->d_srcA || FORW->E_dstM == FORW->d_srcB)
    );
}


/**
 * [fetchStage description]
 */
void fetchStage(forwardType * FORW) {
    unsigned int f_pc = selectPC(F.predPC, FORW);
    unsigned int rA = RNONE, rB = RNONE;
    unsigned int valC = 0;
    bool memError = false;

    unsigned char icode = getByte(f_pc, &memError);
    f_pc++;

    icode = (memError) ? I_NOP : icode;
    unsigned int ifun = (memError) ? 0x0 : (0x0f & icode);
    icode = lsr((icode & 0xf0), 4);

    unsigned int ins[14] = {
        I_NOP, I_HALT, I_RRMOVL, I_IRMOVL, I_RMMOVL, I_MRMOVL, I_OPL, I_JXX,
        I_CALL, I_CMOVXX, I_RET, I_PUSHL, I_POPL, I_DUMP
    };
    bool ins_valid = valinarr(icode, ins, 14);

    unsigned char stat = S_AOK;
    if (memError) {
        stat = S_ADR;
    } else if (icode == I_HALT) {
        stat = S_HLT;
    } else if (!ins_valid) {
        stat = S_INS;
    }

    unsigned int ins_regid[8] = {
        I_IRMOVL, I_OPL, I_PUSHL, I_POPL,
        I_IRMOVL, I_RMMOVL, I_MRMOVL, I_CMOVXX
    };
    bool req_regid = valinarr(icode, ins_regid, 8);

    unsigned int ins_valc[6] = {
        I_IRMOVL, I_RMMOVL, I_MRMOVL, I_JXX, I_CALL, I_DUMP
    };
    bool req_valc = valinarr(icode, ins_valc, 6);


    if (req_regid) {
        unsigned char regs = getByte(f_pc, &memError);
        rA = ((0xf0 & regs) >> 4);
        rB = (0x0f & regs);
        f_pc++;
    }

    if (req_valc) {
        unsigned char valc_bytes[4];
        unsigned int i;
        for (i = 0; i < 4; i++, f_pc++) {
            valc_bytes[i] = getByte(f_pc, &memError);
        }
        valC = join_bytes(valc_bytes);
    }

    unsigned int valP = f_pc;

    if (bubbleF()) {
        clearFregister();
    } else if (!stallF(FORW)) {
       F.predPC = (icode == I_JXX || icode == I_CALL) ? valC : valP;
    }

    if (bubbleD(FORW)) {
        clearDregister();
    } else if (!stallD(FORW)) {
        updateDregister(stat, icode, ifun, rA, rB, valC, valP);
    }

    // printf(
//         "FETCH\t\t<pos=0x%.8x, stat=0x%.2x, icode=0x%.2x, ifun=0x%.2x, rA=0x%.2x, rB=0x%.2x, valC=0x%.2x, valP=0x%.2x>\n",
//         f_pc, stat, icode, ifun, rA, rB, valC, valP
//     );

}
