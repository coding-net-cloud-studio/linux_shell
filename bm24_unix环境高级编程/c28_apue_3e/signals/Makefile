# 定义根目录
ROOT=..
# 获取系统类型
PLATFORM=$(shell $(ROOT)/systype.sh)
# 包含平台相关的定义文件
include $(ROOT)/Make.defines.$(PLATFORM)

# 初始化子进程程序名称
CLD =

# 如果平台是linux或solaris,则设置子进程程序名称为child
ifeq "$(PLATFORM)" "linux"
  CLD = child
endif
ifeq "$(PLATFORM)" "solaris"
  CLD = child
endif

# 定义要编译的程序列表
PROGS =	critical mask read1 read2 reenter sigtstp sigusr suspend1 suspend2
# 定义额外的程序列表
MOREPROGS = systest2 tsleep2

# 默认目标:编译所有程序
all:	$(PROGS) $(MOREPROGS) abort.o sleep1.o sleep2.o system.o $(CLD)

# 编译主程序列表
$(PROGS):	$(LIBAPUE)
		$(CC) $(CFLAGS) $@.c -o $@ $(LDFLAGS) $(LDLIBS)

# 编译tsleep2程序
tsleep2:	tsleep2.o sleep2.o
		$(CC) $(CFLAGS) -o tsleep2 tsleep2.o sleep2.o $(LDFLAGS) $(LDLIBS)

# 编译systest2程序
systest2:	systest2.o system.o
		$(CC) $(CFLAGS) -o systest2 systest2.o system.o $(LDFLAGS) $(LDLIBS)

# 清理生成的文件
clean:
	rm -f $(PROGS) $(MOREPROGS) $(TEMPFILES) *.o file.hole $(CLD)

# 包含APUE库的定义文件
include $(ROOT)/Make.libapue.inc
