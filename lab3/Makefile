# Makefile for lab3

COMPILER_TYPE= gnu

CC= gcc

PROG= lab3
SRCS= lab3.c test3.c keyboard.c timer_modular.c readOutBufASM.S

CFLAGS= -Wall

DPADD+= ${LIBDRIVER} ${LIBSYS}
LDADD+= -ldriver -lsys

LDFLAGS+= -L .

MAN=

CPPFLAGS+= -D LAB3

.include <bsd.prog.mk>
.include <bsd.gcc.mk>