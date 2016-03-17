#ifndef __MITE_UTILS_H__
#define __MITE_UTILS_H__
#include <stdint.h>


//16进制打印字串
void mite_utils_printHex(const uint8_t *pbtData,const size_t szBytes);

//计算 LRC
uint8_t mite_utils_chekcLRC(uint8_t *data,int len);











#endif

