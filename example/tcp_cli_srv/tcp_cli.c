#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <unistd.h> 
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define DEFAULT_PORT 8800

void cmd_input(int fd);

int main(int argc, char** argv)
{
	int cPort = DEFAULT_PORT;
	int cClient = 0;
	int cLen = 0;
	struct sockaddr_in cli;
	char cbuf[4096] = {0};

	if(argc < 2)
	{
		printf("Uasge: client[server IP address]\n");
		return -1;
	}

	memset(cbuf, 0, sizeof(cbuf));

	cli.sin_family = AF_INET;
	cli.sin_port = htons(cPort);
	cli.sin_addr.s_addr = inet_addr(argv[1]);

	cClient = socket(AF_INET, SOCK_STREAM, 0);
	if(cClient < 0)
	{
		printf("socket() failure!\n");
		return -1; 
	}
        /*
        int opt = SO_REUSEADDR;
        setsockopt(cClient,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
        */

	if(connect(cClient, (struct sockaddr*)&cli, sizeof(cli)) < 0)
	{
		printf("connect() failure!\n");
		return -1;
	}

	cmd_input(cClient);
/*
	cLen = recv(cClient, cbuf, sizeof(cbuf),0);    
	if((cLen < 0)||(cLen == 0))
	{
		printf("recv() failure!\n");
		return -1;
	}
	printf("recv() Data From Server: [%s]\n", cbuf);
*/
	close(cClient);

	return 0;
}


void cmd_input(int fd)
{
	char buff[1024] = {0};
	int slen = 0;
	int rlen = 0;

	while(1)
	{
		printf(">:");
		scanf("%s",buff);
		slen = write(fd,buff,strlen(buff));
		if(slen < 0)
			return;
		rlen = read(fd,buff,1024);
		printf("<:%s [%d]\n",buff,rlen);
	}
}
