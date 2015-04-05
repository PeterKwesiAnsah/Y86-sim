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

//prototypes for functions called from files other than fetchStage
dregister getDregister();
void clearDregister();
void updateDregister(
    unsigned int stat, unsigned int icode, unsigned int ifun,
    unsigned int rA, unsigned int rB, unsigned int valC, unsigned int valP
);
unsigned int getSrcA();
unsigned int getSrcB();
unsigned int getDstE();
unsigned int getDstM();
#endif
