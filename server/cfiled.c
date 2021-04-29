#include "log.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>       // fork
#include <sys/stat.h>     // umask
#include <sys/resource.h> // struct rlimit
#include <signal.h>
#include <fcntl.h>
#include <syslog.h>

int deamonize()
{
    //忽略终端I/O信号，STOP信号
    signal(SIGTTOU, SIG_IGN);
    signal(SIGTTIN, SIG_IGN);
    signal(SIGTSTP, SIG_IGN);
    signal(SIGHUP, SIG_IGN);

    pid_t pid = fork();
    if (pid < 0)
    {
        printf("fail to fork \n");
        exit(-1);
    }
    else if (pid > 0)
    {
        printf("father process %d exit \n", pid);
        exit(0);
    }

    //建立一个新的进程组,在这个新的进程组中,子进程成为这个进程组的首进程,以使该进程脱离所有终端
    setsid();
    umask(0);

    //再次新建一个子进程，退出父进程，保证该进程不是进程组长，同时让该进程无法再打开一个新的终端
    pid = fork();
    if (pid < 0)
    {
        printf("fail to fork \n");
        exit(-1);
    }
    else if (pid > 0)
    {
        printf("father process %d exit \n", pid);
        exit(0);
    }

    //忽略SIGCHLD信号
    signal(SIGCHLD, SIG_IGN);

    // 切换当前目录到根目录
    chdir("/");

    // 关闭文件描述符
    // for (int i = 0; i < getdtablesize(); ++i)
    // {
    //     printf("close file %d \n", i);
    //     close(i);
    // }

    int fd0 = open("/dev/null", O_RDWR);
    int fd1 = dup(0);
    int fd2 = dup(0);

    openlog("cfiled", LOG_CONS, LOG_DAEMON);

    return 0;
}

int main(int argc, char *argv[])
{
    deamonize();
    FILE * log = fopen(LOG_FILE_PATH, "a+");

    int i = 9393;
    while (1)
    {
        sleep(2);
        // syslog(LOG_ERR, "%d \n", i++);
        fprintf(log, "%d\n", ++i);
        printf("%d\n", ++i);
    }
    fclose(log);

    return 0;
}