CC=gcc

LDFLAGS= -lmysqlclient

CFLAGS= 


TARGET = mysql_test

all:$(TARGET)
mysql_test:mysql_test.c
	@$(CC) -o mysql_test mysql_test.c  $(SRC) $(INC) $(LDFLAGS)  $(CFLAGS)

clean:
	@rm *.o $(TARGET) -f

