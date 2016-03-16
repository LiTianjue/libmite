#include <stdio.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <sys/time.h>
#include <errno.h>


#include <sys/ioctl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>


#include "inc/sockethelper/socket_helper.h"
/*-----------------------------------------------
 * 功能：填充tcp地址结构
 *----------------------------------------------*/
int mite_sock_initAddr(struct sockaddr_in *addr,char *ip,int port)
{
    bzero(addr, sizeof(struct sockaddr_in));
    addr->sin_family = AF_INET;
    addr->sin_port = htons(port);
    if (inet_pton(AF_INET,ip, &(addr->sin_addr)) <= 0 ){
        return -1;
    };

    return 0;
}

/*-----------------------------------------------
 * Name : opensocket by tiemout
 * Error:
 *		-1 : socket
 *		-2 : connect
 *		-3 : socket_error
 *		-4 : timeout or else
 *----------------------------------------------*/
int mite_sock_openSocketByTimeout(char *ip, int port, int timeout)
{
    int sock_fd;
    struct sockaddr_in port_info;
    int ret, optval;


    ret = mite_sock_initAddr(&port_info,ip,port);
    if(ret != 0)
        return -1;

    if ((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        return -1;
    }

    optval = 1;
    ioctl(sock_fd, FIONBIO, &optval);

    if (connect(sock_fd, (struct sockaddr *) &port_info, sizeof(port_info)) < 0)
    {
        int error = -1;
        struct timeval tm;
        fd_set set;
        socklen_t len;

        if (errno != EWOULDBLOCK && errno != EINPROGRESS)
        {
            close(sock_fd);
            return -2;
        }

        //set timeout
        tm.tv_sec  = timeout;
        tm.tv_usec = 0;
        FD_ZERO(&set);
        FD_SET(sock_fd, &set);
        if(select(sock_fd + 1, NULL, &set, NULL, &tm) > 0)
        {
            len = sizeof(socklen_t);
            getsockopt(sock_fd, SOL_SOCKET, SO_ERROR, (char*)&error, &len);
            if(error != 0)
            {
                close(sock_fd);
                return -3;
            }
        }
        else
        {
            close(sock_fd);
            return -4;
        }
    }

    optval = 0;
    ioctl(sock_fd, FIONBIO, &optval);

    return sock_fd;
}

int connectWithTimeout(char *ip,int port,int timeout)
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0)
        return -1;
    struct sockaddr_in serv_addr;
    memset(&serv_addr,0,sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(ip);
    serv_addr.sin_port = htons(port);

    int error=-1, len;
    len = sizeof(int);
    struct timeval tm;
    fd_set set;
    unsigned long ul = 1;
    ioctl(sockfd, FIONBIO, &ul); //设置为非阻塞模式
    int  ret = -1;
    if(connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
    {
        tm.tv_sec = timeout;
        tm.tv_usec = 0;
        FD_ZERO(&set);
        FD_SET(sockfd, &set);
        if(select(sockfd+1, NULL, &set, NULL, &tm) > 0)
        {
            getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &error, (socklen_t *)&len);
            if(error == 0) ret = 0;
            else ret = -1;
        } else ret = -1;
    }
    else ret = 0;
    ul = 0;
    ioctl(sockfd, FIONBIO, &ul); //设置为阻塞模式
    if(ret != 0)
    {
        close( sockfd );
        fprintf(stderr , "Cannot Connect the server!\n");
        return -1;
    }
    fprintf( stderr , "Connected!\n");
    return sockfd;
}

/*---------------------------
 * 功能:保证数据完全发送
 * 返回值:
 *		0 : 成功
 *	   -1 : select 出错
 *	   -2 : 写超时
 *	   -3 : write 出错
 *--------------------------*/
int mite_sock_writeWithTimeout(int sockfd,char*buff,int len,int timeout)
{
    int ret = 0;
    int pos = 0;
    int h = 0;
    fd_set   t_set1;
    struct timeval  tv;

    while(len > 0)
    {
        tv.tv_sec = timeout;
        tv.tv_usec = 0;
        FD_ZERO(&t_set1);
        FD_SET(sockfd, &t_set1);

        h = select(sockfd+1,NULL,&t_set1,NULL,&tv);
        if(h < 0)
        {
            //select error
            return -1;
        }else if(h == 0)
        {
            //time out
            return -2;
        }else if(h > 0)
        {
            // socket can be write
            ret = write(sockfd,buff+pos,len);
            if (ret < 0)
                return -3;
            len -= ret;
            pos += ret ;
        }
    }
    return 0;
}

/*---------------------------
 * 功能:保证一次读全http response
 * 返回值:
 *		0 : 成功
 *	   -1 : select 出错
 *	   -2 : 读超时
 *	   -3 : read 出错
 *--------------------------*/
int mite_sock_readWithTimeout(int sockfd,char *buff,int timeout)
{
    int ret = 0;
    int h = 0;
    int pos = 0;

    fd_set   t_set1;
    struct timeval  tv ;

    while(1)
    {
        tv.tv_sec =timeout;
        tv.tv_usec = 0;
        FD_ZERO(&t_set1);
        FD_SET(sockfd, &t_set1);

        h = select(sockfd+1,&t_set1,NULL,NULL,&tv);
        if(h < 0)
        {
            return -1;
        }else if(h == 0)
        {
            return -2;
        }else if(h > 0)
        {
            // socket can be read
            ret = read(sockfd,buff+pos,MAXLINE);
            if(ret <= 0)
            {
                // read error;
                return -3;
            }

            return ret;
        }
    }
}
