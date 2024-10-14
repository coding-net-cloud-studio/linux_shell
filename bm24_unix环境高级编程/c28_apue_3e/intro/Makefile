# 定义根目录
ROOT=..
# 获取系统类型
PLATFORM=$(shell $(ROOT)/systype.sh)
# 包含平台相关的定义文件
include $(ROOT)/Make.defines.$(PLATFORM)

# 定义要编译的程序列表
PROGS =	getcputc hello ls1 mycat shell1 shell2 testerror uidgid

# 默认目标,编译所有程序
all:	$(PROGS)

# 编译每个程序的规则
# $@ 表示目标,$(LIBAPUE) 表示依赖的库
# $(CC) 是编译器,$(CFLAGS) 是编译选项
# $@.c 表示与目标同名的源文件
# $(LDFLAGS) 和 $(LDLIBS) 是链接选项和库
$(PROGS):	$(LIBAPUE)
		$(CC) $(CFLAGS) $@.c -o $@ $(LDFLAGS) $(LDLIBS)

# 清理生成的文件
clean:
	rm -f $(PROGS) $(TEMPFILES) *.o

# 包含APUE库的编译规则
include $(ROOT)/Make.libapue.inc
