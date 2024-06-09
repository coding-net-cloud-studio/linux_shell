#!/usr/bin/env bash

export v20_article=b20_article
export v23_demo=b30_video
export v24_code=b40_code
export v24_20_code_template_orignal=b42_code_template_original
export v25_run=b50_run
export v26_demo=b60_demo
export v27_include_before=b70_include_before
export v29_include_after=b72_include_after

export catalog_file_name=catalog.yml
export v38_output_catalog_yml_file=d38_catalog.yml
export v40_change_from_v38_yml_file=d40_catalog_current.yml
export v44_student_exercise_file="d44_模版_code_写入bash脚本_b40_code_学生_练习文件.sh"
export v46_code_template_file=d46_模版_code_写入bash脚本.sh
export v54_b50_run_bash_script_template="db50_20_模版_b50_run_写入bash脚本.sh"

export c55_b50_run_me_demo_file_name="cb55_b50_20_run_me_part_for_ab50_20_run_me.sh"

# NOTE 同一个文件_被拷贝到多个不同的目录中去
export v50_code_file_md5sum_file=d50_catalog_b40_code_file_md5sum.sh

# NOTE 同一个文件_被拷贝到多个不同的目录中去
export v56_b20_article_markdown_md5sum_file="dc56_b70_catalog_b20_article_file_md5sum.sh"

export d36_grade_directory=../cs36_learning_grade

export c68_d32_club_file_merge="td68_d32_club_合并内容"



l30_change_catalog_directory_name(){
    if [[ -f $v38_output_catalog_yml_file ]]; then
        cp -f $v38_output_catalog_yml_file $v40_change_from_v38_yml_file
        sed -i 's/_wmgitignore//' $v40_change_from_v38_yml_file
    fi
    return 0
}

l34_copy_d40_to_tutorial_directory(){


    # if [[ -f $v38_output_catalog_yml_file ]]; then
    #     cp $v38_output_catalog_yml_file ../$v38_output_catalog_yml_file
    # fi

    if [[ -f $v40_change_from_v38_yml_file ]]; then
        # cp $v40_change_from_v38_yml_file ../$v40_change_from_v38_yml_file
        cp -f $v40_change_from_v38_yml_file ../$catalog_file_name
    fi

    # NOTE 把自身也拷贝出去
    # if [[ -f f58_拷贝到club课程_tutorial目录后进行后继处理.sh ]]; then
    #     cp f58_拷贝到club课程_tutorial目录后进行后继处理.sh ../f58_拷贝到club课程_tutorial目录后进行后继处理.sh
    # fi


    return 0
}

l36_copy_student_exercise_code_template_file_to_grade_directory(){

    # NOTE 如果存在_代码的模版文件_拷贝到_评分目录中去_这里是_run的_run_me的模版
    # NOTE 在评分的时候_用md5sum_把该_代码的模版文件_进行处理_用于判断_答案_文件是否给学员给更改过
    if [[ -f $v44_student_exercise_file ]]; then
        if [[ -d $d36_grade_directory ]]; then
            cp -f $v44_student_exercise_file $d36_grade_directory/
        else
            mkdir -p $d36_grade_directory
            cp -f $v44_student_exercise_file $d36_grade_directory/
        fi
    fi

    return 0
}

# FIXME 正在修改这里
l40_copy_run_me_code_template_file_to_grade_directory(){

    # NOTE 如果存在_代码的模版文件_拷贝到_评分目录中去_这里是_run的_run_me的模版
    # NOTE 在评分的时候_用md5sum_把该_代码的模版文件_进行处理_用于判断_答案_文件是否给学员给更改过
    if [[ -f $v46_code_template_file ]]; then
        if [[ -d $d36_grade_directory ]]; then
            cp -f $v46_code_template_file $d36_grade_directory/
        else
            mkdir -p $d36_grade_directory
            cp -f $v46_code_template_file $d36_grade_directory/
        fi
    fi

    # NOTE 同一个文件_被拷贝到多个不同的目录中去
    # 这是被拷贝到_评分目录中去
    if [[ -f $v50_code_file_md5sum_file ]]; then
        mkdir -p $d36_grade_directory
        cp -f $v50_code_file_md5sum_file $d36_grade_directory/
    fi

    # NOTE 同一个文件_被拷贝到多个不同的目录中去
    # 这是被拷贝到_评分目录中去
    if [[ -f $v56_b20_article_markdown_md5sum_file ]]; then
        mkdir -p $d36_grade_directory
        cp -f $v56_b20_article_markdown_md5sum_file $d36_grade_directory/
    fi

    return 0
}

l54_copy_run_me_code_template_file_to_grade_directory(){

    if [[ -f ${v54_b50_run_bash_script_template} ]]; then
        if [[ -d $d36_grade_directory ]]; then
            cp -f ${v54_b50_run_bash_script_template} $d36_grade_directory/
        else
            mkdir -p $d36_grade_directory/
            cp -f ${v54_b50_run_bash_script_template} $d36_grade_directory/
        fi
    fi

    return 0
}

l60_10_copy_c55_b50_run_me_demo_file_to_tutorial_directory(){

    # NOTE 拷贝到上层目录中去
    if [[ -f ${v25_run}/${c55_b50_run_me_demo_file_name} ]]; then
        cp -f ${v25_run}/${c55_b50_run_me_demo_file_name} ../${c55_b50_run_me_demo_file_name}
    fi

    return 0
}

l60_20_copy_c55_b50_run_me_demo_file_to_grade_directory(){

    if [[ -f ${v25_run}/${c55_b50_run_me_demo_file_name} ]]; then
        if [[ -d $d36_grade_directory ]]; then
            cp -f ${v25_run}/${c55_b50_run_me_demo_file_name} $d36_grade_directory/
        else
            mkdir -p $d36_grade_directory/
            cp -f ${v25_run}/${c55_b50_run_me_demo_file_name} $d36_grade_directory/
        fi
    fi

    return 0
}

# NOTE 从club其他的教程合并了一些内容_把这些合并的内容_拷贝到_tutorial的根目录下去
l68_32_copy_td68_d32_club_file_merge_under_root_of_tutorial_directory(){

  if [[ -d ${c68_d32_club_file_merge} ]]; then
    cp -r -f ${c68_d32_club_file_merge}/* ../
  fi

  return 0
}

l78_copy_other_files_to_tutorial_directory(){

    export v12_tutorail_data_d22=d22_给d18增加前缀.txt

    # NOTE 拷贝到上层目录中去
    if [[ -f $v12_tutorail_data_d22 ]]; then
        cp -f $v12_tutorail_data_d22 ../$v12_tutorail_data_d22
    fi

    # NOTE 同一个文件_被拷贝到多个不同的目录中去
    # 这是被拷贝到_教程目录中去_也就是当前目录的上一级目录
    if [[ -f $v50_code_file_md5sum_file ]]; then
        cp -f $v50_code_file_md5sum_file ../$v50_code_file_md5sum_file
    fi

    # NOTE 同一个文件_被拷贝到多个不同的目录中去
    # 这是被拷贝到_教程目录中去_也就是当前目录的上一级目录
    if [[ -f $v56_b20_article_markdown_md5sum_file ]]; then
        cp -f $v56_b20_article_markdown_md5sum_file ../$v56_b20_article_markdown_md5sum_file
    fi

    return 0
}

main(){
    l30_change_catalog_directory_name

    l34_copy_d40_to_tutorial_directory

    l36_copy_student_exercise_code_template_file_to_grade_directory

    l40_copy_run_me_code_template_file_to_grade_directory

    l54_copy_run_me_code_template_file_to_grade_directory

    l60_10_copy_c55_b50_run_me_demo_file_to_tutorial_directory

    l60_20_copy_c55_b50_run_me_demo_file_to_grade_directory

    # NOTE 从club其他的教程合并了一些内容_把这些合并的内容_拷贝到_tutorial的根目录下去
    l68_32_copy_td68_d32_club_file_merge_under_root_of_tutorial_directory

    l78_copy_other_files_to_tutorial_directory

    return 0
}

main $@
