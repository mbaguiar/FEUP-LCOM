# Makefile for proj
COMPILER_TYPE= gnu

CC= gcc

PROG= proj

SRCS= proj.c handler.c vbe.c video_gr.c keyboard.c timer_modular.c mouse.c Game.c GameCtrl.c bitmap.c GameModel.c rtc.c serialport.c MultiplayerCtrl.c rtcIntHandlerASM.S

CFLAGS= -Wall

DPADD+= ${LIBDRIVER} ${LIBSYS}

LDADD+= -llm -ldriver -lsys


LDFLAGS+= -L .


MAN=


CPPFLAGS+= -D PROJ

BINDIR?= /usr/sbin


.include <bsd.prog.mk>

.include <bsd.gcc.mk>
