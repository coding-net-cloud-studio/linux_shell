# 定义根目录
ROOT=..
# 获取系统类型
PLATFORM=$(shell $(ROOT)/systype.sh)
# 包含平台相关的Makefile定义
include $(ROOT)/Make.defines.$(PLATFORM)

# 定义要编译的程序列表
PROGS =	add2 add2stdio devzero myuclc pipe1 pipe2 pipe4 popen1 popen2 tshm

# 默认目标,编译所有程序
all:	$(PROGS) popen.o slock.o tellwait.o

# 编译每个程序的规则
$(PROGS):	$(LIBAPUE)
		$(CC) $(CFLAGS) $@.c -o $@ $(LDFLAGS) $(LDLIBS)

# 编译slock.o的目标文件
slock.o:	slock.c slock.h

# 清理规则,删除编译生成的文件
clean:
	rm -f $(PROGS) $(TEMPFILES) *.o

# 包含libapue库的Makefile
include $(ROOT)/Make.libapue.inc
