# 定义根目录
ROOT=..
# 获取系统类型
PLATFORM=$(shell $(ROOT)/systype.sh)
# 包含平台相关的定义文件
include $(ROOT)/Make.defines.$(PLATFORM)

# 如果平台是solaris,添加额外的库
ifeq "$(PLATFORM)" "solaris"
  EXTRALIBS=-lsocket -lnsl
endif

# 定义要编译的程序
PROGS =	pty

# 默认目标,编译所有程序
all:	$(PROGS)

# 编译pty程序,依赖于LIBAPUE库
$(PROGS):	$(LIBAPUE)

# pty程序的具体编译规则,依赖于main.o, loop.o, driver.o
pty:	main.o loop.o driver.o
	$(CC) $(CFLAGS) -o pty main.o loop.o driver.o $(LDFLAGS) $(LDLIBS)

# 清理生成的文件
clean:
	rm -f $(PROGS) $(TEMPFILES) *.o

# 包含libapue库的编译规则
include $(ROOT)/Make.libapue.inc
