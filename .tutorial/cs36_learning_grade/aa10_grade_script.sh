#!/usr/bin/env bash

export my_version=1100

# export wmvar_show_echo_message="展示"
export wmvar_show_echo_message="不展示"

export wmvar_demonstrate_how_to_play="展示"
# export wmvar_demonstrate_how_to_play="不展示"


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

    return 0
}

# pause_60_second(){
#     # NOTE 我啥都不做
#     return 0
# }

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
export cs_club_tutorial_learning_grade_dir=${cs_club_tutorail_path}/cs36_learning_grade
export cs_club_tutorail_learning_grade_script_path=${cs_club_tutorail_path}/cs36_learning_grade/aa10_grade_script.sh

export answer_sheet_original_bash_script_file=${cs_club_tutorail_path}/cs36_learning_grade/d46_模版_code_写入bash脚本.sh

# echo "l32_28_缺省的grade评分脚本路径---> ${cs_club_tutorail_learning_grade_script_path}"

# NOTE 缺省的_奖牌_目录
export cs_club_tutorail_learning_medal_path=${cs_club_tutorail_path}/cs36_learning_grade/dm42_learning_medal

# NOTE 在评价的时候_发现_学员_没有_改动_答题纸_的_内容
# NOTE 展现演示内容给学员
# NOTE 展示内容_封装在_bash的函数内
export cs_club_tutorial_learning_show_demo_to_learner="cb55_b50_20_run_me_part_for_ab50_20_run_me.sh"

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
# NOTE 这条_基本上_废弃了_被下面的f92_grade_by_student_answer_file()函数所调用
# NOTE 被下面的pe20_30_judge_by_compare_with_original_md5sum_value()函数所调用
# f32_show_demo_to_learner(){

#     # NOTE 根据_如下的_开关_决定_是否_演示
#     if [[ $wmvar_demonstrate_how_to_play == "展示" ]]; then

#         if [[ -d ${vb60_demo} ]]; then
#             wmlog_echo "展示demo演示"

#             if [[ -f ${vb60_demo}/${vc20_40_simple_name_of_the_file} ]]; then
#                 # echo "展示_demo演示_的脚本"
#                 bash ${vb60_demo}/${vc20_40_simple_name_of_the_file}
#             else
#                 wmlog_echo "没有找到_展示_demo演示_的脚本"
#             fi
#         fi
#     fi

#     return 0
# }

f32_show_demo_to_learner(){

    local show_demo_to_learner_function=$1

    # NOTE 根据_如下的_开关_决定_是否_演示
    if [[ $wmvar_demonstrate_how_to_play == "展示" ]]; then
        # NOTE 如果演示文件存在
        if [[ -f ${cs_club_tutorial_learning_show_demo_to_learner} ]]; then

            echo -e 6020_"source ${cs_club_tutorial_learning_show_demo_to_learner} ${show_demo_to_learner_function}"
            source ${cs_club_tutorial_learning_show_demo_to_learner} ${show_demo_to_learner_function}
        fi
    fi

    return 0
}

# =======================================================================
# NOTE 展示该古诗的_图像_这个图像_是_由_AIGC_文本生成得到的图像
gs30_18_show_specific_gs_image_file(){
    local gs_text_file=$1
    local gs_image_file="还没有明确"

    local v10_base_name=$(basename $gs_text_file .txt)
    local gs_image_file="${v10_base_name}.png"

    export vgs30_30_10_OLDPWD=$(pwd)

    if [[ -d ${cs_club_tutorial_learning_grade_dir}/gs90_祝贺古文诗歌_80首_图画 ]]; then
        cd ${cs_club_tutorial_learning_grade_dir}/gs90_祝贺古文诗歌_80首_图画/

        if [[ -f ${gs_image_file} ]]; then
            if [[ -f $(which imgcat) ]]; then
                imgcat ${gs_image_file}
            fi
        fi

    fi

    # NOTE 返回原来的路径中去
    cd ${vgs30_30_10_OLDPWD}

    return 0
}

# NOTE 本函数_被下面的_gs30_bg58_30_show_random_gs_text_file()函数_所调用
# 定义一个函数,用于读取文本文件并显示特定的行
gs30_22_show_specific_gs_text_lines() {
    local file=$1
    local skip_lines=2 # 初始化跳过的行数
    local line_number=0
    local first_line=""

    # 使用while循环读取文件内容
    while IFS= read -r line; do
        line_number=$((line_number + 1))

        if [[ $line_number -eq 1 ]]; then
            # NOTE 第1行是特殊处理的
            # NOTE 第1行类似如下格式
            # sg17_送元二使安西_王维

            # 使用awk处理第一行,以"_"作为分隔符,不打印第一个字段
            first_line=$(echo "$line" | awk -F '_' '{for(i=2; i<=NF; i++) printf("%s ",$i); print ""}' )
            echo -e "${first_line}"
            # echo -e "\n"
        fi

        # 如果行数大于2且行不为空,则打印
        if [[ $line_number -eq 2 ]]; then
            # NOTE 跳过第二行
            # NOTE 第2行_是全部的诗文
            continue
        fi
        if [ $line_number -gt 2 ] && [ -n "$line" ]; then
            echo "$line"
        fi
    done < "$file"
}

# NOTE 本函数被_gs30_bg58_30_show_random_gs_text_file()函数所调用
# 展现_该古诗的_图像_和_带有拼音的文本内容
gs30_38_show_gs_both_image_and_text(){

    local gs_text_file=$1

    # NOTE 用imgcat在club.cloudstudio的终端中_展现_该古诗的_文字生成的_图片
    gs30_18_show_specific_gs_image_file "${gs_text_file}"

    # NOTE 只是_显示选中文件的_特定内容
    gs30_22_show_specific_gs_text_lines "${gs_text_file}"

    return 0
}

# NOTE 被下面的gs38_20_call_show_gs()函数所调用
# 定义一个函数_用于随机选择并显示一个_祝贺古诗_文本文件的内容
gs30_bg58_30_show_random_gs_text_file() {

    # 获取所有匹配sg*.txt模式的文件列表
    files=($(ls sg*.txt))

    # 检查是否有匹配的文件
    if [ ${#files[@]} -eq 0 ]; then
        echo "没有发现古诗文本文件."
        return 1
    fi

    # 随机选择一个文件
    random_index=$((RANDOM %${#files[@]}))
    selected_file="${files[$random_index]}"

    # 显示选中的文件_全部内容
    # cat "$selected_file"

    # NOTE 只是_显示选中文件的_特定内容
    # gs30_22_show_specific_lines "$selected_file"

    # 展现_该古诗的_图像_和_带有拼音的文本内容
    gs30_38_show_gs_both_image_and_text "$selected_file"

    echo -e "\n"
    # echo -e "恭喜你! 你的答案可以执行! 上面是送给你的勋章!"

    # NOTE 完成任务_给一个勋章
    echo "cloudstudio club 社区 发来贺电,您获得如上的勋章! "
    echo "你已经超过了,本club教程 众多的 小伙伴!"
    echo "再加把劲,在今日排行榜,力争上游!"

    return 0
}

# NOTE 被下面的f42_show_medal_to_leaner()函数所调用
gs38_20_call_show_gs(){

    export vgs38_20_10_OLDPWD=$(pwd)

    # NOTE 如果没有传入参数_意思就是_随机选取一个
    if [[ -z $1 ]]; then
        # NOTE 这里是随机展示一首祝贺古诗
        if [[ -d ${cs_club_tutorial_learning_grade_dir}/gs74_祝贺古文诗词_80首_拼音_club_终端 ]]; then
            cd ${cs_club_tutorial_learning_grade_dir}/gs74_祝贺古文诗词_80首_拼音_club_终端/

            # 用于随机选择并显示一个_祝贺古诗_文本文件的内容
            gs30_bg58_30_show_random_gs_text_file
        fi
    else
        # echo "传入了_内容_参数"
        # echo -e $1
        # NOTE 如果传递有参数被本函数_就使用_$1_参数_当做必须被展示的古诗
        if [[ -d ${cs_club_tutorial_learning_grade_dir}/gs74_祝贺古文诗词_80首_拼音_club_终端 ]]; then
            cd ${cs_club_tutorial_learning_grade_dir}/gs74_祝贺古文诗词_80首_拼音_club_终端/

            if [[ -f $1 ]]; then
                # NOTE 这里是找到了制定的文件
                cat $1
            else
                # NOTE 这里是没有找到制定的文件
                # REVIEW 为了避免出现尴尬的情况_就使用随机挑选一首古诗去展示了
                # 用于随机选择并显示一个_祝贺古诗_文本文件的内容
                gs30_bg58_30_show_random_gs_text_file
            fi
        fi
    fi

    # NOTE 返回原始的位置
    cd $vgs38_20_10_OLDPWD

    return 0
}


# =======================================================================
# NOTE 被下面的f92_grade_by_student_answer_file()函数所调用
f42_show_medal_to_leaner(){



    if [[ -f $(which imgcat) ]]; then
        # NOTE 下面被注释的代码段_长期保留_这是用于测试程序逻辑的一种方法
        # if [[ -f ${cs_club_tutorail_learning_medal_path}/b11_我自学_我自讲.jpg ]]; then
        #     # NOTE 完成任务_给一个勋章
        #     echo "cloudstudio club 社区 发来贺电,您获得如下的勋章! "
        #     imgcat ${cs_club_tutorail_learning_medal_path}/b11_我自学_我自讲.jpg
        #     echo "你已经超过了,本club教程 56% 的小伙伴!"
        #     echo "再加把劲,在今日排行榜,力争上游!"
        # else
        #     wmlog_echo "没有找到_祝贺_的图片"
        # fi

        # NOTE 这里展示_祝贺古诗的_内容
        gs38_20_call_show_gs
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
# REVIEW 本函数_暂时_不被调用_在main()函数中_屏蔽了
f92_grade_by_student_answer_file(){

    if [[ -f $current_need_to_grade_file_path ]]; then

        if [[ -f ${answer_sheet_original_bash_script_file} ]]; then
            export v22_10_md5sum_of_origin_tutorial_file=$(md5sum ${answer_sheet_original_bash_script_file} | awk '{print $1}')
            export v22_20_md5sum_of_student_file=$(md5sum ${current_need_to_grade_file_path} | awk '{print $1}')

            if [[ ${v22_10_md5sum_of_origin_tutorial_file} == ${v22_20_md5sum_of_student_file} ]]; then
                echo "学员_同学_你并没有_修改_该答题文件啊! 我们是通过md5sum判断的"

                f32_show_demo_to_learner

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

    export v22_06_current_student_anwser_file=$1

    export v22_10_md5sum_of_origin_tutorial_file=$2

    local v22_23_show_demo_to_learner_function=$3

    export v22_20_md5sum_of_student_file=$(md5sum ${v22_06_current_student_anwser_file} | awk '{print $1}')

    if [[ ${v22_10_md5sum_of_origin_tutorial_file} == ${v22_20_md5sum_of_student_file} ]]; then
        echo "学员_同学_你并没有_修改_该答题文件啊! 我们是通过md5sum判断的"

        echo -e 5010_"f32_show_demo_to_learner ${v22_23_show_demo_to_learner_function}"
        f32_show_demo_to_learner ${v22_23_show_demo_to_learner_function}

    else
        # echo "学员_同学_对于_答题文件_进行了修改_可以去进行评分"

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

        # FIXME 正在处理这里
        echo -e 3008_${current_need_to_grade_file_path}

        # NOTE 取出_类似_如下的内容_
        local directory_path=$(dirname ${current_need_to_grade_file_path})

        echo -e 3010_${directory_path}

        # NOTE 取出_类似_如下的内容_c014_ls_展示目录中文件及其属性信息
        local directory_name=$(basename $(dirname ${current_need_to_grade_file_path}) )

        # NOTE 取出_类似_如下的内容_ls____run_me.sh
        local short_file_name_with_suffix=$(basename ${current_need_to_grade_file_path})

        # NOTE 获得如下的_内容_"ls.sh"
        local short_file_name_temp=$(echo ${short_file_name_with_suffix} | awk -F"____run_me" '{print $1$2}')

        local short_file_name=$short_file_name_temp

        # NOTE 获得如下的内容_"ls"
        local name_only_without_suffix=$(basename "$short_file_name" .sh)

        # c014_ls_展示目录中文件及其属性信息
        # NOTE 获得如下的内容_c014的部分
        local get_prefix_of_directory=$(echo ${directory_name} | awk -F"_${name_only_without_suffix}_" '{print $1}' )

        # NOTE 需要拼出如下的内容_当做函数名称 c014_ls_run_me()
        # NOTE 位于 .tutorial/cs36_learning_grade/cb55_b50_20_run_me_part_for_ab50_20_run_me.sh 脚本内
        # NOTE 其中有一个特殊的apt-get 这个名称需要替换为apt_get
        # FIXME
        local show_demo_to_learner_function_name_of_cb55=$(echo "${get_prefix_of_directory}_${name_only_without_suffix}_run_me" | sed 's/-/_/g' )

        echo 3030_${short_file_name}

        #  NOTE 拼接出来一个_特征格式_类似如下_"c014_ls_展示目录中文件及其属性信息###ls.sh"
        local md5sum_mark="${directory_name}###${short_file_name}"

        echo -e 3040_${md5sum_mark}

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
                echo -e 4030_${the_file_original_md5sum}

                local current_student_file_need_to_be_grade="还没有赋值"
                # NOTE 拼接出来_类似如下的全路径名称
                # /workspace/mlabspace/c06_d27_e22_36_linux/21_wmsrc/.tutorial/126_z22_c06_c33_0030_tutorial/b40_code/c058_rmdir_删除空目录文件/rmdir.sh
                current_student_file_need_to_be_grade=${directory_path}/${short_file_name}

                echo -e 4060_"pe20_30_judge_by_compare_with_original_md5sum_value ${current_student_file_need_to_be_grade} ${the_file_original_md5sum} ${show_demo_to_learner_function_name_of_cb55}"
                pe20_30_judge_by_compare_with_original_md5sum_value ${current_student_file_need_to_be_grade} ${the_file_original_md5sum} ${show_demo_to_learner_function_name_of_cb55}

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
