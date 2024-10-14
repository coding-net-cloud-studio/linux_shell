#! /usr/bin/make -f

# 下面一行的定义_对于echo -e模式正确输出是必须的
SHELL=/bin/bash

# NOTE 内部的_LINUX开发机器上的版本_多一个cf_figlinks
# DIRS = lib intro sockets advio daemons datafiles db environ \
# 	fileio filedir ipc1 ipc2 proc pty relation signals standards \
# 	stdio termios threadctl threads printer exercises \
# 	cf_figlinks cg_figlinks ch_figlinks_batch_ai_comment

# NOTE cloudstudio.net等的版本_少了一个cf_figlinks
DIRS = lib intro sockets advio daemons datafiles db environ \
	fileio filedir ipc1 ipc2 proc pty relation signals standards \
	stdio termios threadctl threads printer exercises \
	cg_figlinks ch_figlinks_batch_ai_comment

# -----------------------------------------------------------------------
# 这里是缺省构建目标_只是_展示帮助信息
default: help

# -----------------------------------------------------------------------

.PHONY : all
all:
	@# wmtag_memo_我修改了这里_开始
	@# 安装了点需要用的软件与库文件
	-@bash 10.wmscript_init_this.sh
	@# wmtag_memo_我修改了这里_结束
	for i in $(DIRS); do \
		(cd $$i && echo "making $$i" && $(MAKE) ) || exit 1; \
	done
	sudo cp lib/libapue.a /usr/lib

.PHONY : init
init:
	-@bash 10.wmscript_init_this.sh

# NOTE 这里给club增加一个独立的初始化快捷方式
# 这里仅仅安装几个构建apue需要用到的软件与库文件
install_lib_for_club:
	-@ [[ -f $$(which cloudstudio) ]] && echo "cloudstudio 已经安装" || echo "cloudstudio 未安装 无需执行_本目标"
	-@ [[ -f $$(which cloudstudio) ]] && apt update && apt install libbsd-dev || echo "cloudstudio 未安装 无需执行_本目标"

# REVIEW build构建目标被如下的文件所引用 .vscode/tasks.json
# REVIEW 本构建目标,也间接的被.vscode/launch.json所引用_参与到单个c语言源代码的调试过程中去
.PHONY : build
build:
	for i in $(DIRS); do \
		(cd $$i && echo "making $$i" && $(MAKE) ) || exit 1; \
	done
	sudo cp lib/libapue.a /usr/lib

.PHONY : clean
clean:
	for i in $(DIRS); do \
		(cd $$i && echo "cleaning $$i" && $(MAKE) clean) || exit 1; \
	done

# -----------------------------------------------------------------------

help:
	@echo -e "\n"
	@echo -e "\033[33m 使用帮助: \033[0m"
	@echo -e "\n"
	@echo -e "\033[33m 欢迎来到_考研_学习笔记_的世界 \033[0m"
	@echo -e "\n"
	@echo -e "特别提示: 先输入make 再选择数字 接着按键盘左上角的Tab键 可以自动完成输入"
	@echo -e "\t  --这样的方式对于初学者最为简洁^_&"
	@echo -e "\t  --资深程序员请忽略本方式"
	@echo -e "\n"
	@echo -e "make 0_check                                   : 判断是否处于cloudstudio工作空间中"
	@echo -e "make 11_install_lib_for_club                   : 本目标只是在club教程中运行_快速安装构建需要用到的几个库"
	@echo -e "make 12_init_for_cloudstudio                   : 本目标只是在cloudstudio工作空间中运行_只需要运行1次就可以了"
	@echo -e "make 23_build_all                              : 开始干活_构建本_考研_学习笔记_的所有源文件"
	@echo -e "make 4_查看构建结果                            : 查看查看_查看是否已经构建出可执行文件"
	@echo -e "make 5_show                                    : 用git clean -xdf -n 查看是否已经构建出可执行文件"
	@echo -e "make 6_show_executable                         : 用find命令查找已经构建出的可执行文件"
	@echo -e "make 7_clean_all                               : 打扫卫生_清除上面构建出来的所有可执行目标文件"
	@echo -e "\n\n"
	@exit 0

# -----------------------------------------------------------------------
# bash - Makefile中的基本if else语句
# https://www.coder.work/article/7542462
.PHONY : 0_check
# REVIEW 0_check 这里是真实的定义
0_check:
	@echo -e "$$(pwd)/Makefile wmtask_[0_check]_目标_被运行\n"
	@if [ $$(which cloudstudio) ]; then \
		echo "位于cloudstudio工作空间中"; \
	else \
		echo "没有处于cloudstudio工作空间中"; \
	fi
	@echo -e "\033[33m 执行本目标完毕,可以执行Next(下一步)目标 \033[0m\n"

# -----------------------------------------------------------------------
.PHONY : 11_install_lib_for_club club
# 下面是别名
11_install_lib_for_club: install_lib_for_club
	-@echo -e "$$(pwd)/Makefile wmtask_[11_install_lib_for_club]_目标_被运行\n"
	-@exit 0

# 下面是别名
club: init
	-@echo -e "$$(pwd)/Makefile wmtask_[11_install_lib_for_club]_目标_被运行\n"
	-@exit 0

# -----------------------------------------------------------------------

.PHONY : 12_init_for_cloudstudio cs
# 下面是别名
2_init_for_cloudstudio: init
	-@echo -e "$$(pwd)/Makefile wmtask_[2_init_for_cloudstudio]_目标_被运行\n"
	-@exit 0

# 下面是别名
cs: init
	-@echo -e "$$(pwd)/Makefile wmtask_[2_init_for_cloudstudio]_目标_被运行\n"
	-@exit 0

# -----------------------------------------------------------------------

.PHONY : 23_build_all
# 下面是别名
23_build_all: build
	-@echo -e "$$(pwd)/Makefile wmtask_[23_build_all]_目标_被运行\n"
	-@exit 0

# -----------------------------------------------------------------------
.PHONY : 4_查看构建结果
4_查看构建结果:
	-@echo -e "$$(pwd)/Makefile wmtask_[4_查看构建结果]_目标_被运行\n"
	-@ [[ ! -f $$(which cloudstudio) ]] && git status || exit 0
	-@exit 0

# -----------------------------------------------------------------------
.PHONY : 5_show
5_show:
	-@echo -e "$$(pwd)/Makefile wmtask_[5_show]_目标_被运行\n"
	-@ [[ ! -f $$(which cloudstudio) ]] && git clean -xdf -n || exit 0
	-@ [[ -f $$(which cloudstudio) ]] && find . -type f -executable | grep -v ".vscode" | grep -v ".wmstudy" | grep -v ".sh" | grep -v ".awk" || exit 0
	-@exit 0

# -----------------------------------------------------------------------
# 展示构建的可执行文件列表_排除了shell脚本文件_排除了.o文件_排除了.so文件_排除了awk文件
.PHONY : 6_show_executable
6_show_executable:
	-@echo -e "$$(pwd)/Makefile wmtask_[6_show_executable]_目标_被运行\n"
	-@find . -type f -executable | grep -v vscode | grep -v ".git" | grep -v ".wmstudy" | grep -v ".sh" | grep -v ".so" | grep -v ".awk" || exit 0
	-@echo -e "\n共构建出了如下数目的可执行文件:"
	-@find . -type f -executable | grep -v vscode | grep -v ".git" | grep -v ".wmstudy" | grep -v ".sh" | grep -v ".so" | grep -v ".awk"| wc -l
	-@exit 0

# -----------------------------------------------------------------------

.PHONY : 7_clean_all
7_clean_all: clean
	-@echo -e "$$(pwd)/Makefile wmtask_[7_clean_all]_目标_被运行\n"
	-@ [[ ! -f $$(which cloudstudio) ]] && git clean -xdf -n || exit 0
	-@exit 0

# 下面是一个备用的语句
# -@ [[ ! -f $$(which cloudstudio) ]] && [[ -d ./cf_figlinks/ ]] && cd cf_figlinks/ && find  . -type f -executable -exec rm {} \;

# -----------------------------------------------------------------------