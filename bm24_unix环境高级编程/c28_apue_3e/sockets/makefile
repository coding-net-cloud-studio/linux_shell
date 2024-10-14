# 定义根目录
ROOT=..
# 获取系统类型
PLATFORM=$(shell $(ROOT)/systype.sh)
# 包含平台相关的定义
include $(ROOT)/Make.defines.$(PLATFORM)

# 如果平台是solaris,添加额外的库
ifeq "$(PLATFORM)" "solaris"
  EXTRALIBS = -lsocket -lnsl
endif

# 定义要编译的程序列表
PROGS = ruptime ruptimed ruptimed-fd ruptimed-dg
# 定义其他要编译的程序列表
MOREPROGS = findsvc ruptime-dg

# 默认目标,编译所有程序
all:	$(PROGS) $(MOREPROGS) clconn.o clconn2.o initsrv1.o initsrv2.o 

# 指定编译PROGS列表中的程序需要的库
$(PROGS):	$(LIBAPUE)

# 编译ruptime程序
ruptime:	ruptime.o clconn2.o
		$(CC) $(CFLAGS) -o ruptime ruptime.o clconn2.o $(LDFLAGS) $(LDLIBS)

# 编译ruptimed程序
ruptimed:	ruptimed.o initsrv2.o
		$(CC) $(CFLAGS) -o ruptimed ruptimed.o initsrv2.o $(LDFLAGS) $(LDLIBS)

# 编译ruptimed-fd程序
ruptimed-fd:	ruptimed-fd.o initsrv2.o
		$(CC) $(CFLAGS) -o ruptimed-fd ruptimed-fd.o initsrv2.o $(LDFLAGS) $(LDLIBS)

# 编译ruptimed-dg程序
ruptimed-dg:	ruptimed-dg.o initsrv2.o
		$(CC) $(CFLAGS) -o ruptimed-dg ruptimed-dg.o initsrv2.o $(LDFLAGS) $(LDLIBS)

# 清理编译生成的文件
clean:
	rm -f $(PROGS) $(MOREPROGS) $(TEMPFILES) *.o

# 包含libapue库的编译规则
include $(ROOT)/Make.libapue.inc
