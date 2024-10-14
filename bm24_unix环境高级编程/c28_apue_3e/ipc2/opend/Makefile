# 定义根目录路径
ROOT=../..
# 获取系统类型
PLATFORM=$(shell $(ROOT)/systype.sh)
# 包含平台相关的定义
include $(ROOT)/Make.defines.$(PLATFORM)

# 如果平台是solaris,添加额外的库
ifeq "$(PLATFORM)" "solaris"
  EXTRALIBS=-lsocket -lnsl
endif

# 定义要编译的程序列表
PROGS = opend.poll opend.select

# 默认目标:编译所有程序
all:	$(PROGS)

# 每个程序的编译规则
$(PROGS):	$(LIBAPUE)

# opend.poll程序的编译规则
opend.poll:	main.o request.o cliargs.o client.o loop.poll.o
	$(CC) $(CFLAGS) -o opend.poll main.o cliargs.o client.o request.o loop.poll.o \
		$(LDFLAGS) $(LDLIBS)

# opend.select程序的编译规则
opend.select:	main.o request.o cliargs.o client.o loop.select.o
	$(CC) $(CFLAGS) -o opend.select main.o cliargs.o client.o request.o loop.select.o \
		$(LDFLAGS) $(LDLIBS)

# 清理规则:删除编译生成的文件
clean:
	rm -f $(PROGS) $(TEMPFILES) *.o

# 包含APUE库的编译规则
include $(ROOT)/Make.libapue.inc
