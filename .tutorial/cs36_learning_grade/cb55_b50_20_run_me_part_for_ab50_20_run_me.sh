#!/bin/bash


# ====================================================================

# NOTE 相当于_真的要停止60秒_给操作人员看看_log
# pause_60_second(){
# 	if read -t 60 -p "暂停60秒,按回车继续运行: "db50_20_run_scriptdb50_20_run_scriptdb50_20_run_scriptdb50_20_run_scriptdb50_20_run_scriptdb50_20_run_scriptdb50_20_run_scriptdb50_20_run_me_entry_script.shdb50_20_run_me_entry_script.shcs_club_tutorail_learning_grade_script_pathcs_club_tutorail_learning_grade_script_path
# 	if read -t 60 -p "暂停60秒,按回车继续运行: "db50_20_run_scriptdb50_20_run_scriptdb50_20_run_scriptdb50_20_run_scriptdb50_20_run_scriptdb50_20_run_scriptdb50_20_run_scriptdb50_20_run_me_entry_script.shdb50_20_run_me_entry_script.shcs_club_tutorail_learning_grade_script_pathcs_club_tutorail_learning_grade_script_pathcs_club_tutorail_learning_grade_script_pathab50_20_run_me_entry_script.shab50_20_run_me_entry_script.shab50_20_run_me_entry_script.shab50_20_run_me_entry_script.sh
# 	then
# 		# echo "hello $REPLY,welcome to cheng du"
# 		printf "\n";
# 	else
# 		# echo "sorry, Output timeout, please execute the command again !"
# 		printf "\n时间已到,继续运行\n";
# 	fi
# }

# NOTE 相当于_不做调试
pause_60_second(){
echo "啥都不做"
return 0
}

wmlog_echo(){

    if [[ $wmvar_show_echo_message == "展示" ]]; then
        if [[ -z $1 ]]; then
            # echo "没有传入_内容_参数"
            # echo "无法展示"
            # echo "退出_展示_脚本"
            return 1
        else
            # echo "传入了_内容_参数"
            echo -e $1
            return 0
        fi
    fi

    return 0
}

# ----------------------------------------------------------------------------------------
# NOTE 这个函数通过模拟每个字符的输出和等待,来模拟打字效果.你可以通过调整typing_speed变量的值来改变打字速度.
type_effect() {
    # 检查文件是否存在
    if [ ! -f "$1" ]; then
        echo "File does not exist!"
        return 1
    fi

    # 设置打字速度,单位是字符每秒
    local typing_speed=10

    # 计算每个字符之间需要等待的时间(以秒为单位)
    # 使用bash的内建命令进行计算,不需要bc
    # 计算每个字符之间需要等待的时间.我们使用awk命令来执行浮点数除法,因为bash本身不支持浮点数运算.
    # 1.0 / $typing_speed计算每个字符之间的等待时间,结果赋值给局部变量delay.
    local delay=$(awk "BEGIN {print 1.0 /$typing_speed}")

    # 逐行读取文件内容
    # while IFS= read -r line || [[ -n "$line" ]]; do:这个循环用于逐行读取文件内容.
    # IFS= read -r line是一个常见的bash idiom,用于读取文件的每一行,同时保持行的完整性(-r选项防止反斜杠转义).
    # || [[ -n "$line" ]]确保即使在读取到文件末尾时,如果最后一行是非空行,也会执行循环体.
    while IFS= read -r line || [[ -n "$line" ]]; do
        # 逐字符输出行内容
        for (( i=0; i<${#line}; i++ )); do
            echo -n "${line:$i:1}"
            sleep $delay
        done
        echo "" # 输出换行符,移动到下一行
    done < "$1"

    return 0
}

# NOTE 下面是上述函数的注释
# for (( i=0; i<${#line}; i++ )); do:这个循环用于逐个字符地输出每一行.${#line}是bash的参数扩展,用于获取line变量的长度(即字符数).
# echo -n "${line:$i:1}":这个命令输出当前字符.${line:$i:1}是bash的参数扩展,用于获取line变量中从索引i开始的第一个字符.-n选项告诉echo不要在输出后添加换行符.
# sleep $delay:这个命令使脚本暂停delay秒,模拟打字速度.
# echo "":在输出完一行所有字符后,输出一个空行,模拟换行.
# done < "$1":这个命令将文件描述符0(标准输入)重定向到文件"$1",这样read命令就会从文件中读取内容,而不是从键盘.

# 使用示例
# type_effect "path/to/your/file.txt"
# type_effect "sg73_菩萨蛮_书江西造口壁_辛弃疾.txt"
# type_effect "/etc/passwd"

# ====================================================================

###NOTE wmtag_memo_替换这里_开始###
c014_ls_run_me(){
    echo -e "\n这是_缺省的_run_me_bash_函数_2010."
    export the_run_me_demo_script_name=$(basename ${BASH_SOURCE[0]})
    echo -e "运行在****$(pwd)/${the_run_me_demo_script_name}脚本的****${FUNCNAME}()函数中****第${LINENO}行\n"



    unset the_run_me_demo_script_name
    return 0
}

###NOTE wmtag_memo_替换这里_结束###


#-------------------------------------------------------


###NOTE wmtag_memo_替换这里_开始###
c018_pwd_run_me(){
    echo -e "\n这是_缺省的_run_me_bash_函数_2010."
    export the_run_me_demo_script_name=$(basename ${BASH_SOURCE[0]})
    echo -e "运行在****$(pwd)/${the_run_me_demo_script_name}脚本的****${FUNCNAME}()函数中****第${LINENO}行\n"



    unset the_run_me_demo_script_name
    return 0
}

###NOTE wmtag_memo_替换这里_结束###


#-------------------------------------------------------


###NOTE wmtag_memo_替换这里_开始###
c022_cd_run_me(){
    echo -e "\n这是_缺省的_run_me_bash_函数_2010."
    export the_run_me_demo_script_name=$(basename ${BASH_SOURCE[0]})
    echo -e "运行在****$(pwd)/${the_run_me_demo_script_name}脚本的****${FUNCNAME}()函数中****第${LINENO}行\n"



    unset the_run_me_demo_script_name
    return 0
}

###NOTE wmtag_memo_替换这里_结束###


#-------------------------------------------------------


###NOTE wmtag_memo_替换这里_开始###
c026_id_run_me(){
    echo -e "\n这是_缺省的_run_me_bash_函数_2010."
    export the_run_me_demo_script_name=$(basename ${BASH_SOURCE[0]})
    echo -e "运行在****$(pwd)/${the_run_me_demo_script_name}脚本的****${FUNCNAME}()函数中****第${LINENO}行\n"



    unset the_run_me_demo_script_name
    return 0
}

###NOTE wmtag_memo_替换这里_结束###


#-------------------------------------------------------


###NOTE wmtag_memo_替换这里_开始###
c030_date_run_me(){
    echo -e "\n这是_缺省的_run_me_bash_函数_2010."
    export the_run_me_demo_script_name=$(basename ${BASH_SOURCE[0]})
    echo -e "运行在****$(pwd)/${the_run_me_demo_script_name}脚本的****${FUNCNAME}()函数中****第${LINENO}行\n"



    unset the_run_me_demo_script_name
    return 0
}

###NOTE wmtag_memo_替换这里_结束###


#-------------------------------------------------------


###NOTE wmtag_memo_替换这里_开始###
c034_cal_run_me(){
    echo -e "\n这是_缺省的_run_me_bash_函数_2010."
    export the_run_me_demo_script_name=$(basename ${BASH_SOURCE[0]})
    echo -e "运行在****$(pwd)/${the_run_me_demo_script_name}脚本的****${FUNCNAME}()函数中****第${LINENO}行\n"



    unset the_run_me_demo_script_name
    return 0
}

###NOTE wmtag_memo_替换这里_结束###


#-------------------------------------------------------


###NOTE wmtag_memo_替换这里_开始###
c042_which_run_me(){
    echo -e "\n这是_缺省的_run_me_bash_函数_2010."
    export the_run_me_demo_script_name=$(basename ${BASH_SOURCE[0]})
    echo -e "运行在****$(pwd)/${the_run_me_demo_script_name}脚本的****${FUNCNAME}()函数中****第${LINENO}行\n"



    unset the_run_me_demo_script_name
    return 0
}

###NOTE wmtag_memo_替换这里_结束###


#-------------------------------------------------------


###NOTE wmtag_memo_替换这里_开始###
c046_history_run_me(){
    echo -e "\n这是_缺省的_run_me_bash_函数_2010."
    export the_run_me_demo_script_name=$(basename ${BASH_SOURCE[0]})
    echo -e "运行在****$(pwd)/${the_run_me_demo_script_name}脚本的****${FUNCNAME}()函数中****第${LINENO}行\n"



    unset the_run_me_demo_script_name
    return 0
}

###NOTE wmtag_memo_替换这里_结束###


#-------------------------------------------------------


###NOTE wmtag_memo_替换这里_开始###
c050_touch_run_me(){
    echo -e "\n这是_缺省的_run_me_bash_函数_2010."
    export the_run_me_demo_script_name=$(basename ${BASH_SOURCE[0]})
    echo -e "运行在****$(pwd)/${the_run_me_demo_script_name}脚本的****${FUNCNAME}()函数中****第${LINENO}行\n"



    unset the_run_me_demo_script_name
    return 0
}

###NOTE wmtag_memo_替换这里_结束###


#-------------------------------------------------------


###NOTE wmtag_memo_替换这里_开始###
c054_mkdir_run_me(){
    echo -e "\n这是_缺省的_run_me_bash_函数_2010."
    export the_run_me_demo_script_name=$(basename ${BASH_SOURCE[0]})
    echo -e "运行在****$(pwd)/${the_run_me_demo_script_name}脚本的****${FUNCNAME}()函数中****第${LINENO}行\n"



    unset the_run_me_demo_script_name
    return 0
}

###NOTE wmtag_memo_替换这里_结束###


#-------------------------------------------------------


###NOTE wmtag_memo_替换这里_开始###
c058_rmdir_run_me(){
    echo -e "\n这是_缺省的_run_me_bash_函数_2010."
    export the_run_me_demo_script_name=$(basename ${BASH_SOURCE[0]})
    echo -e "运行在****$(pwd)/${the_run_me_demo_script_name}脚本的****${FUNCNAME}()函数中****第${LINENO}行\n"



    unset the_run_me_demo_script_name
    return 0
}

###NOTE wmtag_memo_替换这里_结束###


#-------------------------------------------------------


###NOTE wmtag_memo_替换这里_开始###
c062_mv_run_me(){
    echo -e "\n这是_缺省的_run_me_bash_函数_2010."
    export the_run_me_demo_script_name=$(basename ${BASH_SOURCE[0]})
    echo -e "运行在****$(pwd)/${the_run_me_demo_script_name}脚本的****${FUNCNAME}()函数中****第${LINENO}行\n"



    unset the_run_me_demo_script_name
    return 0
}

###NOTE wmtag_memo_替换这里_结束###


#-------------------------------------------------------


###NOTE wmtag_memo_替换这里_开始###
c066_rm_run_me(){
    echo -e "\n这是_缺省的_run_me_bash_函数_2010."
    export the_run_me_demo_script_name=$(basename ${BASH_SOURCE[0]})
    echo -e "运行在****$(pwd)/${the_run_me_demo_script_name}脚本的****${FUNCNAME}()函数中****第${LINENO}行\n"



    unset the_run_me_demo_script_name
    return 0
}

###NOTE wmtag_memo_替换这里_结束###


#-------------------------------------------------------


###NOTE wmtag_memo_替换这里_开始###
c070_man_run_me(){
    echo -e "\n这是_缺省的_run_me_bash_函数_2010."
    export the_run_me_demo_script_name=$(basename ${BASH_SOURCE[0]})
    echo -e "运行在****$(pwd)/${the_run_me_demo_script_name}脚本的****${FUNCNAME}()函数中****第${LINENO}行\n"



    unset the_run_me_demo_script_name
    return 0
}

###NOTE wmtag_memo_替换这里_结束###


#-------------------------------------------------------


###NOTE wmtag_memo_替换这里_开始###
c074_cp_run_me(){
    echo -e "\n这是_缺省的_run_me_bash_函数_2010."
    export the_run_me_demo_script_name=$(basename ${BASH_SOURCE[0]})
    echo -e "运行在****$(pwd)/${the_run_me_demo_script_name}脚本的****${FUNCNAME}()函数中****第${LINENO}行\n"



    unset the_run_me_demo_script_name
    return 0
}

###NOTE wmtag_memo_替换这里_结束###


#-------------------------------------------------------


###NOTE wmtag_memo_替换这里_开始###
c078_cat_run_me(){
    echo -e "\n这是_缺省的_run_me_bash_函数_2010."
    export the_run_me_demo_script_name=$(basename ${BASH_SOURCE[0]})
    echo -e "运行在****$(pwd)/${the_run_me_demo_script_name}脚本的****${FUNCNAME}()函数中****第${LINENO}行\n"



    unset the_run_me_demo_script_name
    return 0
}

###NOTE wmtag_memo_替换这里_结束###


#-------------------------------------------------------


###NOTE wmtag_memo_替换这里_开始###
c082_nl_run_me(){
    echo -e "\n这是_缺省的_run_me_bash_函数_2010."
    export the_run_me_demo_script_name=$(basename ${BASH_SOURCE[0]})
    echo -e "运行在****$(pwd)/${the_run_me_demo_script_name}脚本的****${FUNCNAME}()函数中****第${LINENO}行\n"



    unset the_run_me_demo_script_name
    return 0
}

###NOTE wmtag_memo_替换这里_结束###


#-------------------------------------------------------


###NOTE wmtag_memo_替换这里_开始###
c086_more_run_me(){
    echo -e "\n这是_缺省的_run_me_bash_函数_2010."
    export the_run_me_demo_script_name=$(basename ${BASH_SOURCE[0]})
    echo -e "运行在****$(pwd)/${the_run_me_demo_script_name}脚本的****${FUNCNAME}()函数中****第${LINENO}行\n"



    unset the_run_me_demo_script_name
    return 0
}

###NOTE wmtag_memo_替换这里_结束###


#-------------------------------------------------------


###NOTE wmtag_memo_替换这里_开始###
c090_echo_run_me(){
    echo -e "\n这是_缺省的_run_me_bash_函数_2010."
    export the_run_me_demo_script_name=$(basename ${BASH_SOURCE[0]})
    echo -e "运行在****$(pwd)/${the_run_me_demo_script_name}脚本的****${FUNCNAME}()函数中****第${LINENO}行\n"



    unset the_run_me_demo_script_name
    return 0
}

###NOTE wmtag_memo_替换这里_结束###


#-------------------------------------------------------


###NOTE wmtag_memo_替换这里_开始###
c094_wc_run_me(){
    echo -e "\n这是_缺省的_run_me_bash_函数_2010."
    export the_run_me_demo_script_name=$(basename ${BASH_SOURCE[0]})
    echo -e "运行在****$(pwd)/${the_run_me_demo_script_name}脚本的****${FUNCNAME}()函数中****第${LINENO}行\n"



    unset the_run_me_demo_script_name
    return 0
}

###NOTE wmtag_memo_替换这里_结束###


#-------------------------------------------------------


###NOTE wmtag_memo_替换这里_开始###
c098_sed_run_me(){
    echo -e "\n这是_缺省的_run_me_bash_函数_2010."
    export the_run_me_demo_script_name=$(basename ${BASH_SOURCE[0]})
    echo -e "运行在****$(pwd)/${the_run_me_demo_script_name}脚本的****${FUNCNAME}()函数中****第${LINENO}行\n"



    unset the_run_me_demo_script_name
    return 0
}

###NOTE wmtag_memo_替换这里_结束###


#-------------------------------------------------------


###NOTE wmtag_memo_替换这里_开始###
c102_head_run_me(){
    echo -e "\n这是_缺省的_run_me_bash_函数_2010."
    export the_run_me_demo_script_name=$(basename ${BASH_SOURCE[0]})
    echo -e "运行在****$(pwd)/${the_run_me_demo_script_name}脚本的****${FUNCNAME}()函数中****第${LINENO}行\n"



    unset the_run_me_demo_script_name
    return 0
}

###NOTE wmtag_memo_替换这里_结束###


#-------------------------------------------------------


###NOTE wmtag_memo_替换这里_开始###
c106_tail_run_me(){
    echo -e "\n这是_缺省的_run_me_bash_函数_2010."
    export the_run_me_demo_script_name=$(basename ${BASH_SOURCE[0]})
    echo -e "运行在****$(pwd)/${the_run_me_demo_script_name}脚本的****${FUNCNAME}()函数中****第${LINENO}行\n"



    unset the_run_me_demo_script_name
    return 0
}

###NOTE wmtag_memo_替换这里_结束###


#-------------------------------------------------------


###NOTE wmtag_memo_替换这里_开始###
c110_cut_run_me(){
    echo -e "\n这是_缺省的_run_me_bash_函数_2010."
    export the_run_me_demo_script_name=$(basename ${BASH_SOURCE[0]})
    echo -e "运行在****$(pwd)/${the_run_me_demo_script_name}脚本的****${FUNCNAME}()函数中****第${LINENO}行\n"



    unset the_run_me_demo_script_name
    return 0
}

###NOTE wmtag_memo_替换这里_结束###


#-------------------------------------------------------


###NOTE wmtag_memo_替换这里_开始###
c114_df_run_me(){
    echo -e "\n这是_缺省的_run_me_bash_函数_2010."
    export the_run_me_demo_script_name=$(basename ${BASH_SOURCE[0]})
    echo -e "运行在****$(pwd)/${the_run_me_demo_script_name}脚本的****${FUNCNAME}()函数中****第${LINENO}行\n"



    unset the_run_me_demo_script_name
    return 0
}

###NOTE wmtag_memo_替换这里_结束###


#-------------------------------------------------------


###NOTE wmtag_memo_替换这里_开始###
c118_du_run_me(){
    echo -e "\n这是_缺省的_run_me_bash_函数_2010."
    export the_run_me_demo_script_name=$(basename ${BASH_SOURCE[0]})
    echo -e "运行在****$(pwd)/${the_run_me_demo_script_name}脚本的****${FUNCNAME}()函数中****第${LINENO}行\n"



    unset the_run_me_demo_script_name
    return 0
}

###NOTE wmtag_memo_替换这里_结束###


#-------------------------------------------------------


###NOTE wmtag_memo_替换这里_开始###
c122_w_run_me(){
    echo -e "\n这是_缺省的_run_me_bash_函数_2010."
    export the_run_me_demo_script_name=$(basename ${BASH_SOURCE[0]})
    echo -e "运行在****$(pwd)/${the_run_me_demo_script_name}脚本的****${FUNCNAME}()函数中****第${LINENO}行\n"



    unset the_run_me_demo_script_name
    return 0
}

###NOTE wmtag_memo_替换这里_结束###


#-------------------------------------------------------


###NOTE wmtag_memo_替换这里_开始###
c126_apt_get_run_me(){
    echo -e "\n这是_缺省的_run_me_bash_函数_2010."
    export the_run_me_demo_script_name=$(basename ${BASH_SOURCE[0]})
    echo -e "运行在****$(pwd)/${the_run_me_demo_script_name}脚本的****${FUNCNAME}()函数中****第${LINENO}行\n"



    unset the_run_me_demo_script_name
    return 0
}

###NOTE wmtag_memo_替换这里_结束###


#-------------------------------------------------------


###NOTE wmtag_memo_替换这里_开始###
c130_tree_run_me(){
    echo -e "\n这是_缺省的_run_me_bash_函数_2010."
    export the_run_me_demo_script_name=$(basename ${BASH_SOURCE[0]})
    echo -e "运行在****$(pwd)/${the_run_me_demo_script_name}脚本的****${FUNCNAME}()函数中****第${LINENO}行\n"



    unset the_run_me_demo_script_name
    return 0
}

###NOTE wmtag_memo_替换这里_结束###


#-------------------------------------------------------


###NOTE wmtag_memo_替换这里_开始###
c134_whereis_run_me(){
    echo -e "\n这是_缺省的_run_me_bash_函数_2010."
    export the_run_me_demo_script_name=$(basename ${BASH_SOURCE[0]})
    echo -e "运行在****$(pwd)/${the_run_me_demo_script_name}脚本的****${FUNCNAME}()函数中****第${LINENO}行\n"



    unset the_run_me_demo_script_name
    return 0
}

###NOTE wmtag_memo_替换这里_结束###


#-------------------------------------------------------


###NOTE wmtag_memo_替换这里_开始###
c138_find_run_me(){
    echo -e "\n这是_缺省的_run_me_bash_函数_2010."
    export the_run_me_demo_script_name=$(basename ${BASH_SOURCE[0]})
    echo -e "运行在****$(pwd)/${the_run_me_demo_script_name}脚本的****${FUNCNAME}()函数中****第${LINENO}行\n"



    unset the_run_me_demo_script_name
    return 0
}

###NOTE wmtag_memo_替换这里_结束###


#-------------------------------------------------------


###NOTE wmtag_memo_替换这里_开始###
c142_file_run_me(){
    echo -e "\n这是_缺省的_run_me_bash_函数_2010."
    export the_run_me_demo_script_name=$(basename ${BASH_SOURCE[0]})
    echo -e "运行在****$(pwd)/${the_run_me_demo_script_name}脚本的****${FUNCNAME}()函数中****第${LINENO}行\n"



    unset the_run_me_demo_script_name
    return 0
}

###NOTE wmtag_memo_替换这里_结束###


#-------------------------------------------------------


###NOTE wmtag_memo_替换这里_开始###
c146_ps_run_me(){
    echo -e "\n这是_缺省的_run_me_bash_函数_2010."
    export the_run_me_demo_script_name=$(basename ${BASH_SOURCE[0]})
    echo -e "运行在****$(pwd)/${the_run_me_demo_script_name}脚本的****${FUNCNAME}()函数中****第${LINENO}行\n"



    unset the_run_me_demo_script_name
    return 0
}

###NOTE wmtag_memo_替换这里_结束###


#-------------------------------------------------------


###NOTE wmtag_memo_替换这里_开始###
c150_sort_run_me(){
    echo -e "\n这是_缺省的_run_me_bash_函数_2010."
    export the_run_me_demo_script_name=$(basename ${BASH_SOURCE[0]})
    echo -e "运行在****$(pwd)/${the_run_me_demo_script_name}脚本的****${FUNCNAME}()函数中****第${LINENO}行\n"



    unset the_run_me_demo_script_name
    return 0
}

###NOTE wmtag_memo_替换这里_结束###


#-------------------------------------------------------


###NOTE wmtag_memo_替换这里_开始###
c154_kill_run_me(){
    echo -e "\n这是_缺省的_run_me_bash_函数_2010."
    export the_run_me_demo_script_name=$(basename ${BASH_SOURCE[0]})
    echo -e "运行在****$(pwd)/${the_run_me_demo_script_name}脚本的****${FUNCNAME}()函数中****第${LINENO}行\n"



    unset the_run_me_demo_script_name
    return 0
}

###NOTE wmtag_memo_替换这里_结束###


#-------------------------------------------------------


###NOTE wmtag_memo_替换这里_开始###
c158_ln_run_me(){
    echo -e "\n这是_缺省的_run_me_bash_函数_2010."
    export the_run_me_demo_script_name=$(basename ${BASH_SOURCE[0]})
    echo -e "运行在****$(pwd)/${the_run_me_demo_script_name}脚本的****${FUNCNAME}()函数中****第${LINENO}行\n"



    unset the_run_me_demo_script_name
    return 0
}

###NOTE wmtag_memo_替换这里_结束###


#-------------------------------------------------------


###NOTE wmtag_memo_替换这里_开始###
c162_wget_run_me(){
    echo -e "\n这是_缺省的_run_me_bash_函数_2010."
    export the_run_me_demo_script_name=$(basename ${BASH_SOURCE[0]})
    echo -e "运行在****$(pwd)/${the_run_me_demo_script_name}脚本的****${FUNCNAME}()函数中****第${LINENO}行\n"



    unset the_run_me_demo_script_name
    return 0
}

###NOTE wmtag_memo_替换这里_结束###


#-------------------------------------------------------


###NOTE wmtag_memo_替换这里_开始###
c166_tar_run_me(){
    echo -e "\n这是_缺省的_run_me_bash_函数_2010."
    export the_run_me_demo_script_name=$(basename ${BASH_SOURCE[0]})
    echo -e "运行在****$(pwd)/${the_run_me_demo_script_name}脚本的****${FUNCNAME}()函数中****第${LINENO}行\n"



    unset the_run_me_demo_script_name
    return 0
}

###NOTE wmtag_memo_替换这里_结束###


#-------------------------------------------------------


###NOTE wmtag_memo_替换这里_开始###
c170_md5sum_run_me(){
    echo -e "\n这是_缺省的_run_me_bash_函数_2010."
    export the_run_me_demo_script_name=$(basename ${BASH_SOURCE[0]})
    echo -e "运行在****$(pwd)/${the_run_me_demo_script_name}脚本的****${FUNCNAME}()函数中****第${LINENO}行\n"



    unset the_run_me_demo_script_name
    return 0
}

###NOTE wmtag_memo_替换这里_结束###


#-------------------------------------------------------


###NOTE wmtag_memo_替换这里_开始###
c174_xargs_run_me(){
    echo -e "\n这是_缺省的_run_me_bash_函数_2010."
    export the_run_me_demo_script_name=$(basename ${BASH_SOURCE[0]})
    echo -e "运行在****$(pwd)/${the_run_me_demo_script_name}脚本的****${FUNCNAME}()函数中****第${LINENO}行\n"



    unset the_run_me_demo_script_name
    return 0
}

###NOTE wmtag_memo_替换这里_结束###


#-------------------------------------------------------


main(){
    echo "main_函数_我啥都不做_怕出错"

    echo -e "\n这是_缺省的_run_me_bash_函数_2010."
    export the_run_me_demo_script_name=$(basename ${BASH_SOURCE[0]})
    echo -e "运行在****$(pwd)/${the_run_me_demo_script_name}脚本的****${FUNCNAME}()函数中****第${LINENO}行\n"

    unset the_run_me_demo_script_name
    return 0
}

# ====================================================================

[ -z $1 ] && eval main || eval $1 $*

