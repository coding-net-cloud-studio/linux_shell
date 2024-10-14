# 定义根目录
ROOT=..
# 获取系统类型
PLATFORM=$(shell $(ROOT)/systype.sh)
# 包含平台相关的定义
include $(ROOT)/Make.defines.$(PLATFORM)

# 根据平台类型定义ZAP变量
ifeq "$(PLATFORM)" "linux"
	ZAP = zap
else
	ZAP =
endif

# 定义要编译的程序列表,我修改了这里,注释掉了原先的一行
# PROGS =	access cdpwd changemod filetype mycd umask unlink $(ZAP)
PROGS =	access cdpwd changemod devrdev filetype mycd umask unlink $(ZAP)

# 定义其他要编译的程序
MOREPROGS = ftw8

# 默认目标,编译所有程序
all:	$(PROGS) $(MOREPROGS)

# 编译PROGS列表中的每个程序
$(PROGS):	$(LIBAPUE)
	$(CC) $(CFLAGS) $@.c -o $@ $(LDFLAGS) $(LDLIBS)

# 编译ftw8程序
ftw8:	ftw8.c $(LIBAPUE)
	$(CC) $(CFLAGS) $(NAMEMAX) ftw8.c -o ftw8 $(LDFLAGS) $(LDLIBS)

# 清理生成的文件
clean:
	rm -f $(PROGS) $(MOREPROGS) $(TEMPFILES) *.o $(ZAP)

# 包含libapue库的定义
include $(ROOT)/Make.libapue.inc
