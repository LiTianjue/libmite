#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "daemon.h"

#define PID_FILE	"/tmp/test_pidfile"

int 
main(int argc, char *argv[]){
	
#if 1
	/*daemon test*/
	//if(daemonize_sys()){
	if(daemonize_user()){
		fprintf(stderr,"fail in daemon\n");
		return 0;
			
	}
	printf("this message can not be print\n");
	fprintf(stderr,"daemonize_user print this message\n");
	

#endif 

#if 1
	/*单实例进程测试*/
	if(already_running(PID_FILE)) {
		fprintf(stderr,"already Running !!!\n");
		return -1;
	}
	fprintf(stderr,"not process running.\n");
	//getchar();
	while(1)
	{
		sleep(1);
	}


#endif 

}
