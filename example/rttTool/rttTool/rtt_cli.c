#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h> 
#include <unistd.h> 
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "client_util.h"

#define DEFAULT_PORT 8800
#define MAX_BUFFER_SIZE	1024

void cmd_input(int fd);
void rtt_test(int fd);
void big_data_test(int fd);

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
	if(argc >= 3)
	{
		cPort = atoi(argv[2]);
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
        
        int opt = SO_REUSEADDR;
        setsockopt(cClient,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
        

	if(connect(cClient, (struct sockaddr*)&cli, sizeof(cli)) < 0)
	{
		printf("connect() failure!\n");
		return -1;
	}
	printf("connect success.\n");

	//cmd_input(cClient);
	//rtt_test(cClient);
	big_data_test(cClient);

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


static long long total = 10*1024*1024;	//10M数据
static long long snd_ = 0;
static long long rev_ = 0;


void big_data_test(int fd)
{
    char buff[MAX_BUFFER_SIZE] = {0};
    int len = MAX_BUFFER_SIZE;

    int slen = 0;
    int rlen = 0;
	
	uint64_t start_ = 0;
	uint64_t end_   = 0;
    uint32_t index = 0;

	snd_  = 0;
	rev_  = 0;
	start_ = iclock64();
    while(1)
    {
		if(snd_ < total){
			memset(buff,'1',MAX_BUFFER_SIZE);
			slen = write(fd,buff,len);
			if(slen < 0)
				return;
			snd_+= slen;
			//printf("----->send %d\n",index++);
		}
		
		if(rev_ < total)
		{
			memset(buff,'\0',MAX_BUFFER_SIZE);
			rlen = read(fd,buff,MAX_BUFFER_SIZE);
			//printf("<-----read %d\n",rlen);
			if(rlen < 0)
				return;
			rev_+= rlen;
		}
		else
		{
			break;
		}
    }
	end_ = iclock64();
	printf("cost  :%ld ms\n",end_-start_);
}



void rtt_test(int fd)
{
    uint32_t index = 0;
    uint64_t sumrtt = 0;
    int count = 0;
    int maxrtt = 0;

    char buff[MAX_BUFFER_SIZE] = {0};
    int len = MAX_BUFFER_SIZE;
    uint32_t current = iclock();

    int slen = 0;
    int rlen = 0;
    while(1)
    {
        //每20ms发送1k数据，发送1000次
        millisecond_sleep(10);
		//sleep(2);
        current = iclock();

        *(uint32_t*)(buff + 0) = index++;
        *(uint32_t*)(buff + 4) = current;
		//printf("before :%lu\n",current);
        slen = write(fd,buff,len);
        if(slen < 0)
            return;
		memset(buff,'\0',MAX_BUFFER_SIZE);
        rlen = read(fd,buff,MAX_BUFFER_SIZE);
        if(rlen < 0)
            return;

        current = iclock();
        uint32_t sn = *(uint32_t*)(buff + 0);
        uint32_t ts = *(uint32_t*)(buff + 4);
        uint32_t rtt = current - ts;
		//if(rtt >= 0 && rtt < 500)
		if(1)
		{
			sumrtt += rtt;
			count++;
			if(rtt > (uint32_t)maxrtt)
				maxrtt=rtt;
		}
		else
		{
			printf("sn=%lu rtt=%lu\n",sn, rtt);
			printf("------------->ts=%lu current=%lu\n",ts,current);
		}

		printf("sn=%lu rtt=%lu\n",sn, rtt);
		//printf("after :%lu\n",current);

        if(index==1000)
            break;
    }
    printf("avgrtt=%d maxrtt=%d\n", (int)(sumrtt / count), maxrtt);
}






















