#include <stdio.h>

#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>

#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/errno.h>
#include <string.h>

#include "socket/unix_socket.h"


typedef struct _server_fds
{	
	fd_set client_fds;	//客户端的fd
	fd_set read_fds;	//可读的fd
	fd_set write_fds;	//可写的fd
	fd_set	error_fds;	//出错的fd
	int num_fds;
}server_fds_t;


char *server_path="/tmp/unix_server_listener";

int main()
{
	//init pararms
	
	int ret;
	char buff[2048];
	server_fds_t serverfds; 

	int unix_server = 0;
	int new_sock;
	unix_server = create_unixsocket_listener(server_path);

	if(unix_server < 0)
		exit(-1);


	FD_ZERO(&(serverfds.client_fds));

	while(1)
	{
		FD_SET(unix_server,&(serverfds.read_fds));	//添加监听服务
		//FD_SET(SOCK_FD(udp_serv),&(serverfds.read_fds));	//添加监听服务
		
		ret = select(FD_SETSIZE,&(serverfds.read_fds),NULL,NULL,NULL);
		if(ret < 0)
		{
			//iferr
		}
		serverfds.num_fds = ret;
		if(serverfds.num_fds == 0)
			continue;

		if(FD_ISSET(unix_server,&(serverfds.read_fds)))
		{
			//net tcp incomming data
			new_sock = accept(unix_server,NULL,NULL);
			printf("client comming ..... \n");
			if(new_sock < 0)
			{
				continue;
			}
			//添加到一个客户端队列中继续使用select模型处理
			//创建一个线程单独处理
			//单任务模式，短连接，直接处理掉

			//real work
			read(new_sock,buff,1024);
			printf("read data %s\n",buff);
			serverfds.num_fds--;
			close(new_sock);
		}
	
		
	}
	
}
