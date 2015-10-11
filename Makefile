
CFLAGS=$(shell pkg-config --cflags gio-2.0) \
       $(shell pkg-config --cflags json-glib-1.0) \
			 $(shell curl-config --cflags)
LIBS=$(shell pkg-config --libs gio-2.0) \
     $(shell pkg-config --libs json-glib-1.0) \
		 $(shell curl-config --libs)

all:
	gcc ${CFLAGS} yt-ringer-main.c -o yt-ringer ${LIBS}

objects: *.c *.h
	gcc ${CFLAGS} maman-bar.c objects-test.c -o objects ${LIBS}

preprocess:
	gcc -E ${CFLAGS} yt-ringer-main.c > preprocessed.c
