#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <openssl/md5.h>

#include <openssl/evp.h>

#include <openssl/bio.h>

void printHex(unsigned char *hex,int length)
{
	int i = 0;
	char *tmp = malloc(length*2);
	printf("length = %d\n",length);

	for(i = 0;i < length;i++)
	{
	//	printf("%02X",hex[i]);
		sprintf(tmp+i*2,"%02X",hex[i]);
	}
	printf("%s\n",tmp);
	printf("\n");
	free(tmp);
}

void getmd5str(unsigned char *md5,char *hex,int length)
{
	if(md5==NULL || hex==NULL)
		return;
	int i = 0;
	for(i = 0; i < length;i++)
	{
		sprintf(hex+i*2,"%02x",md5[i]);
	}
}

//标准调用方法
void mite_MD5_standarCal(char *buffer,int length,unsigned char *md5)
{
	MD5_CTX ctx;
	MD5_Init(&ctx);
	MD5_Update(&ctx,buffer,length);

	MD5_Final(md5,&ctx);
}

//简单调用方法(要求数据在时间和空间上都是连续的)
void mite_MD5_simpleCal(char *buffer,unsigned long length,unsigned char *md5)
{
	MD5((const unsigned char *)buffer,length,md5);
}

//EVP 接口方法
void mite_MD5_evpCal(char *buffer,unsigned long length,unsigned char *md5)
{
	EVP_MD_CTX ctx;
	const EVP_MD *type = EVP_md5();
//	OpenSSL_add_all_digests();

	EVP_DigestInit(&ctx,type);
	EVP_DigestUpdate(&ctx,buffer,length);
	EVP_DigestFinal(&ctx,md5,NULL);
}

//BIO调用方式
void mite_MD5_BIOCal(char *buffer,unsigned long length,unsigned char *md5)
{
	BIO* bio_null;
	BIO* bio_md;
	bio_null = BIO_new(BIO_s_null());
	bio_md = BIO_new(BIO_f_md());

	BIO_set_md(bio_md,EVP_md5());
	bio_md = BIO_push(bio_md,bio_null);

	BIO_write(bio_md,buffer,length);
	BIO_flush(bio_md);
	//int len = BIO_gets(bio_md,(char *)md5,EVP_MAX_MD_SIZE);
	//数据经过这种BIO不会被修改也不会被保留，只有摘要值是保留的
	//所以需要用BIO_gets而不是BIO_read获取数据
	BIO_gets(bio_md,(char *)md5,EVP_MAX_MD_SIZE);
	BIO_free_all(bio_md);
	//BIO_free_all(bio_null); 段错误
}



  
int mite_CalFileMD5(char *filename,char *md5)
{  
	MD5_CTX ctx;  
	int len = 0;  
	int flen = 0;
	unsigned char buffer[1024] = {0};  
	unsigned char digest[16] = {0};  

    FILE *pFile = fopen(filename, "rb");
	if(pFile == NULL)
	{
		perror("Open File");
		return -1;
	}

    MD5_Init(&ctx);

    while ((len = fread(buffer, 1, 1024, pFile)) > 0)
	{  
        MD5_Update(&ctx, buffer, len);
		flen += len;
	}  

    MD5_Final(digest, &ctx);

	fclose(pFile);  

	getmd5str(digest,md5,16);
	return flen;
}



int mite_CalFileMD5_with_offset(char *filename,int offset,char *md5)
{  
	MD5_CTX ctx;  
	int len = 0;  
	int flen = 0;
	unsigned char buffer[1024] = {0};  
	unsigned char digest[16] = {0};  

	FILE *pFile = fopen (filename, "rb");
	if(pFile == NULL)
	{
		perror("Open File");
		return -1;
	}
	
	int ret = 0;
	ret = fseek(pFile,offset,SEEK_SET);
	if(ret != 0)
	{
		perror("fseek:");
		return -1;
	}

	MD5_Init (&ctx);  

	while ((len = fread (buffer, 1, 1024, pFile)) > 0)  
	{  
		MD5_Update (&ctx, buffer, len);  
		flen += len;
	}  

	MD5_Final (digest, &ctx);  

	fclose(pFile);  

	getmd5str(digest,md5,16);
	return flen;
}


/*
int main(int argc,char **argv)
{
	char *tmp = (char *)"Hello World";
	unsigned char *md51 = (unsigned char *)malloc(MD5_DIGEST_LENGTH);
	unsigned char *md52 = (unsigned char *)malloc(MD5_DIGEST_LENGTH);
	unsigned char *md53 = (unsigned char *)malloc(MD5_DIGEST_LENGTH);
	MD5_simpleCal(tmp,strlen(tmp),md51);
	MD5_standarCal(tmp,strlen(tmp),md52);

	//MD5_evpCal(tmp,strlen(tmp),md53);
	
	MD5_BIOCal(tmp,strlen(tmp),md53);

	printHex(md51,MD5_DIGEST_LENGTH);
	printHex(md52,MD5_DIGEST_LENGTH);
	printHex(md53,MD5_DIGEST_LENGTH);

	free(md51);
	free(md52);
	free(md53);

	return 0;
}

*/
