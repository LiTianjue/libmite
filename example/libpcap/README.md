
通过将网卡设置为混杂模式，可以捕获所有经过该网络接口的数据包

1.捕获数据包
2.构造发送数据包
3.流量采集与统计
4.规则过滤
5.分析网络协议
6.网络流量发生器
7.入侵检测
8.网络扫描


基本编程流程
1.查找网络设备
2.打开网络设备
3.获取网络参数
4.编译过滤规则
5.设置过滤器
6.利用回调函数捕获数据包
7.关闭网络设备

本示例将基于libpcap编写测试函数，
目标
可以作为编程参考，
直接提供封装函数调用


1.对网络协议头的定义
	mite_pcap_protocolHead.h
	linux下可以直接使用
	#include <net/ethernet.h>
	#include <net/if.h>
	#include <netinet/ip.h>
	#include <netinet/udp.h>
	#include <netinet/tcp.h>
	windows 下可能需要手动添加协议头或者使用MinGw不是特别清楚


2.libpcap主要函数
	//打开pcap网络句柄
	pcap_open_live

	//打开pcap本地文件或脱机监听
	pcap_open_offline
	pcap_offline_read

	//打开用于保存网络包的文件句柄
	pcap_dump_open

	//返回的网络设备名指针
	pcap_lookupdev

	//获得网络设备的网络号和掩码
	pcap_lookupnet

	//捕获并处理数据包
	pcap_dispatch()
	pcap_loop()

	//向打开的文件输入数据包
	pcap_dump()

	//将字符串编译到过滤程序中
	pcap_compile

	//指定过滤程序
	pcap_setfilter

	//指向下一个数据包
	pcap_next

	//返回数据链路层类型 DLT_EN10MB
	pcap_datalink(pcap_t *p)

	//打印库的错误信息
	pcap_perror()
	pcap_geterr
	pcap_strerror(int error)

	//关闭
	pcap_close
	pcap_dump_close

3.主要结构	
	pcap_if pcap_if_t	//pcap句柄，描述设备
	pcap_addr_t			//设备的地址信息 pcap_if_t->address

	pcap_pkthdr			//数据包信息，时间戳，长度





