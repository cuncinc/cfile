#include "client_net.h"
#include "config.h"
#include "../method.h"

#include <stdio.h>
#include <string.h>
#include <getopt.h> //引入getopt_long工具解析长选项

// getopt_long要求的声明，
// optarg —— 指向当前选项参数(如果有)的指针。
// optind —— 再次调用 getopt() 时的下一个argv指针的索引。
// optopt —— 最后一个未知选项。
// opterr ­—— 如果不希望getopt()打印出错信息，则只要将全域变量opterr设为0即可。
extern int optind, opterr, optopt;
extern char *optargi;

//定义长选项
static struct option long_options[] =
{
    //命令行参数，参数，返回值是value，返回的flag
    {"help", no_argument, NULL, 'h'},
    {"version", no_argument, NULL, 'v'},
    {"host", required_argument, NULL, 'o'},
    {"port", required_argument, NULL, 'p'},
    {"list", required_argument, NULL, 'l'},
    {"get", required_argument, NULL, 'g'},
};

int main(int argc, char *argv[])
{
    struct method method;
    memset(&method, 0, sizeof(method));
    int index = 0;
    int c = 0;                       //用于接收选项
    char optstring[] = "hvo:p:l:g:"; //无:则无参数，一个:必须有参数，二个:可选参数
    /*循环处理参数*/
    while (EOF != (c = getopt_long(argc, argv, optstring, long_options, &index)))
    {
        switch (c)
        {
        case 'h':
            printf("help\n");
            break;
        case 'v':
            printf("cfile version 0.0.1\n");
            break;
        case 'o':
            printf("IP is %s\n", optarg);
            write_config_file("host", optarg);
            break;
        case 'p':
            printf("Port is %s\n", optarg);
            write_config_file("port", optarg);
            break;
        case 'l':
            strncpy(method.key, "list", 4);
            strncpy(method.value, optarg, strlen(optarg));
            // printf("Dir is %s\n", optarg);
            send_message(&method);
            break;
        case 'g':
            printf("Get is %s\n", optarg);
            break;

        //表示选项不支持
        // case '?':
        //     printf("cfile: '%s' is not a cfile command. See 'cfile --help'.\n", argv[optind]);
        //     break;
        default:
            break;
        }
    }
    return 0;
}