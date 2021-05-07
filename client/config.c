#include "config.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char *read_config_file(char *name)
{
    char * value = NULL;
    FILE * file = fopen(CONFIG_FILE_PATH, "a+");
    char buffer[128];

    while (!feof(file))
    {
        fgets(buffer, sizeof(buffer), file);
        if (buffer[strlen(buffer)-1]=='\n')
        {
            buffer[strlen(buffer)-1] = '\0';
        }

        if (0 == strncmp(name, buffer, strlen(name)))
        {
            int len = strlen(buffer)-strlen(name);
            value = (char*)malloc(len * sizeof(char));
            strncpy(value, buffer+strlen(name)+1, len);
            value[len-1] = '\0';
            break;
        }
    }

    fclose(file);
    return value;
}

bool write_config_file(char *name, char *value)
{
    bool state = true;
    char buffer[4096];
    memset(buffer, 0, sizeof(buffer));
    FILE * file = fopen(CONFIG_FILE_PATH, "a+");
    int len = 0;

    while (!feof(file))
    {
        fgets(buffer+len, sizeof(buffer), file);
        len = sizeof(buffer);
        // if (0 == strncmp(name, buffer, strlen(name)))
        // {
        //     // 修改文件内容，重新读写一遍
        // }
    }
    fclose(file);

    file = fopen(CONFIG_FILE_PATH, "w");
    fprintf(file, "%s=%s\n", name, value);
    fputs(buffer, file);
    fclose(file);
    return state;
}