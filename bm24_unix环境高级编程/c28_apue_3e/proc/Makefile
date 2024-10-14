# 定义根目录
ROOT=..
# 获取系统类型
PLATFORM=$(shell $(ROOT)/systype.sh)
# 包含平台相关的定义文件
include $(ROOT)/Make.defines.$(PLATFORM)

# 初始化账户相关变量
ACCT=
ACCTFLAGS=
# 根据不同的平台设置账户程序及其编译标志
ifeq "$(PLATFORM)" "linux"
  ACCT=pracct
  ACCTFLAGS = -DHAS_AXSIG -DHAS_ACORE
endif
ifeq "$(PLATFORM)" "macos"
  ACCT=pracct
  ACCTFLAGS = -DHAS_AXSIG -DHAS_ACORE
endif
ifeq "$(PLATFORM)" "solaris"
  ACCT=pracct
  ACCTFLAGS = -DHAS_AC_STAT
endif

# 定义要编译的程序列表
PROGS =	echoall exec1 exec2 fork1 fork2 nice pruids tellwait1 tellwait2 test1 times1 vfork1 wait1
# 定义更多的程序列表
MOREPROGS = systest1 systest3

# 默认目标,编译所有程序
all:	$(PROGS) $(MOREPROGS) system.o $(ACCT)

# 编译普通程序的规则
$(PROGS):	$(LIBAPUE)
		$(CC) $(CFLAGS) $@.c -o $@ $(LDFLAGS) $(LDLIBS)

# 编译systest1程序的规则
systest1:	system.o systest1.o
		$(CC) $(CFLAGS) -o systest1 systest1.o system.o $(LDFLAGS) $(LDLIBS)

# 编译systest3程序的规则
systest3:	system.o systest3.o
		$(CC) $(CFLAGS) -o systest3 systest3.o system.o $(LDFLAGS) $(LDLIBS)

# 编译pracct程序的规则
pracct:		pracct.c
		$(CC) $(CFLAGS) $(ACCTFLAGS) -o pracct pracct.c $(LDFLAGS) $(LDLIBS)

# 清理生成的文件
clean:
	rm -f $(PROGS) $(MOREPROGS) $(TEMPFILES) *.o $(ACCT)

# 包含libapue库的编译规则
include $(ROOT)/Make.libapue.inc
