#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

//#include <openssl/evp.h>
#include "inc/base64/mite_base64.h"
#include "inc/openssl/evp.h"
//base64将数据每3个字节分为一组，在将一组数据按二进制分为4组
//即：3个字节变为4个字节
//不能对齐的部分补1个或两个等号
// openssl 的base64编码是带换行的！！，80个字节也会换行




/***********************
    Decode
************************/
int mite_string_2_base64(char *input,int input_len,char **base64)
{
    EVP_ENCODE_CTX ectx;

    EVP_EncodeInit(&ectx);

    int out_len =0;
    if(input_len%3==0)
        out_len = input_len/3 *4;
    else
        out_len = (input_len/3 +1)*4;

    //数据够长的话会有多个换行
    char *out = (char *)malloc(out_len + out_len/80 +1);
    int total = 0;
    int encode_len = 0;
    EVP_EncodeUpdate(&ectx,out,&encode_len,input,input_len);
    total += encode_len;
    EVP_EncodeFinal(&ectx,out+total,&encode_len);
    total += encode_len;

    printf("%s",out);
    *base64 = out;
    return total;
}

int mite_string_2_base64_block(char *input,int input_len,char **base64)
{
    int len = 0;
    char *out = NULL;
    out = malloc(((input_len/3+1)*4));
    len = EVP_EncodeBlock(out,input,input_len);

    *base64 = out;
    return len;
    /*
    char *p = out+len-1;
    int pad=0;
    int i = 0;
    for(i=0;i<4;i++)
    {
        if(*p=='=')
            pad++;
        p--;
    }
    */
}



/***********************
    Decode
************************/
int mite_base64_2_string(char *base64,int inlen,char **ppout)
{
    EVP_ENCODE_CTX dctx;
    EVP_DecodeInit(&dctx);
    int outlen = 0;
    int total = 0;
    int ret = 0;
    char *out = malloc(inlen);
    ret = EVP_DecodeUpdate(&dctx,out,&outlen,base64,inlen);
    if(ret < 0)
    {
        printf("EVP_DecodeUpdate err!\n");
        return -1;
    }
    total+=outlen;
    ret = EVP_DecodeFinal(&dctx,out,&outlen);
    total += outlen;
    *ppout=out;
    return total;
}

int mite_base64_2_string_block(char *base64,int inlen,char **ppout)
{
    char *p=base64+inlen -1;
    int pad = 0;
    int i = 0;
    for(i=0;i<4;i++)
    {
        if(*p=='=')
            pad++;
        p--;
    }
    char *out;
    out = malloc(inlen);
    int len = EVP_DecodeBlock(out,base64,inlen);
    len -= pad;

    *ppout = out;
    return len;
}


int mite_file_2_base64(char *file, char *base64)
{


}



int mite_base64_2_file(char *base,char *filename)
{

}
