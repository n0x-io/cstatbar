# cStatBar Makefile

SRC = cstatbar.c
OBJ = ${SRC:.c=.o}
CC = cc
CFLAGS = -std=c99 -g
LDFLAGS = -lX11


all: options cstatbar

options:
	@echo cstatusbar build options are as follow:
	@echo "CFLAGS = ${CFLAGS}"
	@echo "LDFLAGS = ${LDFLAGS}"
	@echo "CC = ${CC}"

.c.o:
	${CC} -c ${CFLAGS} $<

${OBJ}: config.h

config.h:
	cp config.def.h $@

cstatbar: ${OBJ}
	${CC} -o $@ ${OBJ} ${LDFLAGS}

clean:
	rm -f cstatbar ${OBJ}

.PHONY: all options clean
