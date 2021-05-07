#include "client_net.h"
#include "config.h"
#include "../protocal.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

void *send_method(struct method *pmethod)
{
    struct sockaddr_in caddr;
    struct response resp;
    int tmp;
    int sfd; // socket  file describtor
    int n;   // size received

    sfd = socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == sfd)
    {
        printf("err: socket create\n");
        exit(0);
    }

    //初始化
    char *host = read_config_file("host");
    char *pstr = read_config_file("port");

    int port = atoi(pstr);
    free(pstr);
    memset(&caddr, 0, sizeof(caddr));
    caddr.sin_family = AF_INET;
    // 把IP地址从字符串转为int
    inet_pton(AF_INET, host, &caddr.sin_addr);
    caddr.sin_port = htons(port);

    tmp = connect(sfd, (struct sockaddr *)&caddr, sizeof(caddr));
    if (-1 == tmp)
    {
        printf("err: socket connect \n");
        exit(0);
    }

    send(sfd, pmethod, sizeof(*pmethod), 0);

    n = recv(sfd, &resp, sizeof(resp), 0);
    if (200 != resp.errcode)
    {
        printf("err: socket receve, error code %d\n", resp.errcode);
    }
    else
    {
        puts(resp.body);
    }

    close(sfd);
    free(host);
}

void *send_file_method(struct method *pmethod)
{
    struct sockaddr_in caddr;
    struct response resp;
    int tmp;
    int sfd; // socket  file describtor
    int n;   // size received

    sfd = socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == sfd)
    {
        printf("err: socket create\n");
        exit(0);
    }

    //初始化
    char *host = read_config_file("host");
    char *pstr = read_config_file("port");

    int port = atoi(pstr);
    free(pstr);
    memset(&caddr, 0, sizeof(caddr));
    caddr.sin_family = AF_INET;
    // 把IP地址从字符串转为int
    inet_pton(AF_INET, host, &caddr.sin_addr);
    caddr.sin_port = htons(port);

    tmp = connect(sfd, (struct sockaddr *)&caddr, sizeof(caddr));
    if (-1 == tmp)
    {
        printf("err: socket connect \n");
        exit(0);
    }

    send(sfd, pmethod, sizeof(*pmethod), 0);

    if (0 == strcmp(pmethod->key, "get"))
    {
        FILE * file = NULL;
        if ((n = recv(sfd, &resp, sizeof(resp), 0)) > 0)
        {
            //接受客户端传过来的数据
            if (200 == resp.errcode)
            {
                // 先判断此文件在本地是否已经存在，存在则不下载
                file = fopen(pmethod->value, "w");
                fputs(resp.body, file);
                printf("%d\n", n);
                memset(&resp, 0, sizeof(resp));
                while ((n = recv(sfd, &resp, sizeof(resp), 0)) > 0)
                {
                    printf("%d\n", n);
                    fwrite(resp.body, 1, sizeof(resp.body), file);
                    // fputs(resp.body, file);
                    memset(&resp, 0, sizeof(resp));
                }
                fclose(file);
                printf("get %s done\n", pmethod->value);
            }
            else if (404 == resp.errcode)
            {
                printf("%s\n", resp.body);
            }
            else
            {
                printf("err: socket received, error code %d\n", resp.errcode);
            }
        }
    }
    else
    {

    }

    close(sfd);
    free(host);
}