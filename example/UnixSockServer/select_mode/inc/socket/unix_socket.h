#ifndef UNIX_SOCKET_H
#define UNIX_SOCKET_H

int create_unixsocket_listener(char *server_path);
int unixsocket_client_connect(char *server_path);

#endif /* UNIX_SOCKET_H */
