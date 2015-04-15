#ifndef FETCHSTAGE_H
#define FETCHSTAGE_H
typedef struct 
{
    unsigned int predPC;
} fregister;

fregister getFregister();
void clearFregister();
unsigned int sel_pc(unsigned int predPC, forwardType * FORW);
bool req_regid(unsigned int icode);
bool req_valc(unsigned int icode);
bool stall_F(forwardType * FORW);
bool bubble_F();
bool stall_D(forwardType * FORW);
bool bubble_D(forwardType * FORW);
void fetchStage(forwardType * FORW);
#endif
