//杂项测试

#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include "inc/utils/mite_utils.h"



int main()
{
    //
    char data[8] = {"1234567"};
    mite_utils_printHex(data,8);
    uint8_t lrc=0;
    lrc = mite_utils_chekcLRC(data,8);
    printf("lrc:%02x\n",lrc);


    return 0;
}
