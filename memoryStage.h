#ifndef MEMORYSTAGE_H
#define MEMORYSTAGE_H
typedef struct 
{
    unsigned int predPC;
} mregister;

//prototypes for functions called from files other than fetchStage
mregister getMregister();
void clearMregister();
#endif
