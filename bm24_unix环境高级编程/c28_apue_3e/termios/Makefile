# 定义根目录
ROOT=..
# 获取系统类型
PLATFORM=$(shell $(ROOT)/systype.sh)
# 包含平台相关的定义
include $(ROOT)/Make.defines.$(PLATFORM)

# 定义要编译的程序列表
PROGS =	csize settty t_getpass t_isatty t_raw t_ttyname winch

# 默认目标:编译所有程序并生成目标文件
all:	$(PROGS) ctermid.o getpass.o isatty.o ttyname.o

# 编译每个程序的规则
# $(PROGS) 是要编译的程序列表
# $(LIBAPUE) 是编译这些程序所需的库
$(PROGS):	$(LIBAPUE)
		# 使用CC编译器,CFLAGS编译选项,$@.c 源文件,-o $@ 输出可执行文件名
		# LDFLAGS 链接选项,LDLIBS 链接库
		$(CC) $(CFLAGS) $@.c -o $@ $(LDFLAGS) $(LDLIBS)

# 清理生成的文件
clean:
	rm -f $(PROGS) $(TEMPFILES) *.o

# 包含libapue库的编译规则
include $(ROOT)/Make.libapue.inc
