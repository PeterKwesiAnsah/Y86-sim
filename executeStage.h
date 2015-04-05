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

//prototypes for functions called from files other than fetchStage
unsigned int perform_dump();
unsigned int perform_none();
void initExecFuncs();
eregister getEregister();
void clearEregister();
void updateCC(int val);
unsigned int perform_irmovl();
unsigned int perform_rrmovl();
unsigned int perform_rmmovl();
unsigned int perform_mrmovl();
unsigned int perform_jxx();
unsigned int perform_call();
unsigned int perform_ret();
unsigned int perform_push();
unsigned int perform_pop();
void setCnd(unsigned int code);
unsigned int perform_opl();
void updateEregister(
    unsigned int stat, unsigned int icode, unsigned int ifun,
    unsigned int valC, unsigned int valA, unsigned int valB,
    unsigned int dstE, unsigned int dstM, unsigned int srcA, unsigned int srcB
);
void executeStage();
#endif
