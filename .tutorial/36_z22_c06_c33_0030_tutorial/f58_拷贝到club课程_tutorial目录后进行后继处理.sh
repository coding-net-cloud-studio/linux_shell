#!/usr/bin/env bash

export catalog_file_name=catalog.yml
export v38_output_catalog_yml_file=d38_catalog.yml
export v40_change_from_v38_yml_file=d40_catalog_current.yml

l30_change_catalog_directory_name(){
    if [[ -f $v38_output_catalog_yml_file ]]; then
        cp $v38_output_catalog_yml_file $v40_change_from_v38_yml_file
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
        cp $v40_change_from_v38_yml_file ../$catalog_file_name
    fi

    # NOTE 把自身也拷贝出去
    # if [[ -f f58_拷贝到club课程_tutorial目录后进行后继处理.sh ]]; then
    #     cp f58_拷贝到club课程_tutorial目录后进行后继处理.sh ../f58_拷贝到club课程_tutorial目录后进行后继处理.sh
    # fi


    return 0
}

main(){
    l30_change_catalog_directory_name

    l34_copy_d40_to_tutorial_directory

    return 0
}

main $@
