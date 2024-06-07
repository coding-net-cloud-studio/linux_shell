# Makefile for Beginning Linux Programming 3rd Edition
# Chapter 4

ARGS=args argopt longopt
ENV=environ showenv
TIME=envtime gmtime ctime strftime
TMP=tmpnam
USER=user
HOST=hostget
LOG=syslog logmask
RES=limits

LDFLAGS=-lm

# NOTE 原始内容
# ALL= $(ARGS) $(ENV) $(TIME) $(TMP) $(USER) $(HOST) $(LOG) $(RES)

# NOTE 修改内容
ALL= $(ARGS) $(ENV) $(TIME) $(TMP) $(USER) $(HOST) $(LOG)



all: $(ALL) limits

limits: limits.c
	gcc -o limits limits.c $(LDFLAGS)

clean:
	@rm -f $(ALL) $(RES) *~
