# 定义根目录路径
ROOT=../..

# 获取系统类型并赋值给PLATFORM变量
PLATFORM=$(shell $(ROOT)/systype.sh)

# 包含平台相关的定义文件
include $(ROOT)/Make.defines.$(PLATFORM)

# 如果平台是solaris,添加额外的库
ifeq "$(PLATFORM)" "solaris"
  EXTRALIBS=-lsocket -lnsl
endif

# 定义要编译的程序列表
PROGS =	openclient

# 默认目标,编译所有程序
all:	$(PROGS)

# 编译指定程序,依赖于LIBAPUE库
$(PROGS):	$(LIBAPUE)

# 编译openclient程序,依赖于main.o和open.o
openclient:	main.o open.o
	$(CC) $(CFLAGS) -o openclient main.o open.o $(LDFLAGS) $(LDLIBS)

# 清理生成的文件
clean:
	rm -f $(PROGS) $(TEMPFILES) *.o

# 包含libapue库的编译规则
include $(ROOT)/Make.libapue.inc
