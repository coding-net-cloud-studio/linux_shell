# 定义根目录为上一级目录
ROOT=..

# 获取当前系统的类型，该命令会执行systype.sh脚本并将结果赋值给PLATFORM变量
PLATFORM=$(shell $(ROOT)/systype.sh)

# 根据不同的平台类型，包含相应的Makefile定义文件
include $(ROOT)/Make.defines.$(PLATFORM)

# 默认目标，编译生成init.o, reread.o, reread2.o, single.o四个目标文件
all:	init.o reread.o reread2.o single.o

# 清理目标，删除临时文件和所有.o目标文件
clean:
	rm -f $(TEMPFILES) *.o
