# 定义根目录
ROOT=..
# 获取系统类型
PLATFORM=$(shell $(ROOT)/systype.sh)
# 包含平台相关的定义
include $(ROOT)/Make.defines.$(PLATFORM)

# 根据不同的操作系统平台,定义不同的变量
ifeq "$(PLATFORM)" "freebsd"
  GETPW = getpw44bsd  # FreeBSD系统使用的函数
  FMEM = fmemopen.o   # FreeBSD系统需要编译的内存文件打开对象文件
  EXTRALIBS=-pthread # FreeBSD系统需要链接的额外库
endif
ifeq "$(PLATFORM)" "linux"
  GETPW = getpwsvr4   # Linux系统使用的函数
  FMEM =              # Linux系统不需要额外编译的内存文件打开对象文件
  EXTRALIBS=-pthread # Linux系统需要链接的额外库
endif
ifeq "$(PLATFORM)" "macos"
  GETPW = getpw44bsd  # macOS系统使用的函数
  FMEM = fmemopen.o   # macOS系统需要编译的内存文件打开对象文件
endif
ifeq "$(PLATFORM)" "solaris"
  GETPW = getpwsvr4   # Solaris系统使用的函数
  FMEM =              # Solaris系统不需要额外编译的内存文件打开对象文件
endif

# 定义要编译的程序列表
PROGS =	bo fifo1 getlogin goodexit longpath pendlock prtime sizepipe vfork3 zombie

# 默认目标,编译所有程序
all:	$(PROGS) asyncsocket.o $(FMEM) openmax.o sleep.o sleepus_poll.o sleepus_select.o $(GETPW)

# 编译每个程序的规则
$(PROGS):	$(LIBAPUE)
		$(CC) $(CFLAGS) $@.c -o $@ $(LDFLAGS) $(LDLIBS)

# 清理规则,删除编译生成的文件
clean:
	rm -f $(PROGS) $(TEMPFILES) *.o $(GETPW)

# 包含APUE库的定义
include $(ROOT)/Make.libapue.inc
