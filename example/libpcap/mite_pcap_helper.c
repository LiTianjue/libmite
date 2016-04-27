#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include <pcap.h>
#include "mite_pcap_helper.h"

//打印地址信息
static int _ifprintDev(pcap_if_t *d);
//地址结构转换
static char *iptos(bpf_u_int32 in);

/***************************************
 * 打印所有网络设备的信息
 ***************************************/
int mite_pcap_findAllDev()
{
  pcap_if_t *alldevs;
  pcap_if_t *d;
  char *s;
  bpf_u_int32 net, mask;
  int exit_status = 0;

  char errbuf[PCAP_ERRBUF_SIZE+1];
  if (pcap_findalldevs(&alldevs, errbuf) == -1)
  {
    fprintf(stderr,"Error in pcap_findalldevs: %s\n",errbuf);
    return -1;
  }
  for(d=alldevs;d;d=d->next)
  {
      //打印设备信息
    if (!_ifprintDev(d))
      exit_status = 2;
  }

  if ( (s = pcap_lookupdev(errbuf)) == NULL)
  {
    fprintf(stderr,"Error in pcap_lookupdev: %s\n",errbuf);
    exit_status = 2;
  }
  else
  {
    printf("Preferred device name: %s\n",s);
  }

  if (pcap_lookupnet(s, &net, &mask, errbuf) < 0)
  {
    fprintf(stderr,"Error in pcap_lookupnet: %s\n",errbuf);
    exit_status = 2;
  }
  else
  {
    printf("Preferred device is on network: %s/%s\n",iptos(net), iptos(mask));
  }

  return exit_status;
}


/***************************************
 * 抓包到文件
 * 不支持any 和 NULL
 ***************************************/
static void packet_save_cb
(u_char *user,const struct pcap_pkthdr* packet_heaert,const u_char *packet_content)
{
    static int packet_number = 1;
    printf("The %d packet is captured len %d\n",packet_number,packet_heaert->len);
    pcap_dump(user,packet_heaert,packet_content);
    pcap_dump_flush((pcap_dumper_t* )user);
    packet_number++;
}

int mite_pcap_captureToFile(char *devname,char *savefile,char *rule)
{
    pcap_t* pcap_handler;
    pcap_dumper_t *dumpfp;

    char error_content[PCAP_ERRBUF_SIZE];

    struct bpf_program bpf_filter;

    bpf_u_int32 net_mask;
    bpf_u_int32 net_ip;
    if(pcap_lookupnet(devname,&net_ip,&net_mask,error_content)<0)
    {
        printf("Can not select dev %s [%s]\n",devname,error_content);
        return -1;
    }

    //混杂模式，无超时
    pcap_handler = pcap_open_live(devname,BUFSIZ,1,0,error_content);
    if(pcap_handler == NULL)
    {
        fprintf(stderr,"Open Dev :%s Error[%s]\n",devname,error_content);
        return -2;
    }
    //编译过滤规则
    if(pcap_compile(pcap_handler,&bpf_filter,rule,0,net_ip) < 0){
        pcap_perror(pcap_handler,"Rule:");
        return -3;
    }

    pcap_setfilter(pcap_handler,&bpf_filter);

    dumpfp = pcap_dump_open(pcap_handler,savefile);
    if(dumpfp == NULL){
        fprintf(stderr,"Erro on open %s\n",savefile);
        return -4;
    }

    pcap_loop(pcap_handler,-1,packet_save_cb,(u_char *)dumpfp);

    pcap_close(pcap_handler);
    pcap_dump_close(dumpfp);
}


int mite_pcap_handlerFile(char *filename,pcap_handler handler,u_char *user)
{
    char errbuf[PCAP_ERRBUF_SIZE];

    // open capture file for offline processing
    pcap_t * descr = pcap_open_offline(filename, errbuf);
    if (descr == NULL) {
        //cout << "pcap_open_live() failed: " << errbuf << endl;
        fprintf(stderr,"pcap_open_offline:%s\n",errbuf);
        return -1;
    }

    // start packet processing loop, just like live capture
    if (pcap_loop(descr, 0,handler, user) < 0) {
        fprintf(stderr,"pcap_loop err\n");
        return -2;
    }

    printf("Handler File Finish\n");
    return 0;
}





/***********************************************
 * static call
 **********************************************/
static int _ifprintDev(pcap_if_t *d)
{
    pcap_addr_t *a;

#ifdef INET6
  char ntop_buf[INET6_ADDRSTRLEN];
#endif
  int status = 1; /* success */

  //打印设备名称
   printf("%s\n",d->name);
   //如果有设备描述，打印设备描述
   if(d->description)
       printf("\t Description: %s\n",d->description);

   //查看是不是回环地址
    printf("\tLoopback: %s\n",(d->flags & PCAP_IF_LOOPBACK)?"yes":"no");

    //打印设备地址信息
    for(a=d->addresses;a;a=a->next) {
        if(a->addr != NULL){
            switch(a->addr->sa_family) {
            case AF_INET:
                //IPV4
                printf("\tAddress Family: AF_INET\n");
                //打印IP
                if (a->addr)
                    printf("\t\tAddress: %s\n",
                           inet_ntoa(((struct sockaddr_in *)(a->addr))->sin_addr));
                //掩码
                if (a->netmask)
                    printf("\t\tNetmask: %s\n",
                           inet_ntoa(((struct sockaddr_in *)(a->netmask))->sin_addr));
                //广播地址
                if (a->broadaddr)
                    printf("\t\tBroadcast Address: %s\n",
                           inet_ntoa(((struct sockaddr_in *)(a->broadaddr))->sin_addr));
                //目的地址
                if (a->dstaddr)
                    printf("\t\tDestination Address: %s\n",
                           inet_ntoa(((struct sockaddr_in *)(a->dstaddr))->sin_addr));
                break;
#ifdef INET6
            case AF_INET6
            printf("\tAddress Family: AF_INET6\n");
            //其他信息我们暂时不用，地址格式转换用inet_ntop
            break;
#endif
            default:
                printf("\tAddress Family: Unknown (%d)\n", a->addr->sa_family);
                break;
            }
        }else{
            fprintf(stderr, "\tWarning: a->addr is NULL, skipping this address.\n");
        }
    }

    printf("\n");
    return status;
}

#define IPTOSBUFFERS	12
static char *iptos(bpf_u_int32 in)
{
    static char output[IPTOSBUFFERS][3*4+3+1];
    static short which;
    u_char *p;

    p = (u_char *)&in;
    which = (which + 1 == IPTOSBUFFERS ? 0 : which + 1);
    sprintf(output[which], "%d.%d.%d.%d", p[0], p[1], p[2], p[3]);
    return output[which];
}
























