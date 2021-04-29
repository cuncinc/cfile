#ifndef _CONFIG_H
#define _CONFIG_H

#include <stdbool.h>

// 要放到Linux的配置文件目录中才行
#define CONFIG_FILE_PATH "./cfile_client.ini"

/**
 * read config of name from the config file, if not found, return null
 * return: config of name
 * opt: config name
 */
char *read_config_file(char *name);

/**
 * write config name and value into the config file,
 * if the config is exists, over write id,
 * if fail, return false
 */
bool write_config_file(char *name, char *value);

#endif