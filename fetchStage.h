#ifndef FETCHSTAGE_H
#define FETCHSTAGE_H
typedef struct 
{
    unsigned int predPC;
} fregister;

//prototypes for functions called from files other than fetchStage
fregister getFregister();
// bool req_regid(unsigned int icode);
// unsigned int req_valc(unsigned int icode);
// unsigned int selectPC(unsigned int predPC, forwardType * FORW);
void clearFregister();
void fetchStage();
#endif
