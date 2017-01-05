#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <fcntl.h>

#include "global.h"
#include "command.h"
#include "users.h"

extern userList connectedUser, unconnectedUser;

int maxFileDescriptor;
fd_set readSet;

static double calculateSpeed(struct timeval *before, struct timeval *after, int size);
static int calculateSleepTime(double speedNow, int speedLimit, \
		int lastSleepTime);
static void transferFile(int fromFileDescriptor, int toFileDescriptor, \
						user* user, int upOrDown);
static void connectToClient(user* user);

void handleCommand(user* currentUser, const char* buffer, ssize_t size)
{
	printf("handling user %s : %s\n", currentUser -> username, buffer);

	char request[REQUEST_BUFF] = {0}, parameter[BUFFER_SIZE] = {0};
	sscanf(buffer, "%4s %1023s", request, parameter);

	if(DEBUG)
		printf("%s|%s|\n", request, parameter);

	if( strcmp(request, "PASS") == 0)		// password
	{
		if( strcmp(parameter, currentUser -> password) == 0)
		{
			// if password is correct,
			// reply 230 login success
			// set environments for user
			// print user information on server's screen

			currentUser -> loginTime = time(NULL);
			strcpy(currentUser -> currentPath,	\
					currentUser -> homeDirectory);
			reply(currentUser -> controlSocket, LOGIN_SUCCESS,	\
				   	"login successful");
			printf("user %s logged in\n", currentUser -> username);

			struct sockaddr_in peerAddress;
			socklen_t addressLength;
			addressLength = sizeof(peerAddress);
			getpeername(currentUser -> controlSocket,	\
					(struct sockaddr*)&peerAddress,		\
					&addressLength);

			printf("IP address: %s:%d\n\n",	\
					inet_ntoa( peerAddress.sin_addr),	\
					(int) ntohs(peerAddress.sin_port));
		}
		else
		{
			// if password not correct
			// reply 530 login failed
			// close socket file descriptor
			// move user from connectedUser to unconnectedUser

			reply(currentUser -> controlSocket, LOGIN_FAILED,	\
					"login failed");
			removeSocket(currentUser -> controlSocket);
			currentUser -> controlSocket = 0;
			moveUser(&connectedUser, &unconnectedUser, currentUser);
		}
	}
	else if( strcmp(request, "SYST") == 0)	// system
	{
		reply(currentUser -> controlSocket, SYSTEM_TYPE, \
				"UNIX Type: L8");
	}
	else if( strcmp(request, "CWD") == 0)	// change working directory
	{
		chdir(currentUser -> currentPath);
		if( ( chdir(parameter) ) == 0)
		{
			char path[PATH_LENGTH];
			getcwd(path, sizeof(path));
			if( newPathAccessible(path, currentUser -> homeDirectory) &&
				currentUser -> readAccess)
			{
				strcpy(currentUser -> currentPath, path);
				reply(currentUser -> controlSocket, OPERATE_SUCCESS,	\
						"Change working directory success!");
			}
			else
			{
				reply(currentUser -> controlSocket,	OPERATE_FAILED, \
						"you have no permission");
			}
		}
		else
		{
			reply(currentUser -> controlSocket, OPERATE_FAILED,	\
					"Change working directory failure!");
		}
		
	}
	else if( strcmp(request, "PWD") == 0)	// print working directory
	{
		char message[BUFFER_SIZE];
		sprintf(message, "\"%s\"", currentUser -> currentPath);
		reply(currentUser -> controlSocket, MKD_SUCCESS, message);
	}
	else if( strcmp(request, "LIST") == 0 ||
				strcmp(request, "NLST") == 0)	// list files and directories
	{
		if(! currentUser -> readAccess)
		{
			reply(currentUser -> controlSocket,	OPERATE_FAILED, \
					"you have no permission");
			return;
		}

		// since sending file list needs blocking system call,
		// fork a child process first
		pid_t pid = fork();
		if(pid < 0)
		{
			fprintf(stderr, "fork failed\n");
			exit(-1);
		}
		else if(pid != 0)
		{
			// parent
			close(currentUser -> dataSocket);
			currentUser -> dataSocket = 0;
			return;
		}

		int fd;
		if(currentUser -> ftpMode == PASSIVE_MODE)
			fd = acceptNew(currentUser -> dataSocket);
		else
		{
			fd = currentUser -> dataSocket;
			connectToClient(currentUser);
		}
			
		reply(currentUser -> controlSocket, STARTING_DATA,	\
				"here comes listing");

		// get list and send
		DIR *dp;
		struct dirent *dirp;
		dp = opendir(currentUser -> currentPath);
		char buffer[BUFFER_SIZE] = {0};
		int n = 0;
		while( (dirp = readdir(dp)) != NULL)
		{
			sprintf(buffer, "%-25s", dirp -> d_name);

			if( n % 3 == 0 && strcmp(request, "LIST") == 0)	// 3 columns a line
			{
				strcat(buffer, "\r\n");
			}
			write(fd, buffer, strlen(buffer));
			n++;
		}
		sprintf(buffer, "\r\n");
		write(fd, buffer, strlen(buffer));

		close(fd);
		reply(currentUser -> controlSocket, CLOSING_DATA,	\
				"directory send OK");
		exit(0);
	}
	else if( strcmp(request, "RNFR") == 0)	// rename from
	{
		chdir(currentUser -> currentPath);		
		if( (access(parameter, F_OK)) == 0) // test if file exists
		{
			if(currentUser -> writeAccess)
			{
				strcpy(currentUser -> renameFile, parameter);
				reply(currentUser -> controlSocket, FURTHER_INFO,	\
						"ready for RNTO");
			}
			else
			{
				reply(currentUser -> controlSocket, OPERATE_FAILED, \
						"you have no permission");
			}
		}
		else
			reply(currentUser -> controlSocket, OPERATE_FAILED,	\
					"RNFR command failed");
	}
	else if( strcmp(request, "RNTO") == 0)	// rename to
	{
		chdir(currentUser -> currentPath);
		if( rename(currentUser -> renameFile, parameter) == 0)
		{
			reply(currentUser -> controlSocket, OPERATE_SUCCESS,	\
					"rename successful");
		}
		else
			reply(currentUser -> controlSocket, OPERATE_FAILED,		\
					"rename failed");
	}
	else if( strcmp(request, "PASV") == 0)	// passive mode
	{
		// start a new server, waiting for client
		struct sockaddr_in newServer;
		memset(&newServer, 0, sizeof(newServer));
		newServer.sin_family = AF_INET;
		// FIXME may have security issues
		newServer.sin_addr.s_addr = htonl(INADDR_ANY);
		newServer.sin_port = htons(UNUSED_PORT);

		int newServerfd = startServer(&newServer);

		struct sockaddr_in localAddressOfControlSocket;
		struct sockaddr_in localAddressOfNewServer;

		// get local ip and listen port,
		// convert to format (IP,IP,IP,IP,port,port)
		// and reply to client
		socklen_t addressLength;
		addressLength = sizeof(localAddressOfControlSocket);
		getsockname(currentUser -> controlSocket,	\
				(struct sockaddr*)&localAddressOfControlSocket,	\
						&addressLength);
		addressLength = sizeof(localAddressOfNewServer);
		getsockname(newServerfd,	\
				(struct sockaddr*)&localAddressOfNewServer,	\
					&addressLength);

		char message[BUFFER_SIZE] = {0};
		char addressBuffer[BUFFER_SIZE] = {0};
		int localPort = ntohs(localAddressOfNewServer.sin_port);
		sprintf(addressBuffer,"%s", inet_ntoa(	\
					localAddressOfControlSocket.sin_addr));
		int i;
		for(i = 0; i < strlen(addressBuffer); i++)
			if(addressBuffer[i] == '.')
				addressBuffer[i] = ',';

		sprintf(message, "entering passive mode (%s,%d,%d).",	\
				addressBuffer, localPort / 256, localPort % 256);
		reply(currentUser -> controlSocket, ENTER_PASSIVE, message);
		
		currentUser -> dataSocket = newServerfd;
		currentUser -> ftpMode = PASSIVE_MODE;

		if(DEBUG)
		{
			printf("local address: %s\n", \
					inet_ntoa( localAddressOfControlSocket.sin_addr) );
			printf("local port: %d\n", (int) ntohs(	\
						localAddressOfNewServer.sin_port));
		}
	}
	else if( strcmp(request, "PORT") == 0)		// active mode
	{
		int a,b,c,d,e,f;
		sscanf(parameter, "%d,%d,%d,%d,%d,%d", &a, &b, &c, &d, &e, &f);
		char ipString[BUFFER_SIZE];
		sprintf(ipString, "%d.%d.%d.%d", a, b, c, d);
		currentUser -> clientAddress.sin_family = AF_INET;
		currentUser -> clientAddress.sin_port = htons(e * 256 + f);
		inet_pton(AF_INET, ipString, &(currentUser -> clientAddress.sin_addr) );
		currentUser -> dataSocket = socket(AF_INET, SOCK_STREAM, 0);
		currentUser -> ftpMode = ACTIVE_MODE;
		reply(currentUser -> controlSocket, COMMAND_OK,		\
					"PORT command successful");
	}
	else if( strcmp(request, "TYPE") == 0)	// data transfer type
	{
		switch(parameter[0])
		{
			case 'I':
				currentUser -> transferMode = BINARY_MODE;
				reply(currentUser -> controlSocket, COMMAND_OK, \
						"changed to binary mode");
				break;
			case 'A':
				currentUser -> transferMode = ASCII_MODE;
				reply(currentUser -> controlSocket, COMMAND_OK, \
						"changed to ASCII mode");
				break;
			default:
				reply(currentUser -> controlSocket, SYNTAX_ERROR, \
						"unrecognised TYPE command");
		}		
	}
	else if( strcmp(request, "SIZE") == 0)	// file size
	{
		chdir(currentUser -> currentPath);
		struct stat fileStatus;
		if( stat(parameter, &fileStatus) == 0)
		{
			char message[BUFFER_SIZE];
			sprintf(message, "%d", (int)fileStatus.st_size);
			reply(currentUser -> controlSocket, FILE_STATUS, message);
		}
		else
			reply(currentUser -> controlSocket, OPERATE_FAILED,	\
					"could not get file size");
	}
	else if( strcmp(request, "RETR") == 0)	// retrieve file
	{
		if(! currentUser -> readAccess)
		{
			reply(currentUser -> controlSocket,	OPERATE_FAILED, \
					"you have no permission");
			return;
		}

		// fork a child process
		pid_t pid = fork();
		if(pid < 0)
		{
			fprintf(stderr, "fork failed\n");
			exit(-1);
		}
		else if(pid != 0)	// parent
		{
			close(currentUser -> dataSocket);
			currentUser -> dataSocket = 0;
			return;
		}
		
		int datafd;
		if(currentUser -> ftpMode == PASSIVE_MODE)
			datafd = acceptNew(currentUser -> dataSocket);
		else
		{
			datafd = currentUser -> dataSocket;
			connectToClient(currentUser);
		}

		// open and read file, save to buffer, send to client
		chdir(currentUser -> currentPath);	
		int filefd;
		if( (filefd = open(parameter, O_RDONLY)) < 0)
		{
			reply(currentUser -> controlSocket, OPERATE_FAILED,	\
					"failed to open file");
			exit(0);
		}
		else
			reply(currentUser -> controlSocket, STARTING_DATA,	\
					"starting transfer file");

		transferFile(filefd, datafd, currentUser, FILE_RETR);

		reply(currentUser -> controlSocket, CLOSING_DATA,	\
				"file transfer complete");
		exit(0);
	}
	else if( strcmp(request, "STOR") == 0)	// store file
	{
		if(! currentUser -> writeAccess)
		{
			reply(currentUser -> controlSocket,	OPERATE_FAILED, \
					"you have no permission");
			return;
		}

		// fork a child process
		pid_t pid = fork();
		if(pid < 0)
		{
			fprintf(stderr, "fork failed\n");
			exit(-1);
		}
		else if(pid != 0)	// parent
		{
			close(currentUser -> dataSocket);
			currentUser -> dataSocket = 0;
			return;
		}

		int datafd;
		if(currentUser -> ftpMode == PASSIVE_MODE)   
			datafd = acceptNew(currentUser -> dataSocket);
		else
		{
			datafd = currentUser -> dataSocket;
			connectToClient(currentUser);
		}

		// open file, receive data, save to buffer, write to file
		chdir(currentUser -> currentPath);	
		int filefd;
		if( (filefd = open(parameter, O_WRONLY | O_CREAT | O_TRUNC, \
						FILE_MASK)) < 0)
		{
			reply(currentUser -> controlSocket, OPERATE_FAILED,	\
					"failed to open file");
			exit(0);
		}
		else
			reply(currentUser -> controlSocket, STARTING_DATA,	\
					"OK to receive data");
		
		transferFile(datafd, filefd, currentUser, FILE_STOR);
			
		reply(currentUser -> controlSocket, CLOSING_DATA,	\
				"file transfer complete");

		exit(0);
	}
	else if( strcmp(request, "DELE") == 0 ||	\
			 strcmp(request, "RMD") == 0)	// delete file or directory
	{
		if(! currentUser -> writeAccess)
		{
			reply(currentUser -> controlSocket,	OPERATE_FAILED, \
					"you have no permission");
			return;
		}

		// since we can use a same system call remove() to 
		// delete file and delete directory,
		// we handle DELE and RMD together
		
		chdir(currentUser -> currentPath);

		if((access(parameter, F_OK)) == 0)
		{ 
			if(remove(parameter) == 0)
				reply(currentUser -> controlSocket, OPERATE_SUCCESS, \
						"delete success!");
			else
				reply(currentUser -> controlSocket, OPERATE_FAILED,	\
						"delete failure!");
		}
		else
			reply(currentUser -> controlSocket, OPERATE_FAILED,	\
					"No such file or directory!");
	}
	else if( strcmp(request, "MKD") == 0)	// make directory
	{
		chdir(currentUser -> currentPath);

		if(mkdir(parameter,DIR_MASK) == 0) 
			reply(currentUser -> controlSocket, MKD_SUCCESS,	\
					"mkdir success");
		else
			reply(currentUser -> controlSocket, OPERATE_FAILED,	\
					"mkdir failure");
	}
	else if( strcmp(request, "QUIT") == 0)	// client logout
	{
		// reply 221 goodbye to user
		// close control socket of user
		// clean user information data structure
		// move user from connectedUser to unconnectedUser
		reply(currentUser -> controlSocket, SERVIVE_CLOSE, "goodbye");
		memset(currentUser -> currentPath, 0, PATH_LENGTH);
		removeSocket(currentUser -> controlSocket);
		currentUser -> controlSocket = 0;
		moveUser(&connectedUser, &unconnectedUser, currentUser);
		printf("user %s logged out\n", currentUser -> username);
	}
	else	// if client sends something else
	{
		reply(currentUser -> controlSocket, NOT_IMPLEMENT, \
				"command not implemented");
	}

	return;
}

// send message with replyCode (defined in command.h) using a socket
void reply(int socketFileDescriptor, int replyCode, const char* message)
{
	char buffer[BUFFER_SIZE];
	sprintf(buffer, "%d %s\r\n", replyCode, message);
	write(socketFileDescriptor, buffer, strlen(buffer));
}

// start a listen socket using "address"
// return socket file descriptor
int startServer(struct sockaddr_in* address)
{
	int socketfd = socket(AF_INET, SOCK_STREAM, 0);
	if(socketfd < 0)
	{
		fprintf(stderr, "error creating socket\n");
		exit(-1);
	}

	if( bind(socketfd, (struct sockaddr *) address, \
				sizeof(struct sockaddr_in)) < 0 )
	{
		fprintf(stderr, "can't bind to port %d\n", LISTEN_PORT);
		exit(-1);
	}

	if( listen(socketfd, LISTEN_QUEUE) < 0)
	{
		fprintf(stderr, "error converting to passive socket\n");
		exit(-1);
	}
	return socketfd;
}

// close socket and clear corresponding flag in readSet
void removeSocket(int fileDescriptor)
{
	close(fileDescriptor);
	FD_CLR(fileDescriptor, &readSet);

	if(fileDescriptor == maxFileDescriptor)
		maxFileDescriptor--;
}

// accept new connection and return socket file descriptor
int acceptNew(int listenSocketFileDescriptor)
{
	struct sockaddr dataSocketAddress;
	socklen_t len = sizeof(dataSocketAddress);
	int fd = accept( listenSocketFileDescriptor, \
			(struct sockaddr *) &dataSocketAddress, \
			&len);
	return fd;
}

void connectToClient(user* currentUser)
{
	if( connect(currentUser -> dataSocket,	\
				(struct sockaddr *) &(currentUser -> clientAddress), \
				sizeof(currentUser -> clientAddress)) < 0)
	{
		reply(currentUser -> controlSocket, DATA_FAILED,	\
				"can't open data connection");
		exit(0);
	}
}

double calculateSpeed(struct timeval *before, struct timeval *after, int size)
{
	int timeSpan = (after -> tv_sec - before -> tv_sec) * 1000000 + \
				   (after -> tv_usec - before -> tv_usec);
	return 1000000.0 / 1024.0 * size / timeSpan;	// remember in kB/s
}

int calculateSleepTime(double speedNow, int speedLimit, int lastSleepTime)
{
	if( (int)speedNow < speedLimit)
	{
		return lastSleepTime / 2;
	}

	if( (int)speedNow == speedLimit)
	{
		return lastSleepTime;
	}

	double sleepTime = 1000.0 + 1.5 * lastSleepTime;
	return (int)sleepTime;
}

void transferFile(int fromfd, int tofd, user* user, int upOrDown)
{
	int size;
	char buffer[BUFFER_SIZE];
	int sleepTime = 0;		// in microseconds (10e-6 second)
	double speedNow = 0.0;	// in kB/s
	int fileTransferred = 0;// in bytes
	struct timeval timeBeforeTransmission, timeAfterWrite;

	gettimeofday(&timeBeforeTransmission,NULL);

	FILE* input;	
	if(user -> transferMode == ASCII_MODE)
	{
		input = fdopen(fromfd, "r");
	}		

	while(1)
	{
		if(user -> transferMode == BINARY_MODE)
		{
			size = read(fromfd, buffer, BUFFER_SIZE);
			if(size == 0)
				break;
			if(size < 0)
				{
					fprintf(stderr, "read error\n");
					exit(-1);
				}
		}
		else if(user -> transferMode == ASCII_MODE)
		{
			fgets(buffer, BUFFER_SIZE - 3, input);
			
			if( feof(input))
				break;

			if( ferror(input))
			{
				fprintf(stderr, "read error\n");
				exit(-1);
			}

			int len = strlen(buffer);
			if( buffer[len - 2] == '\r' && buffer[len - 1] == '\n' && 
					upOrDown == FILE_STOR)
			{
				buffer[len - 2] = '\n';
				buffer[len - 1] = 0;
				size = len - 1;
			}
			else if( buffer[len - 1] == '\n' && upOrDown == FILE_RETR)
			{
				buffer[len - 1] = '\r';
				buffer[len] = '\n';
				buffer[len + 1] = 0;
				size = len + 1;
			}
		}
		else
		{
			reply(user -> controlSocket, DATA_ABORT,	\
					"transfer mode not supported");
			exit(0);
		}

		int writeSize = write(tofd, buffer, size);
		if(writeSize != size)
		{
			fprintf(stderr, "write error");
			exit(-1);
		}

		fileTransferred += size;
		user -> dataTransferred += size;			

		if(user -> speedLimit)
		{
			gettimeofday(&timeAfterWrite, NULL);	
			speedNow = calculateSpeed(&timeBeforeTransmission, \
					&timeAfterWrite, fileTransferred);
			sleepTime = calculateSleepTime(speedNow, user -> speedLimit, \
						sleepTime);
			usleep(sleepTime);
		}

		if(DEBUG)
		{
			gettimeofday(&timeAfterWrite, NULL);
			speedNow = calculateSpeed(&timeBeforeTransmission, \
					&timeAfterWrite, fileTransferred);
			printf("current speed: %lf\n", speedNow);
		}

	}	
}

int newPathAccessible(char* newPath, char* homeDirectory)
{
	int newPathLength = strlen(newPath);
	int homeDirectoryLength = strlen(homeDirectory);
	if(newPathLength < homeDirectoryLength)
		return 0;

	if( strncmp(newPath, homeDirectory, homeDirectoryLength) == 0)
		return 1;
	else
		return 0;
}
