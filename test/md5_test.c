#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "inc/md5/mite_md5.h"
//#include <openssl/md5.h>
#include "inc/openssl/md5.h"





int main(int argc,char **argv)
{
    char *tmp = (char *)"Hello World";
    unsigned char *md51 = (unsigned char *)malloc(MD5_DIGEST_LENGTH);
    unsigned char *md52 = (unsigned char *)malloc(MD5_DIGEST_LENGTH);
    unsigned char *md53 = (unsigned char *)malloc(MD5_DIGEST_LENGTH);
    unsigned char *md54 = (unsigned char *)malloc(MD5_DIGEST_LENGTH);

    mite_MD5_simpleCal(tmp,strlen(tmp),md51);
    mite_MD5_standarCal(tmp,strlen(tmp),md52);
    mite_MD5_evpCal(tmp,strlen(tmp),md53);
    mite_MD5_BIOCal(tmp,strlen(tmp),md54);

    printf("simple call.\n");
    printHex(md51,MD5_DIGEST_LENGTH);
    printf("stander call.\n");
    printHex(md52,MD5_DIGEST_LENGTH);
    printf("evp call.\n");
    printHex(md53,MD5_DIGEST_LENGTH);
    printf("bio call.\n");
    printHex(md53,MD5_DIGEST_LENGTH);

    free(md51);
    free(md52);
    free(md53);
    free(md54);

    if(argc == 2)
    {
        unsigned char *md5file = (unsigned char *)malloc(MD5_DIGEST_LENGTH*2);
        if(md5file == NULL)
        {
            perror("Malloc:");
            return 0;
        }
        int ret=0;
        printf("%s \n",argv[1]);
        ret = mite_CalFileMD5(argv[1],md5file);
        printf("ret = %d\n",ret);
        printf("MD5 = %s\n",md5file);
       // printHex(md5file,MD5_DIGEST_LENGTH);
        free(md5file);
    }

    return 0;
}
