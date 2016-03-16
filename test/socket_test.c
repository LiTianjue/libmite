#include "inc/sockethelper/socket_helper.h"
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>



int main(int argc,char *argv[])
{
    if(argc < 3)
    {
        printf("usage :\n");
        printf("%s ip port\n",argv[0]);
        return 0;
    }

    int sock= mite_sock_openSocketByTimeout(argv[1],atoi(argv[2]),2);
    if(sock <=0)
    {
        perror("connect:");
        return 0;
    }
    printf("Connect to %s:%s success.\n",argv[1],argv[2]);

    int ret = 0;
    char *msg = "hello world.";
    ret = mite_sock_writeWithTimeout(sock,msg,strlen(msg),2);
    if(ret < 0)
    {
        perror("Send Error.\n");
    }
    else
    {
        char rdata[256] = {0};
        ret = mite_sock_readWithTimeout(sock,rdata,2);
        if(ret > 0)
        {
            printf("recv[%d] : %s\n",ret,rdata);
        }
        else
        {
            perror("recv:");
        }
    }

    close(sock);
    return 0;
}
