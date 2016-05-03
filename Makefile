CC=gcc

LDFLAGS= -lm -lcrypto
LDFLAGS+= -lpthread

CFLAGS= -g

SRC+=src/base64/*.c
SRC+=src/md5/*.c
SRC+=src/json/*.c
SRC+=src/threadpool/*.c
SRC+=src/filehelper/*.c
SRC+=src/sockethelper/*.c
SRC+=src/utils/*.c
SRC+=src/crypto/*.c

INC= -I./

TARGET = base64_test md5_test json_test
TARGET += thread_pool_test
TARGET += file_test
TARGET += socket_test
TARGET += utils_test
TARGET += sm4_test
TARGET += xxtea_test


all:$(TARGET)

base64_test:test/base64_test.c 
	@$(CC) -o bin/base64_test test/base64_test.c  $(SRC) $(INC) $(LDFLAGS) $(CFLAGS)

md5_test:test/md5_test.c 
	@$(CC) -o bin/md5_test test/md5_test.c  $(SRC) $(INC) $(LDFLAGS) $(CFLAGS)

json_test:test/json_test.c 
	@$(CC) -o bin/json_test test/json_test.c  $(SRC) $(INC) $(LDFLAGS) $(CFLAGS)

thread_pool_test:test/thread_pool_test.c 
	@$(CC) -o bin/thread_pool_test test/thread_pool_test.c  $(SRC) $(INC) $(LDFLAGS) $(CFLAGS)

file_test:test/file_test.c 
	@$(CC) -o bin/file_test test/file_test.c  $(SRC) $(INC) $(LDFLAGS) $(CFLAGS)

socket_test:test/socket_test.c 
	@$(CC) -o bin/socket_test test/socket_test.c  $(SRC) $(INC) $(LDFLAGS) $(CFLAGS)

utils_test:test/utils_test.c 
	@$(CC) -o bin/utils_test test/utils_test.c  $(SRC) $(INC) $(LDFLAGS) $(CFLAGS)

sm4_test:test/sm4_test.c 
	@$(CC) -o bin/sm4_test test/sm4_test.c  $(SRC) $(INC) $(LDFLAGS) $(CFLAGS)

xxtea_test:test/xxtea_test.c 
	@$(CC) -o bin/xxtea_test test/xxtea_test.c  $(SRC) $(INC) $(LDFLAGS) $(CFLAGS)


clean:
	@rm *.o $(TARGET) bin/* -f

