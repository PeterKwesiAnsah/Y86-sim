#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <memory.h>
#include "tools.h"

/**
 * Generates a set bitmask in range 0-31 passed by (low, high,) respectively.
 * @param  low  lowest range bit
 * @param  high highest range bit
 * @return      set bitmask in range 0-31
 */
unsigned int bit_mask(int low, int high) {
  if ((low < 0 || low > 31) || (high < 0 || high > 31) || (low > high)) {
    return 0;
  } else {
    return (((unsigned) -1 >> (31 - (high))) & ~((1U << (low)) - 1));
  }
}

/**
 * Extract bytes from some passed `source` into an array of length 4 `bytes`
 * @param  source source integer to extract bytes from
 * @param  bytes  pointer to an array of at least length 4
 * @return        pointer to array of extracted bytes
 */
unsigned char * get_bytes(unsigned int source, unsigned char *bytes) {
  bytes[0] = (source >> 0) & 0xff;
  bytes[1] = (source >> 8) & 0xff;
  bytes[2] = (source >> 16) & 0xff;
  bytes[3] = (source >> 24) & 0xff;
  return bytes;
}

/**
 * Join a array of bytes extracted by ``get_bytes`` together into an integer.
 * @param  bytes pointer to an array of at least length 4
 * @return       newly formed integer from `bytes` 
 */
unsigned int join_bytes(unsigned char *bytes) {
  return (bytes[3] << 24) | (bytes[2] << 16) | (bytes[1] << 8) | (bytes[0] << 0);
}

/**
 * Get the bits in range 0-31 from `source` passed by (low, high,) respectively.
 * @param  low    lowest range bit
 * @param  high   highest range bit
 * @param  source passed integer to retrieve bits from
 * @return        range of bits as unsigned int
 */
unsigned int getBits(int low, int high, unsigned int source) {
  if ((low < 0 || low > 31) || (high < 0 || high > 31) || (low > high)) {
    return 0;
  } else {
    return ((source & bit_mask(low, high)) >> low);
  }
} 

/**
 * Set the bits in range 0-31 in `source` passed by (low, high,) respectively.
 * @param  low    lowest range bit
 * @param  high   highest range bit
 * @param  source passed integer to set bits in
 * @return        passed integer with newley set bits
 */
unsigned int setBits(int low, int high, unsigned int source) {
  if ((low < 0 || low > 31) || (high < 0 || high > 31) || (low > high)) {
    return source;
  } else {
    return (source |= bit_mask(low, high));
  }
}

/**
 * Clear the bits in range 0-31 in `source` passed by (low, high,) respectively.
 * @param  low    lowest range bit`
 * @param  high   highest range bit
 * @param  source passed integer to clear bits from
 * @return        passed integer with newly cleared bits
 */
unsigned int clearBits(int low, int high, unsigned int source) {
  if ((low < 0 || low > 31) || (high < 0 || high > 31) || (low > high)) {
    return source;
  } else {
    return (source & ~(bit_mask(low, high)));
  }
}

/**
 * Assign the passed `bitNumber` in source (range 0-31) to the passed `bitValue`.
 * @param  bitNumber bit to be assigned
 * @param  bitValue  value to assign bit at bit `bitNumber`
 * @param  source    passed integer to modify bit at `bitNumber`
 * @return           newly modified `source`
 */
unsigned int assignOneBit(int bitNumber, int bitValue, unsigned int source) {
  if ((bitNumber < 0 || bitNumber > 31) || (bitValue > 1 || bitValue < 0)) {
    return source;
  } else {
    return (bitValue < 1) ?
      clearBits(bitNumber, bitNumber, source) :
      setBits(bitNumber, bitNumber, source);
  }
}

/**
 * Get the byte in an unsigned integer (range 0-3).
 * @param  byteNo byte number to retrieve
 * @param  source passed integer to retireve byte from
 * @return        retrieved byte `byteNo` from `source`
 */
unsigned char getByteNumber(int byteNo, unsigned int source) {
  if (byteNo > 3 || byteNo < 0) {
    return 0;
  } else {
    unsigned char bytes[4];
    return get_bytes(source, bytes)[byteNo];
  }
}

/**
 * Place a byte (`byteValue`) into the specified `byteNo` of int `source`.
 * @param  byteNo    byte number to place `byteValue`
 * @param  byteValue byte value to place at `byteNo` of `source`
 * @param  source    integer to place byte `byteValue` at `byteNo`
 * @return           newly formed byte
 */
unsigned int putByteNumber(int byteNo, unsigned char byteValue, unsigned int source) {
  if (byteNo > 3 || byteNo < 0) {
    return source;
  } else {

    unsigned char bytes[4];
    get_bytes(source, bytes);
    bytes[byteNo] = byteValue;
    return join_bytes(bytes);
  }
}

/**
 * Build a word from 4 passed bytes.
 * @param  byte0 byte for position 0
 * @param  byte1 byte for position 1
 * @param  byte2 byte for position 2
 * @param  byte3 byte for position 3
 * @return       newly formed word
 */
unsigned int buildWord(
  unsigned char byte0, unsigned char byte1,
  unsigned char byte2, unsigned char byte3
) {
  unsigned char bytes[4] = {byte0, byte1, byte2, byte3};
  return join_bytes(bytes);
}

/**
 * Return 1 if passed `source` is negative, otherwise 0.
 * @param  source integer to check for negativity
 * @return        1 if negative, otherwise 0
 */
bool isNegative(unsigned int source) {
  return (source & (1 << (sizeof(source) * 8 - 1)));
}

/**
 * Expand a integer `source` into an bit array seperated by spaces.
 * @param   source integer to split into bit array
 * @param   bits   bit array to place bits and formatting into
 */
void expandBits(unsigned int source, char bits[36]) {
  unsigned int i;
  int increment = 0;
  for (i = 0; i < 35; i++) {
    if (i == 8 || i == 17 || i == 26) {
      bits[(34 - i)] = ' ';
      increment++;
    } else {
      bits[(34 - i)] = (source & (1 << (i - increment))) ? '1': '0';
    }
  }
  bits[35] = 0;
}

/**
 * Clear a passed buffer `buff`.
 * @param buff buffer to clear
 * @param size size of buffer
 */
void clearBuffer(char * buff, int size) {
  memset(&buff[0], 0, size);
}

/**
 * [strdup description]
 * @param  str [description]
 * @return     [description]
 */
char *strdup(const char *str) {
    int n = strlen(str) + 1;
    char *dup = malloc(n);
    if(dup) {
        strcpy(dup, str);
    }
    return dup;
}

/**
 * [splitString description]
 * @param  line [description]
 * @return      [description]
 */
size_t splitString(char *line, char ***sve_record, char *split) {
  if (line == NULL || sve_record == NULL) {
    return 0;
  }
  size_t size = 0;
  char *tok = strtok(line, split);
  while (tok != NULL) {
    char **_tmp = realloc(*sve_record, sizeof(char *) * (size + 1));
    if (_tmp == NULL) {
      if (*sve_record != NULL)  {
        size_t i;
        for (i = 0; i < size; ++i) {
          free((*sve_record)[i]);
        }
        free(*sve_record);
      }
      return 0;
    }
    *sve_record = _tmp;
    (*sve_record)[size++] = strdup(tok);
    tok = strtok(NULL, split);
  }
  return size;
}