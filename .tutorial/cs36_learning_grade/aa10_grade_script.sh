#!/usr/bin/env bash


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


export current_need_to_grade_file_path=$1

# NOTE 用".tutorial"当分隔符去切分_上面的路径
export cs_club_tutorail_parent_path="${current_need_to_grade_file_path%/.tutorial*}"
export cs_club_tutorail_path=${cs_club_tutorail_parent_path}/.tutorial
# echo $cs_club_tutorail_path

# NOTE 缺省的_判分_入口
export cs_club_tutorail_learning_grade_script_path=${cs_club_tutorail_path}/cs36_learning_grade/aa10_grade_script.sh

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

main(){

    f42_show_medal_to_leaner
    return 0
}


main
