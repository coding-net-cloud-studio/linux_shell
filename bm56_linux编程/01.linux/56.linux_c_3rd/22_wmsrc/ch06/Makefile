ALL=ipmode moveadd pad screen1 color keypad multiw1 subscl

all: ipmode moveadd pad screen1 color keypad multiw1 subscl

#Uncomment and edit the line below if necessary
#CFLAGS=-I/usr/include/ncurses

CC=gcc

LDFLAGS=-lncurses

%: %.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

clean:
	-@rm -f $(ALL) *~
