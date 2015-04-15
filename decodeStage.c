#include <stdio.h>
#include <stdbool.h>

#include "forwarding.h"
#include "instructions.h"
#include "registers.h"
#include "tools.h"

#include "decodeStage.h"
#include "executeStage.h"
#include "memoryStage.h"
#include "writebackStage.h"


static dregister D;


/**
 * Get the state of the decode stage struct.
 * @return The state of the decode stage struct
 */
dregister getDregister() {
    return D;
}

/**
 * Reset the decode stage struct to a default state.
 */
void clearDregister() {
    D.stat = S_AOK;
    D.icode = I_NOP;
    D.ifun = 0;
    D.rA = RNONE;
    D.rB = RNONE;
    D.valC = 0;
    D.valP = 0;
}

/**
 * Update the decode state with the given parameters.
 * @param stat  Status code
 * @param icode Instruction code
 * @param ifun  Instruction function
 * @param rA    Register A
 * @param rB    Register B
 * @param valC  Value C
 * @param valP  Value P
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
 * Get the required value of source A.
 * @return Value to be used for source A
 */
unsigned int get_src_A() {
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
 * Get the required value of soruce B.
 * @return Value to be used for source B
 */
unsigned int get_src_B() {
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
 * Get the required value for destination E.
 * @return Value to be used for destination E
 */
unsigned int get_dst_E() {
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
 * Get the required value for destination M.
 * @return Value to be used for destination M
 */
unsigned int get_dst_M() {
    if (D.icode == I_MRMOVL || D.icode == I_POPL) {
        return D.rA;
    } else {
        return RNONE;
    }
}

/**
 * Select whether to forward a value to source A from the pipe.
 * @param  srcA Current value of soruce A
 * @param  FORW A pointer to the forwarding struct `forwarding.h`
 * @return      Value to be used for source A
 */
unsigned int sel_fwd_A(unsigned int srcA, forwardType * FORW) {
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
 * Select whether to forward a value to source B from the pipe.
 * @param  srcB Current value of source B
 * @param  FORW A pointer to the forwarding struct `forwardning.h`
 * @return      Value to be used for source B
 */
unsigned int sel_fwd_B(unsigned int srcB, forwardType * FORW) {
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
 * Determines if the execute stage should be stalled.
 * @return False
 */
bool stall_E() {
    return false;
}

/**
 * Determines if the execute stage should be bubbled.
 * @param  d_srcA Decodes current value of source A
 * @param  d_srcB Decodes current value of source B
 * @param  FORW   A pointer to the forwarding struct `forwarding.h`
 * @return        True if the execute stage should be bubbled, otherwise false
 */
bool bubble_E(unsigned int d_srcA, unsigned int d_srcB, forwardType * FORW) {
    return (
        (FORW->E_icode == I_JXX && !(FORW->e_Cnd)) ||
        (
            (FORW->E_icode == I_MRMOVL || FORW->E_icode == I_POPL) &&
            (FORW->E_dstM == d_srcA || FORW->E_dstM == d_srcB)
        )
    );
}

/**
 * Execute the decode stage.
 * @param FORW A pointer to the forwarding struct `forwarding.h`
 */
void decodeStage(forwardType * FORW) {

    unsigned int dstE = get_dst_E();
    unsigned int dstM = get_dst_M();
    unsigned int srcA = get_src_A();
    unsigned int srcB = get_src_B();

    FORW->d_srcA = srcA;
    FORW->d_srcB = srcB;
    FORW->D_icode = D.icode;

    unsigned int valA = sel_fwd_A(srcA, FORW);
    unsigned int valB = sel_fwd_B(srcB, FORW);

    if (bubble_E(srcA, srcB, FORW)) {
        clearEregister();
    } else if (!stall_E()) {
        updateEregister(
            D.stat, D.icode, D.ifun, D.valC, valA, valB, dstE, dstM, srcA, srcB
        );
    }
}
