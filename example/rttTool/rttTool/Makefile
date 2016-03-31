CC=gcc

LDFLAGS= 

CFLAGS= 


TARGET = cli srv
SRC = *.c

all:$(TARGET)
cli:rtt_cli.c
	@$(CC) -o cli rtt_cli.c client_util.c  $(INC) $(LDFLAGS)  $(CFLAGS)
srv:rtt_srv.c
	@$(CC) -o srv rtt_srv.c  $(INC) $(LDFLAGS)  $(CFLAGS)

clean:
	@rm *.o $(TARGET) -f

