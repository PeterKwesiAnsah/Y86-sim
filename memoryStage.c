#include "memoryStage.h"
#include "tools.h"

static mregister M;

mregister getMregister() {
    return M;
}

void clearMregister() {
    clearBuffer((char *) &M, sizeof(M));
}

