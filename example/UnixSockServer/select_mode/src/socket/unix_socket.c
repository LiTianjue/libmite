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

//char *server_path="/tmp/unix_server_listener";

int create_unixsocket_listener(char *server_path)
{
	if(!server_path)
		return -1;
	
	int unix_server;
	unix_server = socket(PF_UNIX,SOCK_STREAM,0);
	if(unix_server == -1)
	{
		perror("Create unix socket error.\n");
		return -1;
	}

	unlink(server_path);

	struct sockaddr_un address = {
		.sun_family = AF_UNIX,
	};

	strcpy(address.sun_path,server_path);
	//address.sun_len = SUN_LEN(&address);
	
	int err = bind(unix_server,(const struct sockaddr *)&address,sizeof(address));

	if(err !=0 )
	{
		perror("unix bind error");
		return -1;
	}

	err = listen(unix_server,5);
	if(err !=0)
	{
		perror("unix listen error");
		return -1;
	}

	printf("set unix listen ok.\n");

	return unix_server;

}


int unixsocket_client_connect(char *server_path)
{
	if(!server_path)
		return -1;
	int unix_client = 0;
	unix_client = socket(AF_UNIX,SOCK_STREAM,0);
	
	struct sockaddr_un address;
	address.sun_family = AF_UNIX;
	strcpy(address.sun_path,server_path);

	/*
	struct sockaddr_un cli_addr;
	cli_addr.sun_family = AF_UNIX;
	strcpy(cli_addr.sun_apth)
	*/

	int ret = connect(unix_client,(struct sockaddr *)&address,sizeof(address));
	if(ret == -1)
	{
		perror("unix socket connect Error");
		close(unix_client);
		return -1;
	}

	return unix_client;
}
