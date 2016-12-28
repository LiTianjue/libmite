#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/file.h>
#include <sys/stat.h>

#define LOCKMODE (S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH)

//文件锁
static int 
lockfile(int fd) {
	struct flock f1;

	//F_RDLCK:只读共享锁
	//F_WRLOK:独占锁
	//F_UNLCK:解除锁
	f1.l_type = F_WRLCK;

	f1.l_start = 0;
	
	//文件位置
	//SEEK_SET:开始位置
	//SEEK_CUR:当前位置
	//SEEK_END:文件尾
	f1.l_whence = SEEK_SET;

	//锁长度
	f1.l_len = 0;

	//F_GETLK 根据lock参数值，决定是否上文件锁
	//F_SETLK 设置lock参数值的文件锁
	//F_SETLKW 这是 F_GETLK的阻塞版本，在无法获取锁时，会进入睡眠状态。
	return (fcntl(fd,F_SETLK,&f1));
}

int
already_running(const char *pidfile) {
	int fd;
	char buf[16];
	
	fd = open(pidfile,O_RDWR | O_CREAT,LOCKMODE);
	if(fd < 0)
	{
		fprintf(stderr,"open \"%s\" failed (%d:%s)\n",pidfile,errno,strerror(errno));
		return -1;
	}
	if(lockfile(fd)<0) {
		if(errno == EACCES || errno == EAGAIN) {
			close(fd);
			return 1;
		}
		fprintf(stderr,"Can not lock %s :%s \n",pidfile,strerror(errno));

		return -1;
	}
	
	/*
	 * create pid file 
	 */
	if(ftruncate(fd,0)){
		fprintf(stderr,"Can not truncate %s :%s \n",pidfile,strerror(errno));
		return -1;
	}
	sprintf(buf,"%ld\n",(long)getpid());

	if(write(fd,buf,strlen(buf)+1) == -1) {
		fprintf(stderr,"Can not write %s :%s \n",pidfile,strerror(errno));
		return -1;
	}

	return 0;
}

void 
create_pidfile(const char *pidfile) {
	FILE *fp = fopen(pidfile,"w");
	if(fp) {
		fprintf(fp,"%ld\n",(long)getpid());
		fclose(fp);
	}
}

void
delete_pidfile(const char *pidfile) {
	unlink(pidfile);
}

int
daemonize_user(void) {
	int fd;
	pid_t pid;

	switch (pid = fork()) {
	case -1:
		fprintf(stderr,"fork() failed.\n");
		return -1;

	case 0:
		break;

	default:
		exit(0);	//father exit
	}

	setsid();
	if((fd = open("/dev/null",O_RDWR,0)) != -1) {
		dup2(fd,STDIN_FILENO);
		dup2(fd,STDOUT_FILENO);
		/*dup2(fd,STDERR_FILENO);*/

	}

	return 0;
}


int
daemonize_sys(void) {
	return daemon(0,0);
}

