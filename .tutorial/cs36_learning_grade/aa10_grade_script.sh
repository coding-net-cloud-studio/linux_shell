#!/usr/bin/env bash

export my_version=1060


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

echo $vb20_article
echo $vb30_video
echo $vb40_code
echo $vb50_run
echo $vb60_demo

# NOTE 类似 date.sh
export vc20_10_basename_of_the_file=$(basename ${current_need_to_grade_file_path})
export vc20_20_parent_dir_name_of_the_file=$(dirname ${current_need_to_grade_file_path})
# NOTE 类似 c030_date_显示和设置系统日期与时间
export vc20_30_basename_of_the_parent_dir_name_of_the_file=$(basename ${vc20_20_parent_dir_name_of_the_file})
echo -e "\n"
echo $vc20_10_basename_of_the_file
echo $vc20_20_parent_dir_name_of_the_file
echo $vc20_30_basename_of_the_parent_dir_name_of_the_file
# NOTE 类似 c030_date_显示和设置系统日期与时间###date.sh
export vc20_40_simple_name_of_the_file=$vc20_30_basename_of_the_parent_dir_name_of_the_file"###"$vc20_10_basename_of_the_file

echo $vc20_40_simple_name_of_the_file

echo -e "\n"



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

    if [[ -d ${vb60_demo} ]]; then 
        echo "展示demo演示"

        if [[ -f ${vb60_demo}/${vc20_40_simple_name_of_the_file} ]]; then 
            # echo "展示_demo演示_的脚本"
            source ${vb60_demo}/${vc20_40_simple_name_of_the_file}
        else
            echo "没有找到_展示_demo演示_的脚本"
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
            echo "没有找到_祝贺_的图片"
        fi
    fi

    # REVIEW ollama_调用_qwen_0.5写一首_祝贺

    return 0
}


# =======================================================================
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

main(){

    # TODO 下面这条需要被删除_暂时保留
    # f42_show_medal_to_leaner

    # NOTE 展示一些判断依据
    # echo $current_need_to_grade_file_path
    # md5sum ${answer_sheet_original_bash_script_file}
    # md5sum ${current_need_to_grade_file_path}


    f92_grade_by_student_answer_file

    return 0
}


main
