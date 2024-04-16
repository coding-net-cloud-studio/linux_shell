#!/usr/bin/env bash
# 1_动手_是学些编码的最简单的方式
# 2_可以从_左面的_教学手册中_拷贝_部分内容_到这里
# 3_然后_在本编辑器中_点击鼠标右键_选择_"Run Code"的_菜单选项
# 4_或者_按组合键_Alt+Ctrl+N 执行一下_看看效果
clear && echo -e "\n\n" # NOTE 清理一下屏幕内容
# NOTE 5_在下方_编写你的_练习_代码_你的代码从下面这里开始




#NOTE 6_打印点_信息
echo -e "\n\n你好,中国人,你要自信!\n恭喜你啊!\n你的_命令_已经被_成功地_执行!\n加油啊,你正在走入星辰大海!\n\n"











# ==========================================================
# REVIEW 下面是注释_不会被执行
# 15_grasspy.cn
# 16_草蟒极速 QQ 交流群:760167264 风里雨里我们在这个QQ群里等你 快来吧,大家一起快乐的聊天吧!
# 来聊聊,中西如何互为借鉴,砥砺前行,中西合璧,走进新时代,实践全球化的命运共同体

# NOTE 多种_运行_方法_下面的每种方法_都是独立进行的
# d01_点击右上角的_白色三角形_就可以_执行简单的_草蟒中文grasspy版本的python
# d02_在本编辑器中_点击鼠标右键_选择_"Run Code"的_菜单选项
# d03_在本编辑器中_按组合键_Alt+Ctrl+N
# d04_cloudstudio的终端中,直接运行类似如下的命令,直接执行.例如 python 18_打开文件.py
# ==========================================================
# REVIEW 下面是调用评分脚本的区域
pause_60_second(){
	if read -t 60 -p "暂停60秒,按回车继续运行: "
	then
		# echo "hello $REPLY,welcome to cheng du"
		printf "\n";
	else
		# echo "sorry, Output timeout, please execute the command again !"
		printf "\n时间已到,继续运行\n";
	fi
}


# NOTE 被下面的_评分_grade()函数所调用
l32_call_default_cs_club_learning_grade_entry_shell_script(){

    # NOTE 获取_本_脚本的完整路径
    current_script_full_path=$(realpath "$0")

    # NOTE 找到cloudstudio的_club课程_tutorial目录_所在的位置
    my_current_pwd=$(pwd)
    # echo $my_current_pwd

    # NOTE 用".tutorial"当分隔符去切分_上面的路径
    cs_club_tutorail_parent_path="${my_current_pwd%/.tutorial*}"
    cs_club_tutorail_path=${cs_club_tutorail_parent_path}/.tutorial
    # echo $cs_club_tutorail_path

    # NOTE 缺省的_判分_入口
    cs_club_tutorail_learning_grade_script_path=${cs_club_tutorail_path}/cs36_learning_grade/aa10_grade_script.sh

    # echo "l32_28_缺省的grade评分脚本路径---> ${cs_club_tutorail_learning_grade_script_path}"

    # NOTE 缺省的_奖牌_目录
    cs_club_tutorail_learning_medal_path=${cs_club_tutorail_path}/cs36_learning_grade/dm42_learning_medal

    # echo "l32_36_缺省的_奖牌_路径---> ${cs_club_tutorail_learning_medal_path}"

    if [[ -f ${cs_club_tutorail_learning_grade_script_path} ]]; then
        chmod +x ${cs_club_tutorail_learning_grade_script_path}
        # REVIEW 最为重要的一步_运行评分体系的_总入口脚本
        # NOTE 把当前的_脚本的全路径_作为调用参数_传递给改_grade_评分脚本



        # echo -e "\n准备执行的命令如下_先把命令打印出来看看效果:\n"
        # echo "source ${cs_club_tutorail_learning_grade_script_path} ${current_script_full_path}"
        # echo -e "\n"

        # pause_60_second

        # echo "准备_执行_判分_的_入口_脚本_aa10_grade_script.sh"
        source ${cs_club_tutorail_learning_grade_script_path} ${current_script_full_path}
        # echo "执行_完毕_判分_的_入口_脚本_aa10_grade_script.sh"
    else
        echo "没有找到_判分_的_入口_脚本_aa10_grade_script.sh"
        echo "请在_cloudstudio_的_club课程_tutorial目录_下_创建_aa10_grade_script.sh"
        echo "然后_再_运行_这个_脚本"
        echo -e "\n"
    fi

    return 0
}

# --------------------------------------------------------

# NOTE 定义_评分_grade_函数_接受一个参数
grade() {

    # 获取_本_脚本的完整路径
    script_full_path=$(realpath "$0")

    # echo ${script_full_path}

    # 实际调用的函数_如下
    l32_call_default_cs_club_learning_grade_entry_shell_script

    return 0
}

# REVIEW 评分
grade

# ==========================================================
