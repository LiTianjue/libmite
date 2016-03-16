#include "inc/filehelper/mite_file_helper.h"
#include <sys/stat.h>
 #include <libgen.h>

#define BUFFER_SIZE 2048

int mite_file_helper_rename(const char *src, const char *des)
{
    int ret = 0;
    ret = rename(src,des);

    return ret;
}
int mite_file_helper_move(const char *src, const char *des)
{
    int ret = 0;
    ret = mite_file_helper_rename(src,des);
    if(ret == 0)
    {
        return ret;
    }
    ret = mite_file_helper_copy(src,des);
    if(ret == 0)
    {
        unlink(src);
    }
    return ret;
}



int mite_file_helper_copy(const char *src, const char *des)
{
   int from_fd,to_fd;
   int bytes_read,bytes_write;
   char buffer[BUFFER_SIZE];
   char *ptr;

   if((from_fd=open(src,O_RDONLY))==-1)
   {
     return -1;
   }

   /* 创建目的文件 */
   /* 使用了O_CREAT选项-创建文件,open()函数需要第3个参数,
      mode=S_IRUSR|S_IWUSR表示S_IRUSR 用户可以读 S_IWUSR 用户可以写*/
   if((to_fd=open(des,O_WRONLY|O_CREAT,S_IRUSR|S_IWUSR))==-1)
   {
     close(from_fd);
     return -1;
   }

   int ret =0;
   while(bytes_read=read(from_fd,buffer,BUFFER_SIZE))
   {
     /* 一个致命的错误发生了 */
     if((bytes_read==-1)&&(errno!=EINTR))
     {
         ret = -1;
         break;
     }
     else if(bytes_read>0)
     {
       ptr=buffer;
       while(bytes_write=write(to_fd,ptr,bytes_read))
       {
         /* 一个致命错误发生了 */
         if((bytes_write==-1)&&(errno!=EINTR))
         {
             ret = -1;
             break;
         }
         /* 写完了所有读的字节 */
         else if(bytes_write==bytes_read)
             break;
         /* 只写了一部分,继续写 */
         else if(bytes_write>0)
         {
           ptr+=bytes_write;
           bytes_read-=bytes_write;
         }
       }
       /* 写的时候发生的致命错误 */
       if(bytes_write==-1)
       {
           ret = -1;
           break;
       }
     }
   }
   close(from_fd);
   close(to_fd);
   return ret;
}



/********************************************/
/*------------------------------
 *  功能： 获得一个文件的大小,单位 byte
 *  说明 ： 传入的字符串为文件的绝对路径
 *			不能获得文件夹的大小
 *-----------------------------*/
long mite_file_helper_GetFileSize(char *path)
{
    struct stat buf;
    int iRet = stat(path,&buf);
    if(iRet == -1)
        return 0;
    return buf.st_size;
}

/**************************************/

/*------------------------------
 *  功能： 创建文件夹
 *  说明 : 需要是'/'结束的字串，不然最后的字串会被当成文件
 *-----------------------------*/
int mite_file_helper_CreateDir(char *pDir)
{
    int i = 0;
    int iRet = 0;
    int iLen;
    char* pszDir;
    char *file_path = NULL;

    if(NULL == pDir)
    {
        return 0;
    }
    pszDir = strdup(pDir);
    iLen = strlen(pszDir);

    for(i = 0;;i++)
    {
        if(pszDir[i] != '\0')
        {
            if(pszDir[i] == '/')
            {
                file_path = pszDir + i;
            }
        }
        else{
            *file_path = '\0';
            break;
        }
    }

    for (i = 0;i < iLen;i ++)
    {
        if ((pszDir[i] == '/') && (i > 0))
        {
            pszDir[i] = '\0';
            if(access(pszDir,F_OK) < 0)
            {
                if(mkdir(pszDir,0755) < 0)
                {
                    //perror("mkdir");
                    return -1;
                }
            }
            pszDir[i] = '/';
        }
    }

    iRet = mkdir(pszDir,0755);
    free(pszDir);
    return iRet;
}


/*------------------------------
 *  功能： 从路径中提取文件名
 *  描述： 有bug
 *-----------------------------*/
const char *mite_file_helper_GetFileName(char *path)
{
    char m_path[128] = {0};
    strcpy(m_path,path);
    //printf("-- %s --\n",m_path);
    return basename(m_path);
}

/*------------------------------
 *  功能： 去掉文件名获得路径名不带结尾 '/'
 *  描述： 有bug
 *-----------------------------*/
const char *mite_file_helper_GetDirName(char *path)
{
    char m_path[128] = {0};
    strcpy(m_path,path);
    //printf("-- %s --\n",m_path);
    return dirname(m_path);
}






