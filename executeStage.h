#ifndef EXECUTESTAGE_H
#define EXECUTESTAGE_H
typedef struct 
{
    unsigned int stat;
    unsigned int icode;
    unsigned int ifun;
    unsigned int valC;
    unsigned int valA;
    unsigned int valB;
    unsigned int dstE;
    unsigned int dstM;
    unsigned int srcA;
    unsigned int srcB;
} eregister;

void initExecFuncs();
eregister getEregister();
void clearEregister();
bool stall_M();
bool bubble_M(forwardType * FORW);
int perform_dump();
int perform_none();
int perform_irmovl();
int perform_rrmovl();
int perform_rmmovl();
int perform_mrmovl();
int perform_jxx();
int perform_call();
int perform_ret();
int perform_push();
int perform_pop();
int perform_opl();
void updateCC(int val);
void setCnd(unsigned int code);
void updateEregister(
    unsigned int stat, unsigned int icode, unsigned int ifun,
    unsigned int valC, unsigned int valA, unsigned int valB,
    unsigned int dstE, unsigned int dstM, unsigned int srcA, unsigned int srcB
);
void executeStage(forwardType * FORW);
#endif
