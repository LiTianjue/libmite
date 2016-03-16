CC=gcc

LDFLAGS= 

CFLAGS= 


TARGET = cmdopt_test

all:$(TARGET)
cmdopt_test:cmdopt_test.c
	@$(CC) -o cmdopt_test cmdopt_test.c  $(SRC) $(INC) $(LDFLAGS)  $(CFLAGS)

clean:
	@rm *.o $(TARGET) -f

