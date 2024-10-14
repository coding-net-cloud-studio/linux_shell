# 定义根目录
ROOT=..
# 获取系统类型
PLATFORM=$(shell $(ROOT)/systype.sh)
# 包含平台相关的定义文件
include $(ROOT)/Make.defines.$(PLATFORM)

# 初始化TOUT变量
TOUT =
# 如果平台是freebsd,添加-pthread库
ifeq "$(PLATFORM)" "freebsd"
  EXTRALIBS = -pthread
# 如果平台是linux,添加-pthread库和timeout.o目标文件
endif
ifeq "$(PLATFORM)" "linux"
  EXTRALIBS = -pthread
  TOUT = timeout.o
# 如果平台是solaris,添加-lpthread库和timeout.o目标文件
endif
# 定义要编译的程序列表
PROGS =	atfork suspend

# 默认目标,编译所有程序和目标文件
all:	$(PROGS) detach.o getenv1.o getenv2.o getenv3.o $(TOUT)

# 编译每个程序的规则
$(PROGS):	$(LIBAPUE)
		$(CC) $(CFLAGS) $@.c -o $@ $(LDFLAGS) $(LDLIBS)

# 清理生成的文件
clean:
	rm -f $(PROGS) $(TEMPFILES) *.o

# 包含libapue库的编译规则
include $(ROOT)/Make.libapue.inc
