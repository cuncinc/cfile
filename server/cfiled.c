#include "log.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>       // fork
#include <sys/stat.h>     // umask
#include <sys/resource.h> // struct rlimit
#include <signal.h>
#include <fcntl.h>
#include <syslog.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>

#define MAXLINE 4096

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

void listen_client(int port)
{
    struct sockaddr saddr;
    struct sockaddr_in servaddr;
    int tmp;
    int sfd; // socket  file describtor
    int cfd; // content file describtor
    int n;   // size received
    char buffer[MAXLINE];

    sfd = socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == sfd)
    {
        printf("err: socket create\n");
        exit(0);
    }

    //初始化
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY); //IP地址设置成INADDR_ANY,让系统自动获取本机的IP地址。
    servaddr.sin_port = htons(port);              //设置的端口为DEFAULT_PORT

    tmp = bind(sfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
    if (-1 == tmp)
    {
        printf("err: socket bind \n");
        exit(0);
    }

    tmp = listen(sfd, 10);
    if (-1 == listen(sfd, 10))
    {
        printf("err: socket listen \n");
        exit(0);
    }

    while (1)
    {
        cfd = accept(sfd, (struct sockaddr *)NULL, NULL);
        if (-1 == cfd)
        {
            printf("err: socket accept \n");
            continue;
        }

        //接受客户端传过来的数据
        n = recv(cfd, buffer, MAXLINE, 0);
        //向客户端发送回应数据
        if (!fork())
        {
            if (send(cfd, "Hello,you are connected!\n", 26, 0) == -1)
                perror("send error");
            close(cfd);
            exit(0);
        }
        buffer[n] = '\0';
        printf("recv msg from client: %s\n", buffer);
        close(cfd);
    }
    close(sfd);
}

int main(int argc, char *argv[])
{
    deamonize();
    FILE *log = fopen(LOG_FILE_PATH, "a+");

    listen_client(9393);

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