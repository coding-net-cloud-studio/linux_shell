# 定义根目录
ROOT=..
# 定义额外的库
EXTRALIBS=-pthread
# 获取当前平台类型
PLATFORM=$(shell $(ROOT)/systype.sh)
# 包含平台相关的定义文件
include $(ROOT)/Make.defines.$(PLATFORM)

# 初始化变量
BAR =
# 根据平台类型设置不同的锁和库
ifeq "$(PLATFORM)" "macos"
  TLOCK =
  EXTRALIBS=-pthread
else
  TLOCK = timedlock
endif
ifeq "$(PLATFORM)" "linux"
  BAR = barrier
  EXTRALIBS=-pthread -lrt -lbsd
endif
ifeq "$(PLATFORM)" "freebsd"
  BAR = barrier
  EXTRALIBS=-pthread
endif
ifeq "$(PLATFORM)" "solaris"
  BAR = barrier
  EXTRALIBS=-lpthread -lrt
endif

# 定义要编译的程序列表
PROGS = badexit2 cleanup exitstatus threadid

# 默认目标,编译所有程序
all:	$(PROGS) condvar.o maketimeout.o mutex1.o mutex2.o mutex3.o rwlock.o $(TLOCK) $(BAR)

# 编译每个程序
$(PROGS):	$(LIBAPUE)
		$(CC) $(CFLAGS) $@.c -o $@ $(LDFLAGS) $(LDLIBS)

# 清理生成的文件
clean:
	rm -f $(PROGS) $(TEMPFILES) *.o $(TLOCK) $(BAR)

# 包含库相关的Makefile
include $(ROOT)/Make.libapue.inc
