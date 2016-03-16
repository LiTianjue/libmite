#ifndef __MD5_DECODE_H__
#define __MD5_DECODE_H__

//标准接口
void mite_MD5_standarCal(char *buffer,int length,unsigned char *md5);

//简单接口
void mite_MD5_simpleCal(char *buffer,unsigned long length,unsigned char *md5);


//EVP接口
void mite_MD5_evpCal(char *buffer,unsigned long length,unsigned char *md5);

//BIO接口
void mite_MD5_BIOCal(char *buffer,unsigned long length,unsigned char *md5);



//输入md5值，输出hex表示的md5字符串32字节,长度16字节
void getmd5str(unsigned char *md5,char *hex,int length);


int mite_CalFileMD5(char *filename,char *md5);
int mite_CalFileMD5_with_offset(char *filename,int offset,char *md5);

#endif
