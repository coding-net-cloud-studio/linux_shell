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

