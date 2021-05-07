#include "../protocal.h"
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

void listen_client(int port)
{
    struct sockaddr saddr;
    struct sockaddr_in servaddr;
    int tmp;
    int sfd;              // socket  file describtor
    int cfd;              // content file describtor
    int n;                // size received
    struct response resp; //response
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

        ////////////////////////////////////////////////
        memset(resp.body, 0, sizeof(resp.body));
        resp.errcode = 200;

        // 接受客户端传过来的数据
        n = recv(cfd, &method, sizeof(method), 0);

        // 数据处理
        if (0 == strcmp(method.key, "list"))
        {
            char *dirstring = listfile(method.value);
            strcpy(resp.body, dirstring);
            free(dirstring);
            resp.errcode = 200;
            // 向客户端发送回应数据
            if (send(cfd, &resp, sizeof(resp), 0) == -1)
            {
                perror("send error");
            }
        }
        else if (0 == strcmp(method.key, "get"))
        {
            FILE *file = fopen(method.value, "r");
            if (NULL == file)
            {
                resp.errcode = 404;
                strcpy(resp.body, "no such file: ");
                strcpy(resp.body + strlen(resp.body), method.value);
                if (send(cfd, &resp, sizeof(resp), 0) == -1)
                {
                    printf("file not found : %s\n", method.value);
                }
            }
            else
            {
                while (fread(&(resp.body), sizeof(char), sizeof(resp.body), file) > 0)
                {
                    send(cfd, &resp, sizeof(resp), 0);
                    printf("%s\n", resp.errcode, resp.body);
                    memset(&(resp.body), 0, sizeof(resp.body));
                }
                fclose(file);
            }
        }
        else
        {
            resp.errcode = -1;
            strcpy(resp.body, "error \n");
            send(cfd, &resp, sizeof(resp), 0);
        }

        ////////////////////////////////////////////////
        printf("%s: %s \n", method.key, method.value);
        close(cfd);
    }
    close(sfd);
}