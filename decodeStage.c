#include "decodeStage.h"
#include "tools.h"

static dregister D;

dregister getDregister() {
    return D;
}

void clearDregister() {
    clearBuffer((char *) &D, sizeof(D));
}

