#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <time.h>    
#include <sys/time.h>    
#include <signal.h>  

#include <net/if.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/tcp.h> 

#define MAC_ADDR_LEN	6
#define MAC_NUM			3


int readMacAddr(char* eth_dev,unsigned char* buff, int* len)
{
	int sockfd;
	struct sockaddr_in *vpn_ip = NULL;
	struct ifreq ifr;
	
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if(sockfd < 0)
	{
		printf("sock fail\n");
		return -1;
	}
	memset(&ifr, 0, sizeof(ifr));
	strcpy(ifr.ifr_name, eth_dev);

	if (ioctl(sockfd, SIOCGIFHWADDR, &ifr) == -1)
	{  
		printf("ioctl %s SIOCGIFHWADDR fail\n", eth_dev);
		return -1;
	} 

	unsigned char tmp_buf[32] =  { 0 };

	memcpy(tmp_buf,(u_char *)ifr.ifr_hwaddr.sa_data, MAC_ADDR_LEN);  
	//memcpy(buff,(u_char *)ifr.ifr_hwaddr.sa_data, MAC_ADDR_LEN);  
	close(sockfd);
	
	int ilen = *len;
	if(ilen < MAC_ADDR_LEN)
	{
		printf("MAC buff size if too small.\n");
		return -1;
	}
	memset(buff,'\0',ilen);

	int i = 0;
	for(i = 0; i < MAC_ADDR_LEN;i++)
	{
		sprintf( buff+2*i, "%02X",tmp_buf[i]);
	}


	*len = MAC_ADDR_LEN*2 +1;

	return  MAC_ADDR_LEN*2 + 1;
}



int main(int argc,char *argv[])
{
	unsigned char allMac[32] = {0};
	
	int mlen = 32;

	int ret = readMacAddr(argv[1],allMac,&mlen);

	printf("dev %s Mac is :%s\n",argv[1],allMac);
}
