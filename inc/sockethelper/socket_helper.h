#ifndef SOCKET_HELPER
#define SOCKET_HELPER

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/select.h>

#define MAXLINE 1024

int mite_sock_initAddr(struct sockaddr_in *addr,char *ip,int port);

int mite_sock_openSocketByTimeout(char *ip,int port,int timeout);

int  mite_sock_writeWithTimeout(int sockfd,char *buffer,int len,int timeout);

int mite_sock_readWithTimeout(int sockfd,char *buffer,int timeout);




#endif // SOCKET_HELPER

