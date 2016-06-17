CFLAGS += -g  -Wall

file=main

OBJS=main.o 
OBJS+=fileclass.o
OBJS+=flvclass.o

main:$(OBJS)
	$(CC) -g -o $(file) $(OBJS)

clean:
	rm -f *.o
	rm -f $(file)
