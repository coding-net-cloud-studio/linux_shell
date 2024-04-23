#!/usr/bin/env bash
###NOTE wmtag_memo_替换这里_开始###
demo(){
    echo -e "\n这是_缺省的_演示内容."
    echo -e "运行在****$(pwd)/${BASH_SOURCE[5 - 1]}脚本的****${FUNCNAME}()函数中****第${LINENO}行\n"
    return 0
}
demo
###NOTE wmtag_memo_替换这里_结束###
