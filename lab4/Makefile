# Makefile for lab4	
COMPILER_TYPE= gnu
	
CC= gcc
	
PROG= lab4
	
SRCS= lab4.c test4.c mouse.c keyboard.c timer_modular.c
		
CFLAGS= -Wall
	
DPADD+= ${LIBDRIVER} ${LIBSYS}
	
LDADD+= -ldriver -lsys
	
	
LDFLAGS+= -L .
	
	
MAN=
	
	
CPPFLAGS+= -D LAB4
	
	
.include <bsd.prog.mk>
	
.include <bsd.gcc.mk>