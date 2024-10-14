# 定义根目录路径
ROOT=../..

# 获取系统类型并赋值给PLATFORM变量
PLATFORM=$(shell $(ROOT)/systype.sh)

# 根据系统类型包含相应的Makefile定义
include $(ROOT)/Make.defines.$(PLATFORM)

# 如果平台是solaris,添加额外的库
ifeq "$(PLATFORM)" "solaris"
  EXTRALIBS=-lsocket -lnsl
endif

# 定义要编译的程序列表
PROGS =	opend

# 默认目标:编译所有程序
all:	$(PROGS)

# 每个程序的编译规则
$(PROGS):	$(LIBAPUE)

# opend程序的具体编译规则
opend:	main.o request.o cliargs.o
	$(CC) $(CFLAGS) -o opend main.o cliargs.o request.o $(LDFLAGS) $(LDLIBS)

# 清理规则:删除编译生成的文件
clean:
	rm -f $(PROGS) $(TEMPFILES) *.o

# 包含libapue库的Makefile
include $(ROOT)/Make.libapue.inc
