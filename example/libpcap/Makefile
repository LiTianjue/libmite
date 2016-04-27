CC=gcc

LDFLAGS= -lpcap

CFLAGS= 


TARGET = pcap_test
SRC=*.c

all:$(TARGET)
pcap_test:*.c
	@$(CC) -o pcap_test   $(SRC) $(INC) $(LDFLAGS)  $(CFLAGS)

clean:
	@rm *.o $(TARGET) -f

