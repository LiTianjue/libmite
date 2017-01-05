CC = gcc
EXEC = myftp
OBJS = main.o users.o command.o

$(EXEC): $(OBJS)
		   $(CC) -o $(EXEC) $(OBJS)

main.o: main.c global.h
		$(CC) -c $<
users.o: users.c users.h global.h
		$(CC) -c $<
command.o: command.c command.h global.h
		$(CC) -c $<

clean:
		rm -rf $(EXEC) *.o

debug: $(CC) += -g
debug: $(EXEC)
