#include "client_net.h"
#include "config.h"
#include "../protocal.h"

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

static void usage(const char *argv0)
{
    printf("usage:\t%s [option]\n\n", argv0);
    printf("\t--help(-h)\n"
           "\t\tHelp.\n\n");
    printf("\t--list(-l) dir\n"
           "\t\tList directory.\n\n");
    printf("\t--get(-g) filename\n"
           "\t\tDownload file.\n\n");
    // printf("\t--syslog\n"
    //        "\t\tUse syslog for request log.\n\n");
    // printf("\t--chroot (default: don't chroot)\n"
    //        "\t\tLocks server into wwwroot directory for added security.\n\n");
    // printf("\t--daemon (default: don't daemonize)\n"
    //        "\t\tDetach from the controlling terminal and run in the background.\n\n");
    // printf("\t--no-listing\n"
    //        "\t\tDo not serve listing if directory is requested.\n\n");
    // printf("\t--mimetypes filename (optional)\n"
    //        "\t\tParses specified file for extension-MIME associations.\n\n");
    // printf("\t--uid uid/uname, --gid gid/gname (default: don't privdrop)\n"
    //        "\t\tDrops privileges to given uid:gid after initialization.\n\n");
    // printf("\t--pidfile filename (default: no pidfile)\n"
    //        "\t\tWrite PID to the specified file.  Note that if you are\n"
    //        "\t\tusing --chroot, then the pidfile must be relative to,\n"
    //        "\t\tand inside the wwwroot.\n\n");
    // printf("\t--no-keepalive\n"
    //        "\t\tDisables HTTP Keep-Alive functionality.\n\n");
    // printf("\t--forward host url (default: don't forward)\n"
    //        "\t\tWeb forward (301 redirect).\n"
    //        "\t\tRequests to the host are redirected to the corresponding url.\n"
    //        "\t\tThe option may be specified multiple times, in which case\n"
    //        "\t\tthe host is matched in order of appearance.\n\n");
    // printf("\t--forward-all url (default: don't forward)\n"
    //        "\t\tWeb forward (301 redirect).\n"
    //        "\t\tAll requests are redirected to the corresponding url.\n\n");
    // printf("\t--no-server-id\n"
    //        "\t\tDon't identify the server type in headers\n"
    //        "\t\tor directory listings.\n\n");
    // printf("\t--auth username:password\n"
    //        "\t\tEnable basic authentication.\n\n");
}

int main(int argc, char *argv[])
{
    if (1 == argc)
    {
        usage(argv[0]);
        return 0;
    }
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
        case 'h':   // help
            usage(argv[0]);
            break;
        case 'v':   // version
            printf("cfile version 0.0.1\n");
            break;
        case 'o':   // host
            printf("IP is %s\n", optarg);
            write_config_file("host", optarg);
            break;
        case 'p':   // port
            printf("Port is %s\n", optarg);
            write_config_file("port", optarg);
            break;
        case 'l':   // list
            strcpy(method.key, "list");
            strcpy(method.value, optarg);
            // printf("Dir is %s\n", optarg);
            send_method(&method);
            break;
        case 'g':   // get
            strcpy(method.key, "get");
            strcpy(method.value, optarg);
            send_file_method(&method);
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