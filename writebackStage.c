#include "writebackStage.h"
#include "tools.h"

static wregister W;

wregister getWregister() {
    return W;
}

void clearWregister() {
    clearBuffer((char *) &W, sizeof(W));
}

