#include "inc/filehelper/mite_file_helper.h"
#include <errno.h>


int main(int argc ,char* argv[])
{
    if(argc < 3)
    {
        printf("usage :\n");
        printf("move/copy/rename src des\n");
        printf("size file_name\n");
        printf("mkdir dir\n");
        printf("filename dir\n");
        printf("dirname dir\n");
        return 0;
    }
    if(strstr(argv[1],"copy"))
    {
        if(mite_file_helper_copy(argv[2],argv[3]) != 0)
        {
            perror("copy");
        }

    }else if(strstr(argv[1],"move"))
    {
        if(mite_file_helper_move(argv[2],argv[3]) != 0)
        {
            perror("move");
        }

    }else if(strstr(argv[1],"rename"))
    {
        if(mite_file_helper_rename(argv[2],argv[3]) != 0)
        {
            perror("rename");
        }

    }else if(strstr(argv[1],"size"))
    {
        long size = 0;
        size = mite_file_helper_GetFileSize(argv[2]);
        if(size <=0)
        {
            perror("Get file size:");
        }
        else
        {
            printf("File %s size[%d]\n",argv[2],size);
        }
    }else if(strstr(argv[1],"mkdir"))
    {
        if(mite_file_helper_CreateDir(argv[2])< 0)
        {
            perror("mkdir");
        }
    }else if(strstr(argv[1],"filename"))
    {
        printf("FileName:%s\n",mite_file_helper_GetFileName(argv[2]));

    }else if(strstr(argv[1],"dirname"))
    {
        printf("DirName:%s\n",mite_file_helper_GetDirName(argv[2]));
    }else
    {
        printf("unknow opt :%s\n",argv[1]);
    }

    return 0;
}


