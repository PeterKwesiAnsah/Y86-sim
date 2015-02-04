#include "executeStage.h"
#include "tools.h"

static eregister E;

eregister getEregister() {
    return E;
}

void clearEregister() {
    clearBuffer((char *) &E, sizeof(E));
}

