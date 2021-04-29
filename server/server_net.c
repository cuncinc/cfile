#include "../method.h"
#include "server_net.h"
#include "file_system.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>       // fork
#include <sys/stat.h>     // umask
#include <sys/resource.h> // struct rlimit
#include <signal.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>

#define MAXLINE 4096

void listen_client(int port)
{
    struct sockaddr saddr;
    struct sockaddr_in servaddr;
    int tmp;
    int sfd;            // socket  file describtor
    int cfd;            // content file describtor
    int n;              // size received
    char resp[MAXLINE]; //response
    struct method method;

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

        memset(resp, 0, sizeof(resp));

        // 接受客户端传过来的数据
        n = recv(cfd, &method, MAXLINE, 0);

        // 数据处理
        if (0 == strncmp(method.key, "list", 4))
        {
            char * dirstring = listfile(method.value);
            strcpy(resp, dirstring);
            free(dirstring);
        }
        else if (0 == strncmp(method.key, "get", 3))
        {
            strcpy(resp, "get ");
            strcpy(resp + 5, method.value);
        }
        else
        {
            strcpy(resp, "error \n");
        }

        // 向客户端发送回应数据
        if (send(cfd, resp, strlen(resp), 0) == -1)
        {
            perror("send error");
        }

        printf("%s: %s \n", method.key, method.value);
        close(cfd);
    }
    close(sfd);
}