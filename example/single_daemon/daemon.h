#ifndef _DAEMON_H
#define _DAEMON_H

int daemonize_user(void);
int daemonize_sys(void);
int already_running(const char *pidfile);
void create_pidfile(const char *pidfile);
void delete_pidfile(const char *pidfile);

#endif // for #ifndef _DAEMON_H
