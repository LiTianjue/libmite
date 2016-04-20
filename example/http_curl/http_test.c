#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <error.h>
#include <curl/curl.h>

#define MAX_FILE_SIZE 2048
#define MAX_REQUEST_SIZE    1024*8

static const char *OS = "os=x86";
static const char *VERSION="version=2.4.0";

static char responseData[MAX_REQUEST_SIZE] = {0};
static int responseLen = 0;

int sendUrl(char *url,char *post_data,void *save);
int checkVersion(char *ip,int port);
int checkUser(char *ip,int port,char *post_data);
char *Test_Encode(char *source,int len);

//获得回应的回调函数，一般传入的参数是文件描述符，用于写入数据
//需要注意的是一次请求可能会由于网络的原因分多次返回
int getRequest(void *buffer,size_t size,size_t nmemb,void *ptr)
{
    //如果是写入文件或者写到其他地方，就不用管Buffer不够的问题
    // write
    if(ptr != NULL)
    {
        FILE *fp = (FILE *)ptr;
        fwrite(buffer,size,nmemb,fp);
    }

    //这里如果返回的信息大于我们分配的内存，就丢弃数据，实际上应该重新分配内存
    if(responseLen + size*nmemb > MAX_REQUEST_SIZE)
    {
        responseLen = 0;
    }

    //memset(responseData,'\0',MAX_REQUEST_SIZE);
    memcpy(responseData+responseLen,buffer,size*nmemb);
    responseLen += size*nmemb;

    printf("%s\n",buffer);

    return size*nmemb;
}


int main(int argc,char *argv[])
{
    if(argc < 2)
    {
        printf("Usage: %s url  saveFile \n",argv[0]);
        return 0;
    }
    FILE *fp = NULL;

    if(argc == 3)
    {
        fp = fopen(argv[2],"wb");
        if(fp == NULL)
        {
            perror("fopen");
            return 0;
        }
    }

    int http_ret = 0;
	//注意如果url是从命令行传进来的要注意加""号，因为有&符号
    http_ret = sendUrl(argv[1],NULL,fp);

    if(http_ret != 200)
    {
        printf("请求失败 [%d]\n",http_ret);
    }
    else
    {
        printf("获得数据[%d] %s\n",responseLen,responseData);
    }
    return 0;
}


/*
int checkVersion(char *ip,int port)
{
    //http://192.168.1.120/UpgradeVersionAction_check.action?os=x86&version=2.4.0
    char url[1024] = {0};
    char post_data[2048] = {0};
    char *action = "/UpgradeVersionAction_check.action?";

    FILE *fp = NULL;
    fp = fopen("check_verion.json","w+");
    if(fp== NULL)
    {
        perror("fopen:");
        return -1;
    }

    sprintf(url,"http://%s:%d%s",ip,port,action);
    sprintf(post_data,"%s&%s",OS,VERSION);

    int rtCode = 0;
 #if 0
    {
        FILE *f = NULL;
        f = fopen("http_check.txt","w+");
        fwrite(url,1,strlen(url),f);
        fwrite('?',1,strlen(url),f);
        fwrite(post_data,1,strlen(url),f);
    }
 #endif
	printf("url:%s\n",url);
	printf("post:%s\n",post_data);
    rtCode = sendUrl(url,post_data,fp);
    printf("%s [%d]\n",url,rtCode);

    fclose(fp);

    //handle json
    return rtCode;
}
*/
// http 请求发送
int sendUrl(char *url,char *post_data,void *save)
{
    CURL *curl;
    CURLcode res;
    int resp_code = 0;

    curl = curl_easy_init();
    if(NULL != curl)
    {
           curl_easy_setopt(curl,CURLOPT_URL,url);
           curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION,getRequest);
           curl_easy_setopt(curl,CURLOPT_WRITEDATA,save);
           if (post_data != NULL)
           {
                curl_easy_setopt(curl,CURLOPT_POSTFIELDS,post_data);
                curl_easy_setopt(curl,CURLOPT_POST,1);
            }

           res = curl_easy_perform(curl);
    }else
        return -1;

    if(res == CURLE_OK)
    {
        res= curl_easy_getinfo(curl,CURLINFO_RESPONSE_CODE,&resp_code);
        if(res == CURLE_OK)
        {
        }
    }
    else
    {
        fprintf(stderr,"Error :%s\n",curl_easy_strerror(res));
        resp_code = 0;
    }

    curl_easy_cleanup(curl);
    return resp_code;
}


//need to free
// 如果数据中含有base64编码的文件，要用这个函数处理‘=’号
char *Test_Encode(char *source,int len)
{
    CURL *curl = curl_easy_init();
    char *cres = curl_easy_escape(curl,source,len);
    //string res(cres);
    //curl_free(cres);
    curl_easy_cleanup(curl);
    return cres;
}










