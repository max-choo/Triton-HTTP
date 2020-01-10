CC=g++
CFLAGS=-ggdb -std=c++11 -Wall -Wextra -pedantic -Werror

H_FILES = httpd.h Framer.hpp Parser.hpp ResponseHelper.hpp
FILES = main.cpp httpd.cpp DieWithError.cpp HandleTCPClient.cpp Framer.cpp Parser.cpp

all: httpd

httpd:	$(FILES) $(H_FILES)
	$(CC) $(CFLAGS) -o httpd $(FILES) -lpthread

clean:
	rm -rf httpd
