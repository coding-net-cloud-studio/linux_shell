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
PROGS = fig8.17.c figC.12.c fig8.1.c fig1.9.c fig12.16.c fig7.16.c fig17.4.c fig3.2.c fig11.16.c figC.8.c fig4.22.c fig10.2.c fig5.15.c fig8.3.c fig8.6.c fig10.22.c fig10.10.c fig17.21.c figC.7.c fig15.31.c fig4.16.c fig7.11.c fig8.29.c fig4.23.c fig11.3.c fig10.20.c fig20.3.c fig8.13.c fig15.15.c fig2.14.c fig8.23.c fig18.11.c fig4.9.c fig13.7.c fig8.20.c fig11.5.c fig10.31.c fig8.31.c fig1.6.c fig10.11.c fig6.11.c figC.3.c fig8.12.c fig18.16.c fig10.5.c fig9.12.c fig12.17.c fig15.19.c fig4.3.c fig8.24.c fig8.28.c fig17.18.c fig15.6.c fig15.5.c fig15.17.c fig17.3.c figC.24.c fig14.27.c fig11.2.c fig16.16.c figC.20.c fig11.4.c figC.18.c fig7.9.c fig7.3.c fig18.21.c fig1.3.c figC.22.c fig1.5.c fig10.26.c fig16.9.c fig18.14.c fig14.7.c fig5.12.c fig15.14.c fig4.24.c fig7.1.c fig1.10.c fig10.6.c fig16.19.c fig16.20.c fig8.8.c fig4.12.c fig14.20.c figC.14.c fig14.1.c fig3.5.c fig10.15.c fig10.23.c fig16.17.c fig15.11.c fig14.12.c fig5.11.c fig17.28.c fig10.9.c fig7.4.c fig19.11.c fig5.13.c fig13.8.c fig18.18.c fig4.8.c fig5.4.c fig17.5.c fig7.13.c fig1.7.c fig8.30.c fig15.33.c fig18.10.c fig3.11.c fig8.16.c fig12.8.c fig5.5.c fig1.4.c fig1.8.c fig15.18.c fig3.1.c fig4.21.c fig16.18.c fig4.25.c fig18.22.c figC.10.c fig8.25.c figC.15.c

# 默认目标,编译所有程序
all: $(PROGS)


# PROGS: $(PROGS10) $(PROGS20) $(PROGS30) $(PROGS40) $(PROGS50) $(PROGS60) $(PROGS70) $(PROGS80) $(PROGS90) $(PROGSA0) $(PROGSB0) $(PROGSC0) $(PROGSD0) $(PROGSE0)

# 编译 PROGS 列表中的每个程序
# 使用 LIBAPUE 库,编译对应的 .c 文件并链接生成可执行文件
$(PROGS):	$(LIBAPUE)
	$(CC) $(CFLAGS) $@.c -o $@ $(LDFLAGS) $(LDLIBS)

# 清理生成的文件
clean:
	rm -f $(PROGS)

# 包含外部定义的 Makefile 规则
include $(ROOT)/Make.libapue.inc
