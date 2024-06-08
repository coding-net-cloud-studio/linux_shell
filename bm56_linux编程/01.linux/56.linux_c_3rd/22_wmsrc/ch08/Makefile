# Makefile for Beginning Linux Programming 3rd Edition
# Chapter 8

# sudo apt update
# sudo apt install libmysqlclient-dev

CONNECT=connect1 connect2
INSERT=insert1 insert2
SELECT=select1 select2 select3 select4
UPDATE=update1

CC=gcc

ALL=$(CONNECT) $(INSERT) $(SELECT) $(UPDATE)

all: $(ALL)

CFLAGS=-I/usr/include/mysql
# CFLAGS=-I/usr/include/mysql -Wall -g
# LDFLAGS=-lmysqlclient -g
# LDFLAGS=-lmysqlclient -g -L/usr/lib/mysql

# LDFLAGS=-L/usr/lib/x86_64-linux-gnu -lmysqlclient
LDFLAGS=-lmysqlclient

# gcc connect1.c -o connect1 -I/usr/include/mysql -lmysqlclient
# gcc connect2.c -o connect2 -I/usr/include/mysql -lmysqlclient

%: %.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

clean:
	@rm -f $(ALL) *~
