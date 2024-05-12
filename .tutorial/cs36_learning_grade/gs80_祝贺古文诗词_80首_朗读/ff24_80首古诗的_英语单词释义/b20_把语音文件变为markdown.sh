#!/usr/bin/env bash

lb20_20_change_mp3_to_md(){
    for one_file in $(ls *.mp3)
    do
        echo $one_file 
        local new_md_file_name=${one_file%.mp3}.md
        echo $new_md_file_name

        mv $one_file $new_md_file_name

        echo > $new_md_file_name
    done
    return 0
}

lb20_20_change_mp3_to_md
