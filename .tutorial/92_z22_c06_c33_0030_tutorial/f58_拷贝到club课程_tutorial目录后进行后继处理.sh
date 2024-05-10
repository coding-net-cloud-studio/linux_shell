#!/usr/bin/env bash

export catalog_file_name=catalog.yml
export v38_output_catalog_yml_file=d38_catalog.yml
export v40_change_from_v38_yml_file=d40_catalog_current.yml
export v46_code_template_file=d46_模版_code_写入bash脚本.sh

# NOTE 同一个文件_被拷贝到多个不同的目录中去
export v50_code_file_md5sum_file=d50_catalog_b40_code_file_md5sum.sh

export d36_grade_directory=../cs36_learning_grade

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

l38_copy_grade_code_template_file_to_grade_directory(){

    # NOTE 如果存在_代码的模版文件_拷贝到_评分目录中去
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
    return 0
}

l58_copy_other_files_to_tutorial_directory(){

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

    return 0
}

main(){
    l30_change_catalog_directory_name

    l34_copy_d40_to_tutorial_directory

    l38_copy_grade_code_template_file_to_grade_directory

    l58_copy_other_files_to_tutorial_directory

    return 0
}

main $@
