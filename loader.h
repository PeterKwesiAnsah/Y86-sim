bool validFileName(char *file_name);
void discardRest(char *line, FILE * p_file);
bool hasAddress(char ** record);
bool hasData(char ** record);
unsigned int extractAddress(char ** record);
size_t extractData(char ** record, unsigned char * data_record);
bool checkRecord(char ** record);
bool load(int argv, char * args[]);
