#include "inc/filehelper/mite_file_helper.h"
#include <errno.h>


int main(int argc ,char* argv[])
{
    if(argc < 4)
    {
        printf("usage :\n");
        printf("move/copy/rename src des\n");
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

    }else
    {
        printf("unknow opt :%s\n",argv[1]);
    }

    return 0;
}


