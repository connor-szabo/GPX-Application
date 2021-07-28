#Connor Szabo
CC=gcc
CFLAGS=`xml2-config --cflags`
LIBS=`xml2-config --libs`

all: parser

clean:
	rm -f ./bin/*.o
	rm -f ./bin/*.so
	rm -f a.out
	rm -f ~/GPXApp/*.so

parser: LinkedListAPI.o GPXParser.o ExtraHelp.o
	$(CC) -fPIC -shared -o ../libgpxparse.so ./bin/GPXParser.o ./bin/LinkedListAPI.o ./bin/ExtraHelp.o -lxml2

LinkedListAPI.o:
	$(CC) $(CFLAGS) -fPIC -c ./src/LinkedListAPI.c -I ./include/ -o ./bin/LinkedListAPI.o

GPXParser.o:
	$(CC) $(CFLAGS) -fPIC -c ./src/GPXParser.c -I ./include/ -I $(LIBS) -o ./bin/GPXParser.o

ExtraHelp.o:
	$(CC) $(CFLAGS) -fPIC -c ./src/ExtraHelp.c -I ./include/ -I $(LIBS) -o ./bin/ExtraHelp.o