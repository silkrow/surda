CC := gcc
CFLAGS :=-Wall -g
LDFLAGS := -g
HEADERS := surda.h myshell.h log.h printing.h Makefile 
EXE := surda
SOURCES := surda.o

.ALL: ${EXE}

%.o: %.c ${HEADERS} Makefile
	${CC} -c ${CFLAGS} -o $@ $<

${EXE}: ${SOURCES}
	$(CC) ${SOURCES} -o ${EXE} ${LDFLAGS}

clean::
	rm -f ${SOURCES}
