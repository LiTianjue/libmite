#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/ip_icmp.h>
#include <netinet/udp.h>
#include <netinet/tcp.h>
#include <netinet/ip.h>
#include <netinet/if_ether.h>
#include <net/ethernet.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define BUFFER_SIZE 65536

FILE *logfile;
int tcp = 0, udp = 0, icmp = 0, igmp = 0, other = 0, total = 0;
int sock_raw;
struct sockaddr_in source, dest;

void ProcessPacket(unsigned char *, int);
void LogIpHeader(unsigned char *, int);
void LogEthernetHeader(unsigned char *, int);
void LogIcmpPacket(unsigned char *, int);
void LogTcpPacket(unsigned char *, int);
void LogUdpPacket(unsigned char *, int);
void LogData(unsigned char *, int);

int main()
{
    socklen_t saddr_size;
    int data_size;
    struct sockaddr saddr;
    struct in_addr in;

    unsigned char *buffer = (unsigned char *) malloc(BUFFER_SIZE);

    logfile = fopen("log.txt", "w");
    if (logfile == NULL) {
        printf("Unable to create log file.");
        return 1;
    }

    sock_raw = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if (sock_raw < 0) {
        printf("Failed to initiliaze socket.");
        return 1;
    }

    while (true) {
        saddr_size = sizeof saddr;

        data_size = recvfrom(sock_raw, buffer, BUFFER_SIZE, 0, &saddr, &saddr_size);
        if (data_size < 0) {
            printf("Recvfrom failed to get packet.");
            return 1;
        }

        ProcessPacket(buffer, data_size);
    }

    close(sock_raw);

    return 0;
}

void ProcessPacket(unsigned char *buffer, int size)
{
    struct iphdr *iph = (struct iphdr*) (buffer + sizeof(struct ethhdr));

    switch (iph->protocol) {
        case 1: // ICMP Protocol
            icmp++;
            LogIcmpPacket(buffer, size);
            break;
        case 2: // IGMP Protocol
            igmp++;
            break;
        case 6: // TCP Protocol
            tcp++;
            LogTcpPacket(buffer, size);
            break;
        case 17: // UDP Protocol
            udp++;
            LogUdpPacket(buffer, size);
            break;
        default: // Other Protocol
            other++;
    }

    total++;
    printf("TCP: %d | UDP: %d | ICMP: %d | IGMP: %d | Other: %d | Total: %d\r", tcp, udp, icmp, igmp, other, total);
}

void LogEthernetHeader(unsigned char *buffer, int size)
{
    struct ethhdr *eth = (struct ethhdr *) buffer;

    fprintf(logfile, "\n");
    fprintf(logfile, "Ethernet Header\n");
    fprintf(logfile, " |-Destination Address : %.2X-%.2X-%.2X-%.2X-%.2X-%.2X \n", eth->h_dest[0], eth->h_dest[1], eth->h_dest[2], eth->h_dest[3], eth->h_dest[4], eth->h_dest[5]);
    fprintf(logfile, " |-Source Address      : %.2X-%.2X-%.2X-%.2X-%.2X-%.2X \n", eth->h_source[0], eth->h_source[1], eth->h_source[2], eth->h_source[3], eth->h_source[4], eth->h_source[5]);
    fprintf(logfile, " |-Protocol            : %u \n",(unsigned short) eth->h_proto);
}

void LogIpHeader(unsigned char *buffer, int size)
{
    LogEthernetHeader(buffer, size);
   
    unsigned short iphdrlen;
         
    struct iphdr *iph = (struct iphdr *) (buffer  + sizeof(struct ethhdr));
    iphdrlen = iph->ihl * 4;
     
    memset(&source, 0, sizeof(source));
    source.sin_addr.s_addr = iph->saddr;
     
    memset(&dest, 0, sizeof(dest));
    dest.sin_addr.s_addr = iph->daddr;
     
    fprintf(logfile, "\n");
    fprintf(logfile, "IP Header\n");
    fprintf(logfile, " |-IP Version          : %d\n", (unsigned int) iph->version);
    fprintf(logfile, " |-IP Header Length    : %d DWORDS or %d Bytes\n", (unsigned int) iph->ihl, ((unsigned int) (iph->ihl)) * 4);
    fprintf(logfile, " |-Type Of Service     : %d\n", (unsigned int) iph->tos);
    fprintf(logfile, " |-IP Total Length     : %d  Bytes (Size of Packet)\n", ntohs(iph->tot_len));
    fprintf(logfile, " |-TTL                 : %d\n", (unsigned int) iph->ttl);
    fprintf(logfile, " |-Protocol            : %d\n", (unsigned int) iph->protocol);
    fprintf(logfile, " |-Checksum            : %d\n", ntohs(iph->check));
    fprintf(logfile, " |-Source IP           : %s\n", inet_ntoa(source.sin_addr));
    fprintf(logfile, " |-Destination IP      : %s\n", inet_ntoa(dest.sin_addr));
}

void LogIcmpPacket(unsigned char *buffer, int size)
{
    unsigned short iphdrlen;
     
    struct iphdr *iph = (struct iphdr *) (buffer + sizeof(struct ethhdr));
    iphdrlen = iph->ihl * 4;
     
    struct icmphdr *icmph = (struct icmphdr *) (buffer + iphdrlen + sizeof(struct ethhdr));
     
    int header_size =  sizeof(struct ethhdr) + iphdrlen + sizeof icmph;
     
    fprintf(logfile, "\n\n***********************ICMP Packet*************************\n"); 
     
    LogIpHeader(buffer, size);
             
    fprintf(logfile, "\n");
    fprintf(logfile, "ICMP Header\n");
    fprintf(logfile, " |-Type     : %d", (unsigned int) (icmph->type));
             
    if((unsigned int) (icmph->type) == 11)
    {
        fprintf(logfile, "  (TTL Expired)\n");
    }
    else if((unsigned int) (icmph->type) == ICMP_ECHOREPLY)
    {
        fprintf(logfile, "  (ICMP Echo Reply)\n");
    }
     
    fprintf(logfile, " |-Code     : %d\n", (unsigned int) (icmph->code));
    fprintf(logfile, " |-Checksum : %d\n", ntohs(icmph->checksum));
    fprintf(logfile, "\n");
 
    fprintf(logfile, "IP Header\n");
    LogData(buffer, iphdrlen);
         
    fprintf(logfile , "UDP Header\n");
    LogData(buffer + iphdrlen, sizeof icmph);
         
    fprintf(logfile, "Data Payload\n");    
     
    //Move the pointer ahead and reduce the size of string
    LogData(buffer + header_size, (size - header_size));
     
    fprintf(logfile, "\n###########################################################");
}

void LogTcpPacket(unsigned char *buffer, int size)
{
    unsigned short iphdrlen;
     
    struct iphdr *iph = (struct iphdr *) (buffer + sizeof(struct ethhdr));
    iphdrlen = iph->ihl * 4;
     
    struct tcphdr *tcph = (struct tcphdr *) (buffer + iphdrlen + sizeof(struct ethhdr));
             
    int header_size =  sizeof(struct ethhdr) + iphdrlen + tcph->doff * 4;
     
    fprintf(logfile, "\n\n***********************TCP Packet*************************\n");  
         
    LogIpHeader(buffer, size);
         
    fprintf(logfile, "\n");
    fprintf(logfile, "TCP Header\n");
    fprintf(logfile, " |-Source Port          : %u\n", ntohs(tcph->source));
    fprintf(logfile, " |-Destination Port     : %u\n", ntohs(tcph->dest));
    fprintf(logfile, " |-Sequence Number      : %u\n", ntohl(tcph->seq));
    fprintf(logfile, " |-Acknowledge Number   : %u\n", ntohl(tcph->ack_seq));
    fprintf(logfile, " |-Header Length        : %d DWORDS or %d BYTES\n", (unsigned int) tcph->doff, (unsigned int) tcph->doff * 4);
    fprintf(logfile, " |-Urgent Flag          : %d\n", (unsigned int) tcph->urg);
    fprintf(logfile, " |-Acknowledgement Flag : %d\n", (unsigned int) tcph->ack);
    fprintf(logfile, " |-Push Flag            : %d\n", (unsigned int) tcph->psh);
    fprintf(logfile, " |-Reset Flag           : %d\n", (unsigned int) tcph->rst);
    fprintf(logfile, " |-Synchronise Flag     : %d\n", (unsigned int) tcph->syn);
    fprintf(logfile, " |-Finish Flag          : %d\n", (unsigned int) tcph->fin);
    fprintf(logfile, " |-Window               : %d\n", ntohs(tcph->window));
    fprintf(logfile, " |-Checksum             : %d\n", ntohs(tcph->check));
    fprintf(logfile, " |-Urgent Pointer       : %d\n", tcph->urg_ptr);
    fprintf(logfile, "\n");
    fprintf(logfile, "                        DATA Dump                         ");
    fprintf(logfile, "\n");
         
    fprintf(logfile, "IP Header\n");
    LogData(buffer, iphdrlen);
         
    fprintf(logfile, "TCP Header\n");
    LogData(buffer + iphdrlen, tcph->doff * 4);
         
    fprintf(logfile, "Data Payload\n");    
    LogData(buffer + header_size, size - header_size);
                         
    fprintf(logfile, "\n###########################################################");
}

void LogUdpPacket(unsigned char *buffer, int size)
{
    unsigned short iphdrlen;
     
    struct iphdr *iph = (struct iphdr *) (buffer + sizeof(struct ethhdr));
    iphdrlen = iph->ihl * 4;
     
    struct udphdr *udph = (struct udphdr *) (buffer + iphdrlen + sizeof(struct ethhdr));
     
    int header_size = sizeof(struct ethhdr) + iphdrlen + sizeof udph;
     
    fprintf(logfile, "\n\n***********************UDP Packet*************************\n");
     
    LogIpHeader(buffer, size);           
     
    fprintf(logfile, "\nUDP Header\n");
    fprintf(logfile, " |-Source Port      : %d\n", ntohs(udph->source));
    fprintf(logfile, " |-Destination Port : %d\n", ntohs(udph->dest));
    fprintf(logfile, " |-UDP Length       : %d\n", ntohs(udph->len));
    fprintf(logfile, " |-UDP Checksum     : %d\n", ntohs(udph->check));
     
    fprintf(logfile, "\n");
    fprintf(logfile, "IP Header\n");
    LogData(buffer, iphdrlen);
         
    fprintf(logfile, "UDP Header\n");
    LogData(buffer + iphdrlen, sizeof udph);
         
    fprintf(logfile, "Data Payload\n");    
     
    //Move the pointer ahead and reduce the size of string
    LogData(buffer + header_size, size - header_size);
     
    fprintf(logfile, "\n###########################################################");
}

void LogData(unsigned char *buffer, int size)
{
    int i, j;
    for (i = 0; i < size; i++) {
        if (i != 0 && i % 16 == 0) { // One line of hex printing is complete
            fprintf(logfile, "         ");
            
            for (j = i - 16; j < i; j++) {
                if (buffer[j] >= 32 && buffer[j] <= 128) {
                    fprintf(logfile, "%c", (unsigned char) buffer[j]); // Number of letter
                } else {
                    fprintf(logfile, "."); // Otherwise dot
                }

                fprintf(logfile, "\n");
            }
        }

        if (i % 16 == 0) {
            fprintf(logfile, "\n");
        }
        fprintf(logfile, " %02X", (unsigned int) buffer[i]);

        if (i == size - 1) { // Last spaces
            for(j = 0; j < 15 - i % 16; j++)  {
              fprintf(logfile, "   "); //extra spaces
            }
             
            fprintf(logfile, "         ");
             
            for(j = i - i % 16; j <= i; j++) {
                if(buffer[j] >= 32 && buffer[j] <= 128) {
                  fprintf(logfile, "%c", (unsigned char) buffer[j]);
                } else {
                  fprintf(logfile, ".");
                }
            }
             
            fprintf(logfile,  "\n");
        }
    }
}