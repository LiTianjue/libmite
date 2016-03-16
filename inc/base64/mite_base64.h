#ifndef __MITE_BASE64__
#define __MITE_BASE64__

//free it outside
//格式化输出
int mite_string_2_base64(char *input, int input_len, char **base64);
//原始输出
int mite_string_2_base64_block(char *input,int input_len,char **base64);


int mite_base64_2_string(char *base64, int inlen, char **ppout);
int mite_base64_2_string_block(char *base64, int inlen, char **ppout);







#endif
