#!/usr/bin/env bash

export my_version=1100

# export wmvar_show_echo_message="展示"
export wmvar_show_echo_message="不展示"

# export wmvar_demonstrate_how_to_play="展示"
export wmvar_demonstrate_how_to_play="不展示"


# ========================================================================================

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


# ========================================================================================

# echo -e "\n已经进入_评分_grade_脚本_中_执行\n"
# echo "所有_传入的_参数_如下:"
# echo $@

if [[ -z $1 ]]; then
    echo "没有传入_路径_参数"
    echo "无法评分"
    echo "退出_评分_grade_脚本"
    exit 1
fi

# NOTE 当前需要被_评分的_文件_的路径
# NOTE 该文件_可能_被学员_修改过
export current_need_to_grade_file_path=$1

# NOTE 用".tutorial"当分隔符去切分_上面的路径
export cs_club_tutorail_parent_path="${current_need_to_grade_file_path%/.tutorial*}"
export cs_club_tutorail_path=${cs_club_tutorail_parent_path}/.tutorial
# echo $cs_club_tutorail_path

export cs_club_tutorial_b40_code_parent_path="${current_need_to_grade_file_path%/b40_code*}"

export v20_article=b20_article
export v22_video=b30_video
export v24_code=b40_code
export v26_run=b50_run
export v27_demo=b60_demo

export vb20_article=${cs_club_tutorial_b40_code_parent_path}/$v20_article
export vb30_video=${cs_club_tutorial_b40_code_parent_path}/$v22_video
export vb40_code=${cs_club_tutorial_b40_code_parent_path}/$v24_code
export vb50_run=${cs_club_tutorial_b40_code_parent_path}/$v26_run
export vb60_demo=${cs_club_tutorial_b40_code_parent_path}/$v27_demo

wmlog_echo $vb20_article
wmlog_echo $vb30_video
wmlog_echo $vb40_code
wmlog_echo $vb50_run
wmlog_echo $vb60_demo

# NOTE 类似 date.sh
export vc20_10_basename_of_the_file=$(basename ${current_need_to_grade_file_path})
export vc20_20_parent_dir_name_of_the_file=$(dirname ${current_need_to_grade_file_path})
# NOTE 类似 c030_date_显示和设置系统日期与时间
export vc20_30_basename_of_the_parent_dir_name_of_the_file=$(basename ${vc20_20_parent_dir_name_of_the_file})
wmlog_echo "\n"
wmlog_echo $vc20_10_basename_of_the_file
wmlog_echo $vc20_20_parent_dir_name_of_the_file
wmlog_echo $vc20_30_basename_of_the_parent_dir_name_of_the_file
# NOTE 类似 c030_date_显示和设置系统日期与时间###date.sh
export vc20_40_simple_name_of_the_file=$vc20_30_basename_of_the_parent_dir_name_of_the_file"###"$vc20_10_basename_of_the_file

wmlog_echo $vc20_40_simple_name_of_the_file

wmlog_echo "\n"



# NOTE 缺省的_判分_入口
export cs_club_tutorail_learning_grade_script_path=${cs_club_tutorail_path}/cs36_learning_grade/aa10_grade_script.sh

export answer_sheet_original_bash_script_file=${cs_club_tutorail_path}/cs36_learning_grade/d46_模版_code_写入bash脚本.sh

# echo "l32_28_缺省的grade评分脚本路径---> ${cs_club_tutorail_learning_grade_script_path}"

# NOTE 缺省的_奖牌_目录
export cs_club_tutorail_learning_medal_path=${cs_club_tutorail_path}/cs36_learning_grade/dm42_learning_medal

# echo "l32_36_缺省的_奖牌_路径---> ${cs_club_tutorail_learning_medal_path}"

# ========================================================================================

l26_show_message(){

    echo "Hello, World!"


    return 0
}

l38_grade_simple(){

    # 设置变量
    grade=90

    # 使用if语句判断成绩
    if [ $grade -ge 90 ]; then
    echo "优秀"
    elif [ $grade -ge 80 ]; then
    echo "良好"
    elif [ $grade -ge 70 ]; then
    echo "中等"
    elif [ $grade -ge 60 ]; then
    echo "及格"
    else
    echo "不及格"
    fi

    # 输出结果
    echo "你的成绩是 $grade"
    return 0
}

# =======================================================================
# NOTE 被下面的f92_grade_by_student_answer_file()函数所调用
f32_show_demo_to_leaner(){

    # NOTE 根据_如下的_开关_决定_是否_演示
    if [[ $wmvar_demonstrate_how_to_play == "展示" ]]; then

        if [[ -d ${vb60_demo} ]]; then
            wmlog_echo "展示demo演示"

            if [[ -f ${vb60_demo}/${vc20_40_simple_name_of_the_file} ]]; then
                # echo "展示_demo演示_的脚本"
                bash ${vb60_demo}/${vc20_40_simple_name_of_the_file}
            else
                wmlog_echo "没有找到_展示_demo演示_的脚本"
            fi
        fi
    fi

    return 0
}

# =======================================================================
# NOTE 被下面的f92_grade_by_student_answer_file()函数所调用
f42_show_medal_to_leaner(){



    if [[ -f $(which imgcat) ]]; then
        if [[ -f ${cs_club_tutorail_learning_medal_path}/b11_我自学_我自讲.jpg ]]; then
            # NOTE 完成任务_给一个勋章
            echo "cloudstudio club 社区 发来贺电,您获得如下的勋章! "
            imgcat ${cs_club_tutorail_learning_medal_path}/b11_我自学_我自讲.jpg
            echo "你已经超过了,本club教程 56% 的小伙伴!"
            echo "再加把劲,在今日排行榜,力争上游!"
        else
            wmlog_echo "没有找到_祝贺_的图片"
        fi
    fi

    # REVIEW ollama_调用_qwen_0.5写一首_祝贺

    return 0
}


# =======================================================================
# REVIEW 本评分函数_适用于_是在_嵌入_b60_demo_到_b40_code_之前
# NOTE 当前需要被_评分的_文件_的路径
# NOTE 该文件_可能_被学员_修改过
# NOTE 该文件_也可能_没有被学员修改过
# NOTE 需要不同的评分_方法_需要返回不同的信息
f92_grade_by_student_answer_file(){

    if [[ -f $current_need_to_grade_file_path ]]; then

        if [[ -f ${answer_sheet_original_bash_script_file} ]]; then
            export v22_10_md5sum_of_origin_tutorial_file=$(md5sum ${answer_sheet_original_bash_script_file} | awk '{print $1}')
            export v22_20_md5sum_of_student_file=$(md5sum ${current_need_to_grade_file_path} | awk '{print $1}')

            if [[ ${v22_10_md5sum_of_origin_tutorial_file} == ${v22_20_md5sum_of_student_file} ]]; then
                echo "学员_同学_你并没有_修改_该答题文件啊! 我们是通过md5sum判断的"

                f32_show_demo_to_leaner

            else
                echo "学员_同学_对于_答题文件_进行了修改_可以去进行评分"

                f42_show_medal_to_leaner

            fi

            unset v22_10_md5sum_of_origin_tutorial_file
            unset v22_20_md5sum_of_student_file
        fi

    fi

    return 0
}


# =======================================================================
# NOTE 本函数被下面的_pf24_20_grade_by_student_answer_file_after_embedding_b60_demo_into_b40_code()函数_所调用
# NOTE 传入一个md5sum的值_作为_$1
pe20_30_judge_by_compare_with_original_md5sum_value(){

    export v22_10_md5sum_of_origin_tutorial_file=$1
    export v22_20_md5sum_of_student_file=$(md5sum ${current_need_to_grade_file_path} | awk '{print $1}')

    if [[ ${v22_10_md5sum_of_origin_tutorial_file} == ${v22_20_md5sum_of_student_file} ]]; then
        echo "学员_同学_你并没有_修改_该答题文件啊! 我们是通过md5sum判断的"

        f32_show_demo_to_leaner

    else
        echo "学员_同学_对于_答题文件_进行了修改_可以去进行评分"

        f42_show_medal_to_leaner

    fi

    unset v22_10_md5sum_of_origin_tutorial_file
    unset v22_20_md5sum_of_student_file


    return 0
}

# -----------------------------------------------------------------------

# REVIEW 本评分函数_适用于_是在_嵌入_b60_demo_到_b40_code_以后
# NOTE 当前需要被_评分的_文件_的路径
# NOTE 该文件_可能_被学员_修改过
# NOTE 该文件_也可能_没有被学员修改过
# NOTE 需要不同的评分_方法_需要返回不同的信息
pf24_20_grade_by_student_answer_file_after_embedding_b60_demo_into_b40_code(){

    if [[ -f $current_need_to_grade_file_path ]]; then

        # NOTE 取出_类似_如下的内容_
        local directory_path=$(dirname ${current_need_to_grade_file_path})

        # NOTE 取出_类似_如下的内容_c014_ls_展示目录中文件及其属性信息
        local directory_name=$(basename $(dirname ${current_need_to_grade_file_path}) )

        # NOTE 取出_类似_如下的内容_ls.sh
        local short_file_name=$(basename ${current_need_to_grade_file_path})

        #  NOTE 拼接出来一个_特征格式_类似如下_"c014_ls_展示目录中文件及其属性信息###ls.sh"
        local md5sum_mark="${directory_name}###${short_file_name}"

        local the_file_judge_file_name="还没有给文件名赋值"

        local the_file_original_md5sum="还没有赋值"

        if [[ $(find ${directory_path} -type f -name "*${md5sum_mark}*" | wc -l ) -gt 0 ]]; then
            # NOTE 获取到类似如下的文件名_获得第1个文件_就可以了
            # vv20_md5sum###c014_ls_展示目录中文件及其属性信息###ls.sh###7e2a6c0742a74b528de72d40bc8f10f5
            the_file_judge_file_name=$(ls $directory_path/*$md5sum_mark* | head -n 1)
            wmlog_echo "pf24_20_2030_the_file_judge_file_name: ${the_file_judge_file_name}\n"
            # NOTE 拆分上述文件名称_提取其中的信息_提取的是_$4_也就是最后一段信息_$NF
            # 提取到的信息_类似如下
            # "7e2a6c0742a74b528de72d40bc8f10f5"
            # NOTE 这就是_原始答题纸文件的_原始_md5sum_信息
            the_file_original_md5sum=$(echo $the_file_judge_file_name | awk -F"###" '{print $NF}')
            if [[ ${the_file_original_md5sum}  != "还没有赋值" ]]; then
                # NOTE 调用函数_去做判断
                pe20_30_judge_by_compare_with_original_md5sum_value ${the_file_original_md5sum}
            else
                echo "pf24_20_3030_判断依据出现问题_无法判断_学员_是否修改过_该文件"
            fi

        else
            echo "pf24_20_4060_无法找到_判断的依据_无法判断_学员_是否修改过_该文件"

        fi

    fi

    return 0
}


# =======================================================================

main(){

    # TODO 下面这条需要被删除_暂时保留
    # f42_show_medal_to_leaner

    # NOTE 展示一些判断依据
    # echo $current_need_to_grade_file_path
    # md5sum ${answer_sheet_original_bash_script_file}
    # md5sum ${current_need_to_grade_file_path}

    # REVIEW 本评分函数_适用于_是在_嵌入_b60_demo_到_b40_code_之前
    # f92_grade_by_student_answer_file

    # REVIEW 本评分函数_适用于_是在_嵌入_b60_demo_到_b40_code_以后
    pf24_20_grade_by_student_answer_file_after_embedding_b60_demo_into_b40_code

    return 0
}


main
