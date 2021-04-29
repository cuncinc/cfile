#include "file_system.h"

#include <dirent.h>   // dirent声明
#include <unistd.h>   // opendir声明
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

char nofile_message[] = "cannot access: No such file or directory";

char *listfile(const char *dirname)
{
    char *dirstring = (char *)malloc(sizeof(char) * 4096);
    int len = 0;

    // 目录下的每个子项文件
    struct dirent *entry;

    DIR *dir = opendir(dirname);
    if (NULL == dir)
    {
        printf("error: directory %s cannot access\n", dirname);
        return nofile_message;
    }

    chdir(dirname);
    // 对目录遍历
    while (NULL != (entry = readdir(dir)))
    {
        // 遍历的结果有 . 和 .. 2个目录，应忽略之
        if (0 == strcmp(".", entry->d_name) || 0 == strcmp("..", entry->d_name))
        {
            continue;
        }

        strncpy(dirstring+len, entry->d_name, strlen(entry->d_name));
        len += strlen(entry->d_name);
        if (DT_DIR == entry->d_type)
        {
            dirstring[len++] = '/';
        }
        dirstring[len++] = '\n';
    }
    dirstring[len++] = '\0';

    chdir("..");
    closedir(dir);
    return dirstring;
}