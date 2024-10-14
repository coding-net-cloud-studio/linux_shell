# 定义根目录为上一级目录
ROOT=..

# 使用shell脚本获取系统类型,并赋值给PLATFORM变量
PLATFORM=$(shell $(ROOT)/systype.sh)

# 包含根据平台不同而不同的宏定义文件
include $(ROOT)/Make.defines.$(PLATFORM)

# 如果平台是linux,添加额外的库链接选项-lrt
ifeq "$(PLATFORM)" "linux"
  EXTRALIBS=-lrt
endif

# 如果平台是solaris,同样添加额外的库链接选项-lrt
ifeq "$(PLATFORM)" "solaris"
  EXTRALIBS=-lrt
endif

# 定义要编译的程序列表
PROGS =	deadlock mandatory mcopy2 nonblockw rot13a
# 定义额外的程序列表
MOREPROGS = rot13c2

# 默认目标,编译所有程序
all:	$(PROGS) $(MOREPROGS) lockfile.o

# rot13c2.c 文件的生成规则
# 使用 fixup.awk 处理 rot13a.c 并生成 xlate 文件
# 然后从 rot13c2.c.in 中提取到 'same' 为止的内容,重定向到 rot13c2.c
# 接着将 xlate 文件内容追加到 rot13c2.c
# 最后将 rot13c2.c.in 中 'same' 之后的内容追加到 rot13c2.c
rot13c2.c:	rot13c2.c.in $(LIBAPUE)
	./fixup.awk rot13a.c >xlate
	sed '/same/q' rot13c2.c.in >rot13c2.c
	cat xlate >>rot13c2.c
	sed '1,/same/d' rot13c2.c.in >>rot13c2.c

# 编译 PROGS 列表中的每个程序
# 使用 LIBAPUE 库,编译对应的 .c 文件并链接生成可执行文件
$(PROGS):	$(LIBAPUE)
	$(CC) $(CFLAGS) $@.c -o $@ $(LDFLAGS) $(LDLIBS)

# 清理生成的文件
clean:
	rm -f $(PROGS) $(MOREPROGS) $(TEMPFILES) *.o xlate rot13c2.c

# 包含外部定义的 Makefile 规则
include $(ROOT)/Make.libapue.inc
