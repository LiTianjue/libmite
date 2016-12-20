CC=gcc

LDFLAGS= 

CFLAGS= 


TARGET = fifo_test
SRC = *.c

all:$(TARGET)
${TARGET}:${SRC}
	@$(CC) -o ${TARGET} ${SRC} $(INC) $(LDFLAGS)  $(CFLAGS)

clean:
	@rm *.o $(TARGET) -f

