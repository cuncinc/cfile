#include <stdio.h>
#include <getopt.h> //引入getopt_long工具解析长选项

//getopt_long要求的声明
extern int optind, opterr, optopt;
extern char *optargi;

//定义长选项
static struct option long_options[] =
{
    {"help", no_argument, NULL, 'h'},
    {"version", no_argument, NULL, 'v'},
};

int main(int argc, char *argv[])
{
    int index = 0;
    int c = 0; //用于接收选项
    /*循环处理参数*/
    while (EOF != (c = getopt_long(argc, argv, "hv:", long_options, &index)))
    {
        switch (c)
        {
        case 'h':
            printf("help\n");
            break;
        case 'v':
            printf("cfile version 0.0.1\n");
            break;
        //-l选项必须要参数
        // case 'l':
        //     printf("list the %s\n", optarg);
        //     break;

        //表示选项不支持
        // case '?':
        //     printf("cfile: '%s' is not a cfile command. See 'cfile --help'.\n", );
        //     break;
        default:
            break;
        }
    }



    return 0;
}