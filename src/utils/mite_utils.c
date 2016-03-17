#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "inc/utils/mite_utils.h"


//16进制打印字串
void mite_utils_printHex(const uint8_t *pbtData,const size_t szBytes)
{
	size_t szPos;
	for(szPos = 0;szPos < szBytes;szPos++) {
		printf("%02x ",pbtData[szPos]);
	}
	printf("\n");
}


//计算 LRC
uint8_t mite_utils_chekcLRC(uint8_t *data,int len)
{
	int i = 0;
    uint8_t ret;
	for(i= 0;i < len;i++)
	{

        ret ^=  (uint8_t)(*(data+i));
	}
	return ret;
}
