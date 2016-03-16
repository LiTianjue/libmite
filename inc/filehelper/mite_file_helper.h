#ifndef FILE_HELPER_H
#define FILE_HELPER_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

int mite_file_helper_rename(const char *src,const char *des);
int mite_file_helper_copy(const char *src,const char *des);
int mite_file_helper_move(const char *src,const char *des);


#endif // FILE_HELPER_H

