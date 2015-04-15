#ifndef DECODESTAGE_H
#define DECODESTAGE_H
typedef struct 
{
    unsigned int stat;
    unsigned int icode;
    unsigned int ifun;
    unsigned int rA;
    unsigned int rB;
    unsigned int valC;
    unsigned int valP;
} dregister;

dregister getDregister();
void clearDregister();
void updateDregister(
    unsigned int stat, unsigned int icode, unsigned int ifun,
    unsigned int rA, unsigned int rB, unsigned int valC, unsigned int valP
);
unsigned int get_src_A();
unsigned int get_src_B();
unsigned int get_dst_E();
unsigned int get_dst_M();
unsigned int sel_fwd_A(unsigned int srcA, forwardType * FORW);
unsigned int sel_fwd_B(unsigned int srcB, forwardType * FORW);
bool stall_E();
bool bubble_E(unsigned int d_srcA, unsigned int d_srcB, forwardType * FORW);
void decodeStage(forwardType * FORW);
#endif
