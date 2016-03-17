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


//获取文件大小
long mite_file_helper_GetFileSize(char *path);
//创建文件夹
int mite_file_helper_CreateDir(char *pDir);

//获取文件名或路径名
const char *mite_file_helper_GetFileName(char *path);
const char *mite_file_helper_GetDirName(char *path);

#endif // FILE_HELPER_H

