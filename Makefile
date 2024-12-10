CC     = gcc
CFLAGS := -Wall

# default is to build with address sanitizer enabled
all: client server

client: client.c
	$(CC) -o $@ $(CFLAGS) $^

server: server.c
	$(CC) -o $@ $(CFLAGS) $^

%.o : %.c
	$(CC) -c -o $@ $(CFLAGS) $<

.PHONY: clean

clean:
	rm -fr *.o client server

