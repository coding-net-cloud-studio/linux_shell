#!/usr/bin/env bash
###NOTE wmtag_memo_替换这里_开始###
demo(){
    echo -e "\n这是_缺省的_演示内容_1070."
    export the_demo_script_name=$(basename ${BASH_SOURCE[0]})
    echo -e "运行在****$(pwd)/${the_demo_script_name}脚本的****${FUNCNAME}()函数中****第${LINENO}行\n"
    unset the_demo_script_name
    return 0
}
demo
###NOTE wmtag_memo_替换这里_结束###
