#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "inc/base64/mite_base64.h"



int main(int argc,char *argv[])
{
	if(argc < 2)
	{
		printf("usage:%s string\n",argv[0]);
		return 0;
	}
    int len = strlen(argv[1]);
    char *base64=NULL;
    char *pure_data = NULL;
    int ret = mite_string_2_base64(argv[1],len,&base64);
    printf("\n============================\n");
    printf("[%d]  %s\n",ret,base64);
    printf("============================\n");

    ret = mite_base64_2_string(base64,ret,&pure_data);
    printf("[%d]  %s\n",ret,pure_data);
    printf("============================\n");

    free(base64);
    free(pure_data);

    ret = mite_string_2_base64_block(argv[1],len,&base64);
    printf("[%d]  %s\n",ret,base64);
    printf("============================\n");

    ret = mite_base64_2_string_block(base64,ret,&pure_data);
    printf("[%d]  %s\n",ret,pure_data);
    printf("============================\n");

    free(base64);
    free(pure_data);
}
