# 定义根目录
ROOT=..
# 获取系统类型
PLATFORM=$(shell $(ROOT)/systype.sh)
# 包含平台相关的定义
include $(ROOT)/Make.defines.$(PLATFORM)

# 定义要编译的程序列表
PROGS =	fileflags hole mycat seek

# 默认目标:编译所有程序并生成setfl.o文件
all:	$(PROGS) setfl.o

# 编译每个程序的规则
# $@ 表示目标,$< 表示第一个依赖
$(PROGS):	$(LIBAPUE)
		$(CC) $(CFLAGS) $@.c -o $@ $(LDFLAGS) $(LDLIBS)

# 清理规则:删除编译生成的文件
clean:
	rm -f $(PROGS) $(TEMPFILES) *.o file.hole

# 包含libapue库的编译规则
include $(ROOT)/Make.libapue.inc
