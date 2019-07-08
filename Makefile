CC = gcc
CFLAGS = -std=gnu99 -Wall -Wextra

default: traceroute

traceroute: main.o send.o receive.o
	$(CC) $(CFLAGS) -o traceroute main.o send.o receive.o -lm

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

send.o: send.c send.h
	$(CC) $(CFLAGS) -c send.c

receive.o: receive.c receive.h
	$(CC) $(CFLAGS) -c receive.c

clean:
	$(RM) count *.o *~

distclean: clean
	$(RM) traceroute
