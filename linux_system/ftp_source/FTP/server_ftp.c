#include <server_ftp.h>
#include <sys/select.h>
#include <pthread.h>

#define MAXCLIENT	100


typedef struct {
	int login;
	char username[32];
	char address[64];
} client_info;

client_info client_list[MAXCLIENT];

int add_client(char *username,char *ip,int port){
	int i;
	for(i = 0; i < MAXCLIENT;i++) {
		if(client_list[i].login == 0){
			strcpy(client_list[i].username,username);
			sprintf(client_list[i].address,"%s:%d",ip,port);
			client_list[i].login = 1;
			printf("添加用户 %d 成功\n",i);
			return 0;
		}
	}
	return -1;
}

int list_client()
{
	printf("遍历客户端\n");
	int i;
	for(i = 0; i < MAXCLIENT;i++) {
		if(client_list[i].login != 0){
			printf(" %s --- %s\n",client_list[i].username,client_list[i].address);
		}
	}

	return 0;
}



size_t size_sockaddr = sizeof(struct sockaddr), size_packet = sizeof(struct packet);


//服务客户端的线程
void* serve_client(void*);

int main(int argc,char *argv[])
{
	if(argc < 2 || argc > 3)
	{
		printf("ftpserver [IP地址] port启动FTP服务。\n");
		return -1;
	}
	char ip[32] = {0};
	int port = 1221;

	if(argc == 2){
		strcpy(ip,"0.0.0.0");
		port=atoi(argv[1]);
	}else
	{	
		strcpy(ip,argv[1]);
		port  = atoi(argv[2]);
	
	}
	
	printf("listen on %s:%d\n",ip,port);
	
	//初始化客户列表
	int i = 0;
	for(i = 0; i < MAXCLIENT;i++) {
		client_list[i].login = 0;
	}

	//BEGIN: initialization
	struct sockaddr_in sin_server, sin_client;
	int sfd_server, sfd_client, x;
	short int connection_id = 0;
	
	if((x = sfd_server = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
		er("socket()", x);
	
	memset((char*) &sin_server, 0, sizeof(struct sockaddr_in));
	sin_server.sin_family = AF_INET;
	//sin_server.sin_port = htons(PORTSERVER);
	//sin_server.sin_addr.s_addr = htonl(INADDR_ANY);
	
	sin_server.sin_port = htons(port);
	sin_server.sin_addr.s_addr = inet_addr(ip);

	

	if((x = bind(sfd_server, (struct sockaddr*) &sin_server, size_sockaddr)) < 0)
		er("bind()", x);
	
	if((x = listen(sfd_server, 1)) < 0)
		er("listen()", x);
	
	printf(ID "FTP Server started up @ local:%d. Waiting for client(s)...\n\n", PORTSERVER);
	//END: initialization


	//
	fd_set readSet;
	int user_input = 0 ;	//标准输入
	FD_ZERO(&readSet);
	FD_SET(sfd_server, &readSet);
	//FD_SET(user_input, &readSet);
	
	
	while(1)
	{
		int readyNumber = select(sfd_server+1,&readSet,NULL,NULL,NULL);
		if(readyNumber < 0)
		{
			//error
			exit(-1);
		}

		/*
		if( FD_ISSET(user_input, &readSet))
		{
			fprintf(stderr,"USER input :\n");
			char tmpbuf[128] = {0};
			char cmd[32] = {0};
			char arg[32] = {0};
			int n;
			//fgets(tmpbuf,1024,stdin);
			read(user_input,tmpbuf,128);
			sscanf(tmpbuf,"%s %s",cmd,arg);
			printf("get user input %s - %s \n",cmd,arg);
			if(strstr(cmd,"list")){
				printf("当前在线用户:\n");
				//列出当前在线用户
				list_client();
				printf("\n");
			}
			if(strstr(cmd,"kill")) {
				printf("kill user %s\n",arg);
				//kill_user(arg);
				//删除指定用户
			}

			readyNumber--;
			if(readyNumber <= 0)
				continue;
		}
		*/

		if(FD_ISSET(sfd_server,&readSet)){

			if((x = sfd_client = accept(sfd_server, (struct sockaddr*) &sin_client, &size_sockaddr)) < 0)
				er("accept()", x);
			printf(ID "Communication started with %s:%d\n", inet_ntoa(sin_client.sin_addr), ntohs(sin_client.sin_port));
			fflush(stdout);

			//先读取用户名密码
			{
				char buff[64];
				x = read(sfd_client,buff,1024);
				fprintf(stderr,"---------> Login %s\n",buff);

				//add_client(buff,inet_ntoa(sin_client.sin_addr),ntohs(sin_client.sin_port));
			}
			struct client_info* ci = client_info_alloc(sfd_client, connection_id++);

			pthread_t tid;
			if(pthread_create(&tid,NULL,serve_client,(void *)ci)){
				printf("create client thread ERROR.\n");
			}
			readyNumber--;
			if(readyNumber <= 0)
				continue;
		}
		
		//serve_client(ci);
	}
	
	close(sfd_server);
	printf(ID "Done.\n");
	fflush(stdout);
	
	return 0;
}

void* serve_client(void* info)
{
	int sfd_client, connection_id, x;
	struct packet* data = (struct packet*) malloc(size_packet);
	struct packet* shp;
	char lpwd[LENBUFFER];
	struct client_info* ci = (struct client_info*) info;
	sfd_client = ci->sfd;
	connection_id = ci->cid;
	
	while(1)
	{
		if((x = recv(sfd_client, data, size_packet, 0)) == 0)
		{
			fprintf(stderr, "client closed/terminated. closing connection.\n");
			break;
		}
		
		shp = ntohp(data);
		
		if(shp->type == TERM)
			break;
		
		if(shp->conid == -1)
			shp->conid = connection_id;
		
		if(shp->type == REQU)
		{
			switch(shp->comid)
			{
				case PWD:
					if(!getcwd(lpwd, sizeof lpwd))
						er("getcwd()", 0);
					command_pwd(shp, data, sfd_client, lpwd);
					break;
				case CD:
					if((x = chdir(shp->buffer)) == -1)
						fprintf(stderr, "Wrong path.\n");
					command_cd(shp, data, sfd_client, x == -1 ? "fail" : "success");
					break;
				case MKDIR:
					command_mkdir(shp, data, sfd_client);
					break;
				case LS:
					if(!getcwd(lpwd, sizeof lpwd))
						er("getcwd()", 0);
					command_ls(shp, data, sfd_client, lpwd);
					break;
				case GET:
					command_get(shp, data, sfd_client);
					break;
				case PUT:
					command_put(shp, data, sfd_client);
					break;
				case RGET:
					if(!getcwd(lpwd, sizeof lpwd))
						er("getcwd()", 0);
					command_rget(shp, data, sfd_client);
					send_EOT(shp, data, sfd_client);
					if((x = chdir(lpwd)) == -1)
						fprintf(stderr, "Wrong path.\n");
				default:
					// print error
					break;
			}
			/*
			//send info and then proceed to complete the request
			shp->type = INFO;
			strcpy(path, shp->buffer);
			sprintf(shp->buffer, "File found. Processing...");
			data = htonp(shp);
			if((x = send(sfd_client, data, size_packet, 0)) != size_packet)
				er("send()", x);
			
			send_file(path, sfd_client, shp);
			send_TERM(sfd_client, shp);
			*/
		}
		else
		{
			//show error, send TERM and break
			fprintf(stderr, "packet incomprihensible. closing connection.\n");
			send_TERM(shp, data, sfd_client);
			break;
		}
	}

	close(sfd_client);
	fflush(stdout);
}

