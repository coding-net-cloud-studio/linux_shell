demo(){
    echo -e "\n这是_缺省的_演示内容_1070."
    export the_demo_script_name=$(basename ${BASH_SOURCE[0]})
    echo -e "运行在****$(pwd)/${the_demo_script_name}脚本的****${FUNCNAME}()函数中****第${LINENO}行\n"

    #TODO 进入某一个实验目录
    export the_demo_script_name=$(basename ${BASH_SOURCE[0]})
    export to_demo_dirname=$(echo $the_demo_script_name | awk -F### '{print $1}')
    [[ -d ~/${to_demo_dirname} ]] && cd ~/${to_demo_dirname} && pwd && ls -l

    unset the_demo_script_name
    return 0
}
demo
