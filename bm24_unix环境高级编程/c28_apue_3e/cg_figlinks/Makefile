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
# PROGS10 = fig8.17.c figC.12.c fig8.1.c fig1.9.c fig12.16.c fig7.16.c fig17.4.c fig3.2.c fig11.16.c figC.8.c fig4.22.c fig10.2.c fig5.15.c fig8.3.c fig8.6.c fig10.22.c fig10.10.c
# PROGS20 = fig17.21.c figC.7.c fig15.31.c fig4.16.c fig7.11.c fig8.29.c fig4.23.c fig11.3.c fig10.20.c fig20.3.c fig8.13.c fig15.15.c fig2.14.c fig8.23.c fig18.11.c fig4.9.c
# PROGS30 = fig13.7.c fig8.20.c fig11.5.c fig10.31.c fig8.31.c fig1.6.c fig10.11.c fig6.11.c figC.3.c
# PROGS40 = fig8.12.c fig18.16.c fig10.5.c fig9.12.c fig12.17.c fig15.19.c fig4.3.c fig8.24.c
# PROGS50 = fig8.28.c fig17.18.c fig15.6.c fig15.5.c fig15.17.c fig17.3.c figC.24.c
# PROGS60 = fig14.27.c fig11.2.c fig16.16.c figC.20.c fig11.4.c figC.18.c fig7.9.c
# PROGS70 = fig7.3.c fig18.21.c fig1.3.c figC.22.c fig1.5.c fig10.26.c fig16.9.c
# PROGS80 = fig18.14.c fig14.7.c fig5.12.c fig15.14.c fig4.24.c fig7.1.c fig1.10.c
# PROGS90 = fig10.6.c fig16.19.c fig16.20.c fig8.8.c fig4.12.c fig14.20.c figC.14.c
# PROGSA0 = fig14.1.c fig3.5.c fig10.15.c fig10.23.c fig16.17.c fig15.11.c fig14.12.c
# PROGSB0 = fig5.11.c fig17.28.c fig10.9.c fig7.4.c fig19.11.c fig5.13.c fig13.8.c
# PROGSC0 = fig18.18.c fig4.8.c fig5.4.c fig17.5.c fig7.13.c fig1.7.c fig8.30.c fig15.33.c
# PROGSD0 = fig18.10.c fig3.11.c fig8.16.c fig12.8.c fig5.5.c fig1.4.c fig1.8.c
# PROGSE0 = fig15.18.c fig3.1.c fig4.21.c fig16.18.c fig4.25.c fig18.22.c figC.10.c fig8.25.c figC.15.c

# 定义要编译的程序列表
# PROGS = fig8.17.c figC.12.c fig8.1.c fig1.9.c fig12.16.c fig7.16.c fig17.4.c fig3.2.c fig11.16.c figC.8.c fig4.22.c fig10.2.c fig5.15.c fig8.3.c fig8.6.c fig10.22.c fig10.10.c fig17.21.c figC.7.c fig15.31.c fig4.16.c fig7.11.c fig8.29.c fig4.23.c fig11.3.c fig10.20.c fig20.3.c fig8.13.c fig15.15.c fig2.14.c fig8.23.c fig18.11.c fig4.9.c fig13.7.c fig8.20.c fig11.5.c fig10.31.c fig8.31.c fig1.6.c fig10.11.c fig6.11.c figC.3.c fig8.12.c fig18.16.c fig10.5.c fig9.12.c fig12.17.c fig15.19.c fig4.3.c fig8.24.c fig8.28.c fig17.18.c fig15.6.c fig15.5.c fig15.17.c fig17.3.c figC.24.c fig14.27.c fig11.2.c fig16.16.c figC.20.c fig11.4.c figC.18.c fig7.9.c fig7.3.c fig18.21.c fig1.3.c figC.22.c fig1.5.c fig10.26.c fig16.9.c fig18.14.c fig14.7.c fig5.12.c fig15.14.c fig4.24.c fig7.1.c fig1.10.c fig10.6.c fig16.19.c fig16.20.c fig8.8.c fig4.12.c fig14.20.c figC.14.c fig14.1.c fig3.5.c fig10.15.c fig10.23.c fig16.17.c fig15.11.c fig14.12.c fig5.11.c fig17.28.c fig10.9.c fig7.4.c fig19.11.c fig5.13.c fig13.8.c fig18.18.c fig4.8.c fig5.4.c fig17.5.c fig7.13.c fig1.7.c fig8.30.c fig15.33.c fig18.10.c fig3.11.c fig8.16.c fig12.8.c fig5.5.c fig1.4.c fig1.8.c fig15.18.c fig3.1.c fig4.21.c fig16.18.c fig4.25.c fig18.22.c figC.10.c fig8.25.c figC.15.c
# PROGS = fig8.17 figC.12 fig8.1 fig1.9 fig12.16 fig7.16 fig17.4 fig3.2 fig11.16 figC.8 fig4.22 fig10.2 fig5.15 fig8.3 fig8.6 fig10.22 fig10.10 fig17.21 figC.7 fig15.31 fig4.16 fig7.11 fig8.29 fig4.23 fig11.3 fig10.20 fig20.3 fig8.13 fig15.15 fig2.14 fig8.23 fig18.11 fig4.9 fig13.7 fig8.20 fig11.5 fig10.31 fig8.31 fig1.6 fig10.11 fig6.11 figC.3 fig8.12 fig18.16 fig10.5 fig9.12 fig12.17 fig15.19 fig4.3 fig8.24 fig8.28 fig17.18 fig15.6 fig15.5 fig15.17 fig17.3 figC.24 fig14.27 fig11.2 fig16.16 figC.20 fig11.4 figC.18 fig7.9 fig7.3 fig18.21 fig1.3 figC.22 fig1.5 fig10.26 fig16.9 fig18.14 fig14.7 fig5.12 fig15.14 fig4.24 fig7.1 fig1.10 fig10.6 fig16.19 fig16.20 fig8.8 fig4.12 fig14.20 figC.14 fig14.1 fig3.5 fig10.15 fig10.23 fig16.17 fig15.11 fig14.12 fig5.11 fig17.28 fig10.9 fig7.4 fig19.11 fig5.13 fig13.8 fig18.18 fig4.8 fig5.4 fig17.5 fig7.13 fig1.7 fig8.30 fig15.33 fig18.10 fig3.11 fig8.16 fig12.8 fig5.5 fig1.4 fig1.8 fig15.18 fig3.1 fig4.21 fig16.18 fig4.25 fig18.22 figC.10 fig8.25 figC.15 


PROGS = fig8.17 figC.12 fig8.1 fig1.9 fig12.16 fig7.16 fig17.4 fig3.2 figC.8 fig4.22 fig10.2 fig5.15 fig8.3 fig8.6 fig10.22 fig10.10 figC.7 fig15.31 fig4.16 fig8.29 fig4.23 fig11.3 fig10.20 fig8.13 fig15.15 fig2.14 fig8.23 fig18.11 fig4.9 fig8.20 fig11.5 fig10.31 fig8.31 fig1.6 fig10.11 fig6.11 figC.3 fig8.12 fig18.16 fig10.5 fig9.12 fig12.17 fig15.19 fig4.3 fig8.24 fig8.28 fig15.6 fig15.5 fig15.17 fig17.3 fig14.27 fig11.2  figC.20 fig11.4 figC.18 fig7.9 fig7.3 fig18.21 fig1.3 figC.22 fig1.5 fig10.26 fig16.9 fig18.14 fig14.7 fig5.12 fig15.14 fig4.24 fig7.1 fig1.10 fig10.6 fig16.19 fig8.8 fig4.12 fig14.20 figC.14 fig14.1 fig3.5 fig10.15 fig10.23 fig15.11 fig14.12 fig7.4  fig5.13 fig18.18 fig4.8 fig5.4 fig17.5 fig7.13 fig1.7 fig8.30 fig15.33 fig18.10 fig3.11 fig8.16 fig5.5 fig1.4 fig1.8 fig15.18 fig3.1 fig4.21 fig18.22 figC.10 fig8.25 figC.15 fig4.21_wmraw


# 有问题的 
# fig17.21 fig11.16
OTHER =  fig7.11
# 注意_需要把c的后缀给去掉
# 
#  fig20.3 fig13.7 fig17.18 figC.24 
# fig16.16 fig16.20 fig16.17 fig5.11 fig17.28 fig10.9
# fig19.11 fig13.8 fig12.8 fig12.8 fig16.18 fig4.25

# 备份的内容
# fig17.21.c fig7.11.c fig20.3.c fig13.7.c fig17.18.c figC.24.c 
# fig16.16.c fig16.20.c fig16.17.c fig5.11.c fig17.28.c fig10.9.c
# fig19.11.c fig13.8.c fig12.8.c fig12.8.c fig16.18.c fig4.25.c

# 默认目标,编译所有程序
# all: $(PROGS) $(OTHER)
# all: clean git_add $(PROGS)
# all: $(PROGS) $(OTHER)
# all: build_none_main_object
all: clean git_add build_none_main_object
	-@git status


# PROGS: $(PROGS10) $(PROGS20) $(PROGS30) $(PROGS40) $(PROGS50) $(PROGS60) $(PROGS70) $(PROGS80) $(PROGS90) $(PROGSA0) $(PROGSB0) $(PROGSC0) $(PROGSD0) $(PROGSE0)

# 编译 PROGS 列表中的每个程序
# 使用 LIBAPUE 库,编译对应的 .c 文件并链接生成可执行文件
$(PROGS):	$(LIBAPUE)
	$(CC) $(CFLAGS)  $@.c -o $@ $(LDFLAGS) $(LDLIBS)

other: $(OTHER)

# 编译 PROGS 列表中的每个程序
# 使用 LIBAPUE 库,编译对应的 .c 文件并链接生成可执行文件
$(OTHER):	$(LIBAPUE)
	$(CC) $(CFLAGS) $@.c -o $@ $(LDFLAGS) $(LDLIBS)

# 大约79个没有主函数的程序
NONE_MAIN_SRC = fig10.12.c  fig10.8.c   fig12.4.c   fig15.35.c  fig17.14.c  fig17.25.c  fig18.12.c  fig2.16.c  figB.4.c fig10.14.c  fig11.10.c  fig13.1.c   fig15.7.c   fig17.15.c  fig17.26.c  fig18.13.c  fig2.17.c  figC.13.c fig10.18.c  fig11.11.c  fig13.6.c   fig16.10.c  fig17.16.c  fig17.27.c  fig18.15.c  fig3.12.c  figC.16.c fig10.19.c  fig11.12.c  fig13.9.c   fig16.11.c  fig17.17.c  fig17.29.c  fig18.17.c  fig6.2.c   figC.17.c fig10.24.c  fig11.14.c  fig14.24.c  fig16.12.c  fig17.19.c  fig17.2.c   fig18.20.c  fig7.14.c  figC.1.c fig10.25.c  fig11.15.c  fig14.5.c   fig16.22.c  fig17.20.c  fig17.30.c  fig19.10.c  fig8.22.c  figC.23.c fig10.28.c  fig12.11.c  fig14.6.c   fig17.10.c  fig17.22.c  fig17.31.c  fig19.12.c  fig8.5.c   figC.4.c fig10.29.c  fig12.12.c  fig14.9.c   fig17.12.c  fig17.23.c  fig17.8.c   fig19.16.c  figB.1.c fig10.7.c   fig12.13.c  fig15.12.c  fig17.13.c  fig17.24.c  fig17.9.c   fig19.9.c   figB.3.c

NONE_MAIN_OBJECT = fig10.12.o  fig10.8.o   fig12.4.o   fig15.35.o  fig17.14.o  fig17.25.o  fig18.12.o  fig2.16.o  figB.4.o fig10.14.o  fig11.10.o  fig13.1.o   fig15.7.o   fig17.15.o  fig17.26.o  fig18.13.o  fig2.17.o  figC.13.o fig10.18.o  fig11.11.o  fig13.6.o   fig16.10.o  fig17.16.o  fig17.27.o  fig18.15.o  fig3.12.o  figC.16.o fig10.19.o  fig11.12.o  fig13.9.o   fig16.11.o  fig17.17.o  fig17.29.o  fig18.17.o  fig6.2.o   figC.17.o fig10.24.o  fig11.14.o  fig14.24.o  fig16.12.o  fig17.19.o  fig17.2.o   fig18.20.o  fig7.14.o  figC.1.o fig10.25.o  fig11.15.o  fig14.5.o   fig16.22.o  fig17.20.o  fig17.30.o  fig19.10.o  fig8.22.o  figC.23.o fig10.28.o  fig12.11.o  fig14.6.o   fig17.10.o  fig17.22.o  fig17.31.o  fig19.12.o  fig8.5.o   figC.4.o fig10.29.o  fig12.12.o  fig14.9.o   fig17.12.o  fig17.23.o  fig17.8.o   fig19.16.o  figB.1.o fig10.7.o   fig12.13.o  fig15.12.o  fig17.13.o  fig17.24.o  fig17.9.o   fig19.9.o   figB.3.o

NONE_MAIN =   fig10.8   fig12.4     fig17.14  fig18.12  fig2.16  figB.4 fig10.14  fig11.10  fig13.1   fig15.7   fig17.15  fig17.26  fig18.13  fig2.17  figC.13 fig10.18  fig11.11  fig13.6   fig16.10  fig17.16 fig18.15  fig3.12  figC.16 fig10.19  fig11.12  fig13.9   fig16.11  fig17.17  fig17.29  fig18.17  fig6.2   figC.17 fig10.24  fig11.14  fig14.24  fig16.12  fig17.19  fig17.2   fig18.20  fig7.14  figC.1 fig10.25  fig11.15  fig14.5   fig16.22  fig17.20  fig17.30  fig19.10  fig8.22  figC.23 fig10.28  fig12.11  fig14.6   fig17.10    fig17.31  fig19.12  fig8.5    fig10.29  fig12.12  fig14.9   fig17.12  fig17.23  fig17.8   fig19.16  figB.1 fig10.7   fig12.13  fig15.12  fig17.13  fig17.24  fig17.9   fig19.9   figB.3

# 有问题的
# fig10.12 fig15.35   fig17.25 fig17.27 fig17.22 figC.4

build_none_main_object: $(NONE_MAIN)

$(NONE_MAIN):	$(LIBAPUE)
	$(CC) -c $(CFLAGS) $@.c -o $@.o $(LDFLAGS) $(LDLIBS)

# 清理生成的文件
clean:
	-@rm -f $(NONE_MAIN_OBJECT)

# 自动增加git add的文件
git_add:
	-@git add .

# 查找原本_已含有_main函数的c语言源代码
main:
	-@./16_find_c_智谱未来.sh main

# 查找原本_不含有_main函数的c语言源代码
none:
	-@./16_find_c_智谱未来.sh none

# 包含外部定义的 Makefile 规则
include $(ROOT)/Make.libapue.inc
