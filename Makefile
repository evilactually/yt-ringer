
CFLAGS=$(shell pkg-config --cflags gio-2.0)
LIBS=$(shell pkg-config --libs gio-2.0)

all:
	gcc ${CFLAGS} yt-ringer-main.c -o yt-ringer ${LIBS}
