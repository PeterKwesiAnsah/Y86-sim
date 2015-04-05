#include <stdbool.h>
#include <stdlib.h>

unsigned int bit_mask(int low, int high);
unsigned char * get_bytes(unsigned int source, unsigned char *bytes);
unsigned int join_bytes(unsigned char *bytes);
unsigned int getBits(int low, int high, unsigned int source);
unsigned int setBits(int low, int high, unsigned int source);
unsigned int clearBits(int low, int high, unsigned int source);
unsigned int assignOneBit(int bitNumber, int bitValue, unsigned int source);
unsigned char getByteNumber(int byteNo, unsigned int source);
unsigned int putByteNumber(int byteNo, unsigned char byteValue, 
                           unsigned int source);
unsigned int buildWord(unsigned char byte0, unsigned char byte1, 
                       unsigned char byte2, unsigned char byte3);
bool isNegative(unsigned int source);
void expandBits(unsigned int source, char bits[36]);
void clearBuffer(char * buff, int size);
int lsr(int x, int n);
unsigned char * revchararr(unsigned char * arr, unsigned short int size);
void remchars(char *str, char c);
bool valinarr(int val, int * arr, int size);
void chopStart(char *str, size_t n);
bool isxstr(char *str);
char *strdup(const char *str);
size_t splitString(char *line, char ***sve_record, char *split);
int checkPipe(char * line);