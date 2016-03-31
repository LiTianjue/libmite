CC=gcc

LDFLAGS= 

CFLAGS= 


TARGET = cli srv
SRC = *.c

all:$(TARGET)
cli:tcp_cli.c
	@$(CC) -o cli tcp_cli.c  $(INC) $(LDFLAGS)  $(CFLAGS)
srv:tcp_srv.c
	@$(CC) -o srv tcp_srv.c  $(INC) $(LDFLAGS)  $(CFLAGS)

clean:
	@rm *.o $(TARGET) -f

