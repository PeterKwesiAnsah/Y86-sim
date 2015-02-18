#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include "tools.h"
#include "memory.h"
#include "loader.h"


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
 * [extractData description]
 * @param  record [description]
 * @return        [description]
 */
unsigned int extractData(char ** record) {
    if (hasData(record)) {
        char * p_end;
        return strtol(record[1], &p_end, 16);
    }
}

/**
 * [checkRecord description]
 * @param  record [description]
 * @return        [description]
 */
bool checkRecord(char ** record) {
    return (hasAddress(record) && hasData(record));
}

/**
 * [load description]
 * @param  file_name [description]
 * @return           [description]
 */
bool load(char *file_name) {
    if (sizeof(file_name) >= 0) {
        if (validFileName(file_name)) {
            if (access(file_name, F_OK) != -1) {
                FILE * p_file = fopen(file_name, "rt");
                char line[80];
                int line_count = 1;
                while (fgets(line, sizeof(line), p_file)) {
                    char **record = NULL;
                    size_t record_size = splitString(line, &record, " ");
                    if ((record_size > 0) && checkRecord(record)) {
                        bool memError = false;
                        putWord(extractAddress(record), extractData(record), &memError);
                    } else {
                        printf("Error on line %d\n%s\n", line_count, line);
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
    printf("file opening failed\nusage: yess <filename>.yo\n");
    return false;
}

