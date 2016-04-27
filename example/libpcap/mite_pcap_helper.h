#ifndef MITE_PCAP_HELPER_H
#define MITE_PCAP_HELPER_H
#include <pcap.h>

//定义pcap_loop 的回调函数



int mite_pcap_findAllDev();

//抓包到文件
int mite_pcap_captureToFile(char *devname,char *savefile,char *rule);

//读取离线文件并处理
int mite_pcap_handlerFile(char *filename, pcap_handler handler, u_char *user);

#endif // MITE_PCAP_HELPER_H

