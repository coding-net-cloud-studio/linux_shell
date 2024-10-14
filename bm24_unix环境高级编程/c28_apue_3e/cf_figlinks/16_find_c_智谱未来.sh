#!/bin/bash

# 下面是两个bash函数,第一个函数`find_main_files`会找出所有包含`main`函数的C源文件,第二个函数`find_no_main_files`会找出所有不包含`main`函数的C源文件.
# ```bash

# 函数:找到包含main函数的C源文件
find_main_files() {
    local directory=$1
    grep -rl --include=\*.c 'main(' "$directory"
    return 0
}

# 如下是find_main_files函数的快捷方式
main(){
    find_main_files .
    return 0
}


# ---------------------------------------------------------------------------------------

# 用法:comm [选项]... 文件1 文件2
# 逐行比较已排序的文件 <文件1> 和 <文件2>.

# 如果 <文件1> 或 <文件2> 是 "-"(不能都是 "-"),则从标准输入读取.

# 如果不附带选项,程序会生成三列输出.第一列包含 <文件1> 特有的行,
# 第二列包含 <文件2> 特有的行,而第三列包含两个文件共有的行.

#   -1		不输出文件1 特有的行
#   -2		不输出文件2 特有的行
#   -3		不输出两个文件共有的行

#   --check-order			检查输入是否被正确排序,即使所有输入行均成对
#   --nocheck-order		不检查输入是否被正确排序
#   --output-delimiter=STR	依照STR 分列
#   --total           输出一份摘要信息
#   -z, --zero-terminated    以 NUL 空字符而非换行符作为行分隔符
#       --help		显示此帮助信息并退出
#       --version		显示版本信息并退出

# 注意,比较操作遵从 "LC_COLLATE" 所指定的规则.

# 示例:
#   comm -12 文件1 文件2  只打印在 <文件1> 和 <文件2> 中都有的行.
#   comm -3  文件1 文件2  打印在 <文件1> 中有,而 <文件2> 中没有的行.反之亦然.

# GNU coreutils 在线帮助:<https://www.gnu.org/software/coreutils/>
# 请向 <http://translationproject.org/team/zh_CN.html> 报告任何翻译错误
# 完整文档 <https://www.gnu.org/software/coreutils/comm>
# 或者在本地使用:info '(coreutils) comm invocation'


# 函数:找到不包含main函数的C源文件
find_no_main_files() {
    local directory=$1
    local main_files=$(find_main_files "$directory")
    local all_c_files=$(find "$directory" -name "*.c")
    # 使用comm命令比较所有C文件和包含main的文件,找到不同的部分
    comm -23 <(echo "$all_c_files" | sort) <(echo "$main_files" | sort)
    return 0
}

# 如下是find_no_main_files函数的快捷方式
none(){
    find_no_main_files .
    return 0
}

# ---------------------------------------------------------------------------------------

# 使用示例
# 找到包含main函数的文件
# 大约126个
# find_main_files .


# 找到不包含main函数的文件
# 大约79个
# find_no_main_files .

# ```
# 将上面的脚本保存到一个文件中,比如`find_main.sh`,然后在终端中运行它.你可以通过调用`find_main_files`函数和`find_no_main_files`函数来分别找到包含和不包含`main`函数的文件.
# 注意:这个脚本假设`main`函数是作为函数定义的一部分出现的,并且紧跟着`(`字符.如果`main`函数的声明或定义有不同的格式,可能需要调整`grep`的正则表达式来正确匹配.


# ---------------------------------------------------------------------------------------

# 安装各种软件,设置各种环境.
# 需要运行比较长的时间.

f94_2828_30_main(){
    return 0
}

# ---------------------------------------------------------------------------------------

c11_28_wmtask_runonce_set_env(){
    # 下面调用了主函数
    # 在主函数中安装了各种软件
    # 在主函数中执行,需要很多的时间
    # 在cloudstudio环境下
    # 全新的cloudstudio工作空间中_不存在锁文件的时候_争取只是执行一次
    # 再次与多次被运行的时候_如果发现有锁_就不再执行_初始化主函数
    # [[ -f $(which cloudstudio) ]] && [[ ! -f "l30_初始化已经被运行过一次了_我假装自己是一把锁_阻止再次运行-wmgitignore.md" ]] && f94_2828_30_main
    echo "我啥都不做_因为我害怕出错"
    return 0
}

# ---------------------------------------------------------------------------------------

# eval f94_2828_30_main
# 下面是_正式_的入口
# make
# 或
# make install
[ -z "$1" ] && eval c11_28_wmtask_runonce_set_env || eval $1 $*