#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include "tools.h"
#include "memory.h"
#include "loader.h"
unsigned int senti_addr = 0x000;
unsigned int senti_count = NULL;

/**
 * [validFileName description]
 * @param  file_name [description]
 * @return           [description]
 */
bool validFileName(char *file_name) {
    const char *dot = strrchr(file_name, '.');
    if (!(!dot || dot == file_name)) {
        return (strcmp(dot, ".yo") == 0);
    }
    return false;
}

/**
 * [discardRest description]
 * @param line   [description]
 * @param p_file [description]
 */
void discardRest(char *line, FILE * p_file) {
    unsigned int i;
    for (i = 0; i < sizeof(line); i++) {
        if (line[i] != '\n') {
            p_file++;
        }
    }
}

/**
 * [hasAddress description]
 * @param  record [description]
 * @return        [description]
 */
bool hasAddress(char ** record) {
    if (record[0]) {
        return isxdigit(*record[0]);
    } else {
        return false;
    }
}

/**
 * [hasData description]
 * @param  record [description]
 * @return        [description]
 */
bool hasData(char ** record) {
    if (record[1]) {
        return isxdigit(*record[1]);
    } else {
        return false;
    }
}

/**
 * [grabAddress description]
 * @param  line [description]
 * @return      [description]
 */
unsigned int extractAddress(char ** record) {
    if (hasAddress(record)) {
        char * p_end;
        return strtol(record[0], &p_end, 16);
    }
}

/**
 * [checkRecord description]
 * @param  record [description]
 * @return        [description]
 */
bool checkRecord(char ** record) {
    bool valid = true;
    if (hasAddress(record)) {
        char record_addr[25];
        strcpy(record_addr, record[0]);
        remchars(record_addr, ':');
        chopStart(record_addr, 2);
        if (hasData(record)) {
            valid = (
                (strlen(record[1]) % 2 == 0) &&
                (extractAddress(record) == senti_addr)
            );
        }
        valid = (
            record[0][1] == 'x' &&
            record[0][5] == ':' &&
            isxstr(record_addr) &&
            (strlen(record[0]) <= 6)
        ) && valid;
    }
    return valid;
}

/**
 * [extractData description]
 * @param  record      [description]
 * @param  data_record [description]
 * @return             [description]
 */
size_t extractData(char ** record, unsigned char * data_record) {
    unsigned int retnDataIdx = 0;
    if (hasData(record)) {
        int i;
        for (i = 0; (record[1][i] != '\0' && record[1][i+1] != '\0'); i += 2, retnDataIdx++) {
            char p_str[6];
            sprintf(p_str, "%c%c", record[1][i], record[1][i+1]);
            data_record[retnDataIdx] = strtoumax(p_str, NULL, 16);
        }
    }
    return retnDataIdx;
}

/**
 * [load description]
 * @param  file_name [description]
 * @return           [description]
 */
bool load(int argv, char * args[]) {
    
    
    if (argv > 1) {
        char * file_name = args[1];
        if (sizeof(file_name) >= 0) {
            if (validFileName(file_name)) {
                if (access(file_name, F_OK) != -1) {
                    FILE * p_file = fopen(file_name, "rt");
                    char line[256];
                    char line_current[256];
                    int line_count = 1;
                    while (fgets(line, sizeof(line), p_file)) {
                        strcpy(line_current, line);
                        int pipeCheck = checkPipe(line_current);
                        if (pipeCheck > 0 && pipeCheck != 22) {
                            printf("Error on line %d\n%s\n", line_count, line_current);
                            fclose(p_file);
                            return false;
                        }
                        char **record = NULL;
                        size_t s_record = splitString(line, &record, " ");
                        if ((s_record > 0) && checkRecord(record)) {
                            unsigned char record_data[6];
                            size_t s_record_data = extractData(record, &record_data);
                            unsigned int p_addr = extractAddress(record);
                            unsigned int i;
                            // senti_count = (senti_count == NULL) ? p_addr : senti_count;
                            for (i = 0; i < s_record_data; i++, p_addr++, senti_count++) {
                                bool memError = false;
                                putByte(p_addr, record_data[i], &memError);
                            }
                            senti_addr = p_addr;
                        } else {
                            printf("Error on line %d\n%s\n", line_count, line_current);
                            fclose(p_file);
                            return false;
                        }
                        line_count++;
                    } 
                    fclose(p_file);
                    return true;
                }
            }
        }
    }
    printf("file opening failed\nusage: yess <filename>.yo\n");
    return false;
}
