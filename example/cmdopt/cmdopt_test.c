#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <getopt.h>

void print_help()
{
	printf("--usage:--\n");
    printf("-u username(root for default)\n");
    printf("-p password(admin for default)\n");
    printf("-s listen ip (127.0.0.1 for defult)\n");
    printf("-l listen port (22 for default) \n");
    printf("-d debug\n");
}


int main(int argc,char *argv[])
{
    int opt;
    char user[32] = "root";
    char  passwd[32] = "admin";
    char server[32] = "127.0.0.1";
    int listen_port = 22;
	//没有参数的不加冒号
	int debug = 0;

	
    while((opt = getopt(argc,argv,"u:p:s:l:d")) != -1)
    {
        switch(opt)
        {
            case 'u':
                strcpy(user,optarg);
                break;
            case 'p':
                strcpy(passwd,optarg);
                break;
            case 's':
                strcpy(server,optarg);
                break;
            case 'l':
                listen_port = atoi(optarg);
                break;
            case 'd':
				debug =1;
				break;
            case '?':
                print_help();
                return 1;
            default:
                break;
        }
    }

	printf("user:%s\n",user);
	printf("pwd:%s\n",passwd);
	printf("connect:%s:%d\n",server,listen_port);
	printf("Debug [%s]\n",debug?"true":"false");

    return 0;
}






