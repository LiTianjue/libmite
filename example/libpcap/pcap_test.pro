TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.c \
    tests/capturetest.c \
    tests/filtertest.c \
    tests/findalldevstest.c \
    tests/opentest.c \
    tests/reactivatetest.c \
    tests/selpolltest.c \
    tests/valgrindtest.c \
    mite_pcap_helper.c

include(deployment.pri)
qtcAddDeployment()


INCLUDEPATH += include

HEADERS += \
    include/pcap/bluetooth.h \
    include/pcap/bpf.h \
    include/pcap/ipnet.h \
    include/pcap/namedb.h \
    include/pcap/nflog.h \
    include/pcap/pcap.h \
    include/pcap/sll.h \
    include/pcap/usb.h \
    include/pcap/vlan.h \
    include/pcap-bpf.h \
    include/pcap-namedb.h \
    include/pcap.h \
    mite_pcap_netheadr.h \
    mite_pcap_helper.h \
    include/net/ethernet.h \
    include/net/if.h \
    include/net/if_arp.h \
    include/net/if_packet.h \
    include/net/if_ppp.h \
    include/net/if_shaper.h \
    include/net/if_slip.h \
    include/net/ppp-comp.h \
    include/net/ppp_defs.h \
    include/net/route.h \
    include/netinet/ether.h \
    include/netinet/icmp6.h \
    include/netinet/if_ether.h \
    include/netinet/if_fddi.h \
    include/netinet/if_tr.h \
    include/netinet/igmp.h \
    include/netinet/in.h \
    include/netinet/in_systm.h \
    include/netinet/ip.h \
    include/netinet/ip6.h \
    include/netinet/ip_icmp.h \
    include/netinet/tcp.h \
    include/netinet/udp.h
