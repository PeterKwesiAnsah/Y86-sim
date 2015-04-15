#include <stdio.h>
#include <stdbool.h>

#include "forwarding.h"
#include "instructions.h"
#include "registers.h"
#include "memory.h"
#include "tools.h"

#include "fetchStage.h"
#include "decodeStage.h"
#include "memoryStage.h"
#include "writebackStage.h"


static fregister F;


/**
 * Get the state of the fetch stage struct.
 * @return The state of the fetch stage struct
 */
fregister getFregister() {
    return F;
}

/**
 * Reset the fetch stage struct to a default state.
 */
void clearFregister() {
    F.predPC = 0;
}

/**
 * Select the next program counter address index.
 * @param  predPC The current PC (used if no jumps or returns from forwarding)
 * @param  FORW   A pointer to the forwarding struct `forwarding.h`
 * @return        The PC to be used with respect to forwarding
 */
unsigned int sel_pc(unsigned int predPC, forwardType * FORW) {
    if (FORW->M_icode == I_JXX && !FORW->M_Cnd) {
        return FORW->M_valA;
    } else if (FORW->W_icode == I_RET) {
        return FORW->W_valM;
    } else {
        return predPC;
    }
}

/**
 * Determines if a given icode requires a register id.
 * @param  icode Instruction code in question
 * @return       True if instruction requires register id, false otherwise
 */
bool req_regid(unsigned int icode) {
    return (
        (icode == I_CMOVXX) || (icode == I_OPL) || (icode == I_PUSHL) ||
        (icode == I_POPL) || (icode == I_IRMOVL) || (icode == I_RMMOVL) ||
        (icode == I_MRMOVL)
    );
}

/**
 * Determines if a given icode requires a immediate value (C).
 * @param  icode Instruction code in question
 * @return       True if instruction requires immediate value, false otherwise
 */
bool req_valc(unsigned int icode) {
    return (
        (icode == I_JXX) || (icode == I_CALL) || (icode == I_DUMP) ||
        (icode == I_IRMOVL) || (icode == I_RMMOVL) || (icode == I_MRMOVL)
    );
}

/**
 * Determines if the fetch stage should be stalled.
 * @param  FORW A pointer to the forwarding struct `forwarding.h`
 * @return      True if the fetch stage should be stalled, false otherwise
 */
bool stall_F(forwardType * FORW) {
    return ((
        (FORW->E_icode == I_MRMOVL || FORW->E_icode == I_POPL) &&
        (FORW->E_dstM == FORW->d_srcA || FORW->E_dstM == FORW->d_srcB)
    ) || (
        FORW->D_icode == I_RET || FORW->E_icode == I_RET ||
        FORW->M_icode == I_RET
    ));
}

/**
 * Determines if the fetch stage should be bubbled.
 * @return False
 */
bool bubble_F() {
    return false;
}

/**
 * Determines if the decode stage should be stalled.
 * @param  FORW A pointer to the forwarding struct `forwarding.h`
 * @return      True if the decode stage should be stalled, false otherwise
 */
bool stall_D(forwardType * FORW) {
    return (
        (FORW->E_icode == I_MRMOVL || FORW->E_icode == I_POPL) &&
        (FORW->E_dstM == FORW->d_srcA || FORW->E_dstM == FORW->d_srcB)
    );
}


/**
 * Determines if the decode stage should be bubbled.
 * @param  FORW A pointer to the forwarding stuct `forwarding.h`
 * @return      True if the decode stage should be bubbled, false otherwise
 */
bool bubble_D(forwardType * FORW) {
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
 * Execute the fetch stage.
 * @param FORW A pointer to the forwarding struct `forwarding.h`
 */
void fetchStage(forwardType * FORW) {
    unsigned int f_pc = sel_pc(F.predPC, FORW);
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

    // unsigned int ins_regid[8] = {
    //     I_IRMOVL, I_OPL, I_PUSHL, I_POPL,
    //     I_IRMOVL, I_RMMOVL, I_MRMOVL, I_CMOVXX
    // };
    // bool req_regid = valinarr(icode, ins_regid, 8);

    // unsigned int ins_valc[6] = {
    //     I_IRMOVL, I_RMMOVL, I_MRMOVL, I_JXX, I_CALL, I_DUMP
    // };
    // bool req_valc = valinarr(icode, ins_valc, 6);


    if (req_regid(icode)) {
        unsigned char regs = getByte(f_pc, &memError);
        rA = ((0xf0 & regs) >> 4);
        rB = (0x0f & regs);
        f_pc++;
    }

    if (req_valc(icode)) {
        unsigned char valc_bytes[4];
        unsigned int i;
        for (i = 0; i < 4; i++, f_pc++) {
            valc_bytes[i] = getByte(f_pc, &memError);
        }
        valC = join_bytes(valc_bytes);
    }

    unsigned int valP = f_pc;

    if (bubble_F()) {
        clearFregister();
    } else if (!stall_F(FORW)) {
       F.predPC = (icode == I_JXX || icode == I_CALL) ? valC : valP;
    }

    if (bubble_D(FORW)) {
        clearDregister();
    } else if (!stall_D(FORW)) {
        updateDregister(stat, icode, ifun, rA, rB, valC, valP);
    }

}
