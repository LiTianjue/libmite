#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>  
#include <sys/socket.h>  
#include <sys/un.h>  
#include <unistd.h>  


#include "socket/unix_socket.h"

char *server_path = "/tmp/unix_server_listener";
int main()
{
	int client = unixsocket_client_connect(server_path);
	if(client < 0)
		return -1;


	char *buff = "hello world";
	send(client,buff,strlen(buff),0);



}
