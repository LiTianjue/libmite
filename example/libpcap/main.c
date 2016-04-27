#include <stdio.h>
#include <net/ethernet.h>
#include <net/if.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <netinet/tcp.h>

#include "pcap.h"
#include "mite_pcap_helper.h"


void packetHandler(u_char *userData, const struct pcap_pkthdr* pkthdr, const u_char* packet) {

    const struct ether_header* ethernetHeader;  // 以太网头
    const struct ip* ipHeader;                  // ip头
    const struct tcphdr* tcpHeader;             // tcp 头
    char sourceIp[INET_ADDRSTRLEN];             // 源地址
    char destIp[INET_ADDRSTRLEN];               // 目的地址
    u_int sourcePort, destPort;                 // 端口
    u_char *data;
    int dataLength = 0;
    char dataStr[1450] = {0};


    //取出以太网头，看是不是IP协议包
    ethernetHeader = (struct ether_header*)packet;
    if(ntohs(ethernetHeader->ether_type) == ETHERTYPE_IP) {
    //取出IP头，源地址，目的地址
        ipHeader = (struct ip*)(packet+sizeof(struct ether_header));
        inet_ntop(AF_INET, &(ipHeader->ip_src), sourceIp, INET_ADDRSTRLEN);
        inet_ntop(AF_INET, &(ipHeader->ip_dst), destIp, INET_ADDRSTRLEN);

    //取出TCP头，截取源端口，目标端口
        if (ipHeader->ip_p == IPPROTO_TCP) {
            tcpHeader = (struct tcphdr*)(packet + sizeof(struct ether_header) + sizeof(struct ip));
            sourcePort = ntohs(tcpHeader->source);
            destPort = ntohs(tcpHeader->dest);

            //只取应用数据
            /*
            if(tcpHeader->th_flags & TH_SYN)
            {
                printf("SYN-");
            }
            if(tcpHeader->th_flags & TH_ACK){
                printf("ACK-");
            }
            if(tcpHeader->th_flags & TH_FIN){
                printf("FIN-");
            }
            if(tcpHeader->th_flags & TH_PUSH){
                printf("PUSH-");
            }
            */

            //取出ip 包头的长度
            data = (u_char*)(packet + sizeof(struct ether_header) + sizeof(struct ip) + sizeof(struct tcphdr));
            dataLength = pkthdr->len - (sizeof(struct ether_header) + sizeof(struct ip) + sizeof(struct tcphdr));

            if(dataLength >0 && (tcpHeader->th_flags & TH_PUSH))
            {
                //取出可打印字符
                int i = 0;
                for(i = 0;i < dataLength;i++) {
                    if ((data[i] >= 32 && data[i] <= 126) || data[i] == 10 || data[i] == 11 || data[i] == 13) {
                        dataStr[i] = (char)data[i];
                    } else {
                        dataStr[i] = '.';
                    }

                }
                dataStr[i+1] = '\0';

                printf("[%s:%d]-->[%s:%d][%d] : %s\n",
                       sourceIp,sourcePort,destIp,destPort,dataLength,data);
            }

        }
    }
}



int main()
{
    //mite_pcap_findAllDev();

    //mite_pcap_captureToFile("eth0","./haha.pcap","tcp port 6000");

    mite_pcap_handlerFile("./haha.pcap",packetHandler,NULL);
    return 0;
}
