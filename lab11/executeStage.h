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
int p__dump();
int p__none();
int p__irmovl();
int p__rrmovl();
int p__rmmovl();
int p__mrmovl();
int p__jxx();
int p__call();
int p__ret();
int p__push();
int p__pop();
int p__opl();
void updateCC(int val);
void setCnd(unsigned int code);
void updateEregister(
    unsigned int stat, unsigned int icode, unsigned int ifun,
    unsigned int valC, unsigned int valA, unsigned int valB,
    unsigned int dstE, unsigned int dstM, unsigned int srcA, unsigned int srcB
);
void executeStage(forwardType * FORW);
#endif
