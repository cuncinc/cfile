#ifndef _PROTOCAL_H
#define _PROTOCAL_H

struct method
{
    char key[16];
    char value[512];
};

struct response
{
    int errcode;
    char body[4096];
};

#endif