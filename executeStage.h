#ifndef EXECUTESTAGE_H
#define EXECUTESTAGE_H
typedef struct 
{
    unsigned int predPC;
} eregister;

//prototypes for functions called from files other than fetchStage
eregister getEregister();
void clearEregister();
#endif
