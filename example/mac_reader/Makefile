CC=gcc
#CC=arm-none-linux-gnueabi-gcc
#CC=mipsel-linux-gcc

LDFLAGS=
CFLAGS= -I ./

TARGET = cmd_test
SRC=*.c

all:*.o
	@$(CC) -o $(TARGET) *.o $(LDFLAGS) $(CFLAGS)

*.o:$(SRC)
	@$(CC) -c $(SRC) $(CFLAGS)

clean:
	@rm *.o $(TARGET) -f

