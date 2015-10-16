
CFLAGS=$(shell pkg-config --cflags gio-2.0) \
       $(shell pkg-config --cflags json-glib-1.0) \
			 $(shell curl-config --cflags) \
			 -std=c99

LIBS=$(shell pkg-config --libs gio-2.0) \
     $(shell pkg-config --libs json-glib-1.0) \
		 $(shell curl-config --libs)

all:
	gcc ${CFLAGS} yt-ringer-main.c channel.c string-builder.c -o yt-ringer ${LIBS}

objects: *.c *.h
	gcc ${CFLAGS} maman-bar.c objects-test.c -o objects ${LIBS}

preprocess:
	gcc -E ${CFLAGS} channel.c > preprocessed.c

tests:
		# gcc -D"CHANNEL_TESTS" ${CFLAGS} channel.c string-builder.c string-utils.c curl-utils.c -o channel-test ${LIBS}
		# ./channel-test
		# gcc -D"STRING_BUILDER_TESTS" ${CFLAGS} string-builder.c string-utils.c -o string-builder-test ${LIBS}
		# ./string-builder-test
		# gcc -D"CURL_UTILS_TESTS" ${CFLAGS} curl-utils.c string-utils.c string-builder.c -o curl-utils-test ${LIBS}
		# ./curl-utils-test
		# gcc -D"STRING_UTILS_TESTS" ${CFLAGS} string-utils.c string-builder.c -o string-utils-test ${LIBS}
		# ./string-utils-test
		gcc -D"JSON_QUERY_TESTS" ${CFLAGS} json-query.c -o json-query-tests ${LIBS}
		./json-query-tests
