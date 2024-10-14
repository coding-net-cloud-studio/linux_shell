# 定义根目录
ROOT=..
# 获取系统类型
PLATFORM=$(shell $(ROOT)/systype.sh)
# 包含平台相关的定义文件
include $(ROOT)/Make.defines.$(PLATFORM)

# 定义要编译的程序列表
PROGS = doatexit echoarg getrlimit hello1 testjmp

# 默认目标,编译所有程序并生成目标文件
all:	$(PROGS) opendata.o scope.o

# 编译每个程序的规则
# $@ 表示当前目标,$< 表示第一个依赖
$(PROGS):	$(LIBAPUE)
	$(CC) $(CFLAGS) $@.c -o $@ $(LDFLAGS) $(LDLIBS)

# 清理生成的文件
clean:
	rm -f $(PROGS) $(TEMPFILES) *.o

# 包含库相关的Makefile片段
include $(ROOT)/Make.libapue.inc
