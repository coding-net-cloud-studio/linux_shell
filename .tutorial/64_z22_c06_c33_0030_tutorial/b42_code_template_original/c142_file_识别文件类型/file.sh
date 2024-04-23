#!/usr/bin/env bash
set -e
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
    my_current_pwd=$(dirname $current_script_full_path)
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

# NOTE 本教程是针对完全的初学者
# NOTE 本教程是针对"泛开发者"
# NOTE 泛开发者是指:在本职工作领域是专业人士,但不以编程作为谋生手段
# NOTE 本教程不适合初级程序员_初级程序员应该去看更深的教程了_不需要看本教程
	# NOTE 向已经是程序员的群体,深度推荐:白明,柴树杉,丁尔男,史斌,郝林,徐秋华,陈剑煜,刘丹冰,廖显东,郭宏志,湖南长沙老郭(Nick),王中阳,申专等老师与专家的专业课程
    # NOTE 向想走程序员等专业路线的推荐:马士兵教育,路飞学城,老男孩教育,马哥教育,尚硅谷,极客大学,传智播客等专业课程
# NOTE 所有曾经编写过300行代码的人士_都不适合看本初级教程了_应该去看其他更深的教程了
# NOTE Makefile中是支持utf-8(unicode)作为目标名称的_我们这样写的目的是给"泛开发者"在初期容易理解实行的
# NOTE 在后继的课程中_我们将取消这些方法_回归到_英文目标名的传统方式
# NOTE 在bash shell script脚本中_针对_泛开发者_我们使用很长的函数名称这样的_非传统方式
# NOTE 在后继的课程中_我们将取消上述_长函数名的_非传统方式_回归常规写法

# ==========================================================
