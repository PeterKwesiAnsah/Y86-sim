#ifndef MEMORYSTAGE_H
#define MEMORYSTAGE_H
typedef struct 
{
    unsigned int stat;
    unsigned int icode;
    unsigned int Cnd;
    unsigned int valE;
    unsigned int valA;
    unsigned int dstE;
    unsigned int dstM; 
} mregister;

mregister getMregister();
void clearMregister();
void updateMregister(
    unsigned int stat, unsigned int icode, unsigned int Cnd, unsigned int valE,
    unsigned int valA, unsigned int dstE, unsigned int dstM
);
bool stall_W(forwardType * FORW);
void mem_ctrl();
unsigned int mem_addr();
void memoryStage(forwardType * FORW);
#endif
