#include <stdio.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <sys/time.h>
#include <string.h>


#include <sys/ioctl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <error.h>
#include <errno.h>

#include "inc/crypto/sm4.h"

//16进制打印字串
static void printHex(const uint8_t *pbtData,const size_t szBytes)
{
    size_t szPos;
    for(szPos = 0;szPos < szBytes;szPos++) {
        printf("%02x",pbtData[szPos]);
    }
    printf("\n");
}


//加密密钥
static unsigned char key[16] = {0x01,0x02,0x03,0x04,0x06,0x06,0x07,0x08,0x09,0x10,0x11,0x12,0x13,0x14,0x15,0x16};

int main(int argc,char *argv[])
{
    if(argc < 2)
    {
        printf("usage:\n");
        printf("%s string.\n",argv[0]);
        return 0;
    }
    char *encdata = NULL;
    int encLen = (strlen(argv[1])/16+1)*16;
    encdata = malloc(encLen);

	sm4_context ctx;
	sm4_setkey_enc(&ctx,key);
    sm4_crypt_ecb(&ctx,1,strlen((const char *)argv[1]),argv[1],encdata);
    printf("Enc Data[%d] :",encLen);
    printHex(encdata,encLen);

    char *decdata = malloc(encLen);
   sm4_setkey_dec(&ctx,key);
   sm4_crypt_ecb(&ctx,0,encLen,encdata,decdata);

   printf("Dec Data:%s\n",decdata);


   free(encdata);
   free(decdata);
}
