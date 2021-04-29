#include "net.h"
#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

void *send_message(char *message)
{
    struct sockaddr_in caddr;
    int tmp;
    int sfd; // socket  file describtor
    int n;   // size received
    char buffer[MAXLINE];

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
    // inet_pton是Linux下IP地址转换函数，将IP地址在“点分十进制”和“整数”之间转换
    inet_pton(AF_INET, host, &caddr.sin_addr);
    caddr.sin_port = htons(port); //设置的端口为DEFAULT_PORT

    tmp = connect(sfd, (struct sockaddr *)&caddr, sizeof(caddr));
    if (-1 == tmp)
    {
        printf("err: socket connect \n");
        exit(0);
    }

    send(sfd, message, strlen(message), 0);

    //接受客户端传过来的数据
    n = recv(sfd, buffer, MAXLINE, 0);
    buffer[n] = '\0';
    printf("recv msg from server: %s\n", buffer);

    close(sfd);
    free(host);
    return NULL;
}