PROGS = proxy client
CLEANFILES = *.o
CC = g++

all:	${PROGS}

proxy:	proxy.o
		${CC} -o $@ proxy.o

client:	client.o
		${CC} -o $@ client.o


clean:
		rm -f ${PROGS} ${CLEANFILES}
