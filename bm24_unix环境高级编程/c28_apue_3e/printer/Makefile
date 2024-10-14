# 定义根目录
ROOT=..
# 获取系统类型
PLATFORM=$(shell $(ROOT)/systype.sh)
# 包含平台相关的定义文件
include $(ROOT)/Make.defines.$(PLATFORM)
# 初始化额外库为空
EXTRA=

# 如果平台是solaris,添加特定的库
ifeq "$(PLATFORM)" "solaris"
  EXTRALIBS=-lsocket -lnsl -lrt -lpthread
else
  # 否则只添加pthread库
  EXTRALIBS=-pthread
endif

# 定义要编译的程序
PROGS = print printd
# 定义头文件
HDRS = print.h ipp.h

# 默认目标:编译所有程序
all:	$(PROGS) 

# 程序依赖于libapue库
$(PROGS):	$(LIBAPUE)

# util.o依赖于util.c和头文件
util.o:		util.c $(HDRS)

# print.o依赖于print.c和头文件
print.o:	print.c $(HDRS)

# printd.o依赖于printd.c和头文件
printd.o:	printd.c $(HDRS)

# 编译print程序
print:		print.o util.o $(ROOT)/sockets/clconn2.o
		$(CC) $(CFLAGS) -o print print.o util.o $(ROOT)/sockets/clconn2.o $(LDFLAGS) $(LDDIR) $(LDLIBS)

# 编译printd程序
printd:		printd.o util.o $(ROOT)/sockets/clconn2.o $(ROOT)/sockets/initsrv2.o
		$(CC) $(CFLAGS) -o printd printd.o util.o $(ROOT)/sockets/clconn2.o $(ROOT)/sockets/initsrv2.o \
			$(LDFLAGS) $(LDDIR) $(LDLIBS)

# 清理生成的文件
clean:
	rm -f $(PROGS) $(TEMPFILES) *.o

# 包含libapue库的编译规则
include $(ROOT)/Make.libapue.inc
