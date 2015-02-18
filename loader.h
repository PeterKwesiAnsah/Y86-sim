#include <stdbool.h>

bool validFileName(char *file_name);
void discardRest(char *line, FILE * p_file);
bool hasAddress(char ** record);
bool hasData(char ** record);
unsigned int extractAddress(char ** record);
unsigned int extractData(char ** record);
bool checkRecord(char ** record);
bool load(char *file_name);
