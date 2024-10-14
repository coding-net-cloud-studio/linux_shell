# Makefile 用于 misc 库.
#
# 定义根目录和平台相关的宏
ROOT=..
PLATFORM=$(shell $(ROOT)/systype.sh)
# 包含平台相关的定义
include $(ROOT)/Make.defines.$(PLATFORM)

# 定义库文件名和目标文件列表
LIBMISC	= libapue.a
OBJS   = bufargs.o cliconn.o clrfl.o \
			daemonize.o error.o errorlog.o lockreg.o locktest.o \
			openmax.o pathalloc.o popen.o prexit.o prmask.o \
			ptyfork.o ptyopen.o readn.o recvfd.o senderr.o sendfd.o \
			servaccept.o servlisten.o setfd.o setfl.o signal.o signalintr.o \
			sleepus.o spipe.o tellwait.o ttymodes.o writen.o

# 默认目标:构建库文件和 sleep.o
all:	$(LIBMISC) sleep.o

# 构建库文件的规则
$(LIBMISC):	$(OBJS)
	$(AR) rv $(LIBMISC) $? # 使用 ar 工具创建库文件
	$(RANLIB) $(LIBMISC) # 为库文件创建索引

# 清理规则:删除所有生成的文件
clean:
	rm -f *.o a.out core temp.* $(LIBMISC)

# 包含其他库相关的 make 规则
include $(ROOT)/Make.libapue.inc
