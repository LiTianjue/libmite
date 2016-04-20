CC=gcc

LDFLAGS= -lcurl

CFLAGS= 


TARGET = http_test

all:$(TARGET)
http_test:http_test.c
	@$(CC) -o http_test http_test.c  $(SRC) $(INC) $(LDFLAGS)  $(CFLAGS)

clean:
	@rm *.o $(TARGET) -f

