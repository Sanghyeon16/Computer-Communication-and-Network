PROGS =	echo echos
CLEANFILES = *.o

all:	${PROGS}

echo:	echo.o
		${CC} ${CFLAGS} -o $@ echo.o 
echos:	echos.o
		${CC} ${CFLAGS} -o $@ echos.o

clean:
		rm -f ${PROGS} ${CLEANFILES}
