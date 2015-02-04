#include "fetchStage.h"
#include "tools.h"

//F register holds the input for the fetch stage. 
//It is only accessible from this file. (static)
static fregister F;

// Function: getFregister
// Description: Returns a copy of the F register
// Params: none
// Returns: fregister
// Modifies: none
fregister getFregister()
{
    return F;
}

// Function: clearFregister
// Description: Returns a copy of the F register
// Params: none
// Returns: none
// Modifies: F 
void clearFregister()
{
    clearBuffer((char *) &F, sizeof(F));
}
