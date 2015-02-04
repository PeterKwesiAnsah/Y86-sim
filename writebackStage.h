#ifndef WRITEBACKSTAGE_H
#define WRITEBACKSTAGE_H
typedef struct 
{
    unsigned int predPC;
} wregister;

//prototypes for functions called from files other than fetchStage
wregister getWregister();
void clearWregister();
#endif
