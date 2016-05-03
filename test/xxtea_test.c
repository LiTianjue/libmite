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

#include "inc/crypto/xxtea_crypto.h"

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
	//还没有写测试代码
	return 0;
}
