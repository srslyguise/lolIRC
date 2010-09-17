CC=g++
CFLAGS=-Iinclude -Wno-deprecated -Wno-deprecated-declarations
LIBS=-lpthread

all: test/test_parser test/test_connection test/test

test_parser: test/test_parser

test_connection: test/test_connection

test: test/test

src/lolIRC.o: src/lolIRC.cpp include/lolIRC.hpp
	$(CC) $(CFLAGS) -c -o src/lolIRC.o src/lolIRC.cpp $(LIBS)

src/lolIRC_responses.o: src/lolIRC_responses.cpp include/lolIRC.hpp
	$(CC) $(CFLAGS) -c -o src/lolIRC_responses.o src/lolIRC_responses.cpp

src/lolIRC_messages.o: src/lolIRC_messages.cpp include/lolIRC.hpp
	$(CC) $(CFLAGS) -c -o src/lolIRC_messages.o src/lolIRC_messages.cpp

src/lolIRC_connection.o: src/lolIRC_connection.cpp include/lolIRC_connection.hpp
	$(CC) $(CFLAGS) -c -o src/lolIRC_connection.o src/lolIRC_connection.cpp

src/lolIRC_parser.o: src/lolIRC_parser.cpp include/lolIRC_parser.hpp
	$(CC) $(CFLAGS) -c -o src/lolIRC_parser.o src/lolIRC_parser.cpp

src/lolIRC_channel.o: src/lolIRC_channel.cpp include/lolIRC_channel.hpp
	$(CC) $(CFLAGS) -c -o src/lolIRC_channel.o src/lolIRC_channel.cpp

src/lolIRC_nick.o: src/lolIRC_nick.cpp include/lolIRC_nick.hpp
	$(CC) $(CFLAGS) -c -o src/lolIRC_nick.o src/lolIRC_nick.cpp

test/test_parser: test/test_parser.cpp src/lolIRC_parser.o
	$(CC) $(CFLAGS) -o test/test_parser test/test_parser.cpp src/lolIRC_parser.o

test/test_connection: test/test_connection.cpp src/lolIRC_connection.o
	$(CC) $(CFLAGS) -o test/test_connection test/test_connection.cpp src/lolIRC_connection.o

test/test: test/test.cpp src/lolIRC.o src/lolIRC_responses.o src/lolIRC_messages.o src/lolIRC_connection.o src/lolIRC_parser.o src/lolIRC_channel.o src/lolIRC_nick.o
	$(CC) $(CFLAGS) -o test/test test/test.cpp src/lolIRC.o src/lolIRC_responses.o src/lolIRC_messages.o src/lolIRC_connection.o src/lolIRC_parser.o src/lolIRC_channel.o src/lolIRC_nick.o $(LIBS)

clean:
	rm src/*.o
	rm test/test_parser
	rm test/test_connection
	rm test/test
