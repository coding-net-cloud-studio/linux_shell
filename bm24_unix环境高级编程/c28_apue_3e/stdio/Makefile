# 定义根目录
ROOT=..
# 获取系统类型
PLATFORM=$(shell $(ROOT)/systype.sh)
# 包含平台相关的定义
include $(ROOT)/Make.defines.$(PLATFORM)

# 根据平台定义MEMSTR变量,如果是linux平台,则MEMSTR为memstr,否则为空
ifeq "$(PLATFORM)" "linux"
  MEMSTR = memstr
else
  MEMSTR =
endif

# 定义要编译的程序列表,这里已经由用户修改过
# 注意:原始代码中有两行被注释掉的相同定义,已被用户修改为只有一行有效定义
# wmtag_memo_我修改了这里_开始
PROGS =	buf fgetsfputs getcharbug getcputc mkstemp tempfiles
# wmtag_memo_我修改了这里_结束

# 默认目标,编译所有程序和MEMSTR定义的程序
all: clean $(PROGS) $(MEMSTR)
	-@git add .
	-@git status

# 编译每个程序的规则
# $(PROGS)表示所有要编译的程序
# $(LIBAPUE)是编译这些程序所需的库
# $(CC)是编译器,$(CFLAGS)是编译选项,$@.c表示当前程序的源文件
# -o $@表示输出的可执行文件名与程序名相同,$(LDFLAGS)和$(LDLIBS)是链接选项和库
$(PROGS):	$(LIBAPUE)
	$(CC) $(CFLAGS) $@.c -o $@ $(LDFLAGS) $(LDLIBS)

# 清理规则,删除编译生成的程序,临时文件和对象文件
clean:
	rm -f $(PROGS) $(TEMPFILES) *.o $(MEMSTR)

# 包含APUE库的编译规则
include $(ROOT)/Make.libapue.inc
