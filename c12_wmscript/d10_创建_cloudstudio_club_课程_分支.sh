#!/usr/bin/env bash

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

# =========================================================================================


# 10_36个常用linux命令_每3个命令成为一个单独的课程_每个课程是一个club的工作空间_共12个课程
# 12_每个3个命令是1个独立的课程_教师在8分钟内讲解完成_学员可以在后继的时间中直接在club.cloudstudio中操作
# 20_12个课程构成1个系列
# 30_每个1个课程含有3个命令_都对应着本git仓库的一个git分支
# 40_每个分支命名为b10_classroom的模式

l28_create_git_branch_for_club_classroom(){

    # NOTE 我们共建立16个classroom_也就是16个git分支
    # NOTE 我们只需要使用12个classroom
    # NOTE 保留
    git branch b10_classroom
    git branch b15_classroom
    # NOTE 从b20开始使用
    git branch b20_classroom
    git branch b25_classroom
    git branch b30_classroom
    git branch b35_classroom
    git branch b40_classroom
    git branch b45_classroom
    git branch b50_classroom
    git branch b55_classroom
    git branch b60_classroom
    git branch b65_classroom
    git branch b70_classroom
    git branch b75_classroom
    # NOTE 保留
    git branch b80_classroom
    git branch b85_classroom

    return 0
}


# NOTE 把大约16个_classroom的_git分支_都_推送到_3个类似gitcode_github的远程仓库中
l36_git_push_all_classroom_branch_to_remote(){

    # NOTE 保留
    git push -u origin  b10_classroom
    git push -u gitcode b10_classroom
    git push -u github  b10_classroom

    git push -u origin  b15_classroom
    git push -u gitcode b15_classroom
    git push -u github  b15_classroom

    # NOTE 从_b20开始
    git push -u origin  b20_classroom
    git push -u gitcode b20_classroom
    git push -u github  b20_classroom

    git push -u origin  b25_classroom
    git push -u gitcode b25_classroom
    git push -u github  b25_classroom

    git push -u origin  b30_classroom
    git push -u gitcode b30_classroom
    git push -u github  b30_classroom

    git push -u origin  b35_classroom
    git push -u gitcode b35_classroom
    git push -u github  b35_classroom

    git push -u origin  b40_classroom
    git push -u gitcode b40_classroom
    git push -u github  b40_classroom

    git push -u origin  b45_classroom
    git push -u gitcode b45_classroom
    git push -u github  b45_classroom

    git push -u origin  b50_classroom
    git push -u gitcode b50_classroom
    git push -u github  b50_classroom

    git push -u origin  b55_classroom
    git push -u gitcode b55_classroom
    git push -u github  b55_classroom

    git push -u origin  b60_classroom
    git push -u gitcode b60_classroom
    git push -u github  b60_classroom

    git push -u origin  b65_classroom
    git push -u gitcode b65_classroom
    git push -u github  b65_classroom

    git push -u origin  b70_classroom
    git push -u gitcode b70_classroom
    git push -u github  b70_classroom

    git push -u origin  b75_classroom
    git push -u gitcode b75_classroom
    git push -u github  b75_classroom

    # NOTE 保留
    git push -u origin  b80_classroom
    git push -u gitcode b80_classroom
    git push -u github  b80_classroom

    git push -u origin  b85_classroom
    git push -u gitcode b85_classroom
    git push -u github  b85_classroom

    return 0
}

# NOTE 如下的函数已经废弃了
# NOTE 在大约16个classroom的git分支中_都加入同样的.gitignore的配置
# l44_10_all_classroom_change_gitignore(){

#     # NOTE 保留
#     git switch b10_classroom && git add ../.gitignore && git commit -m "2024_04_08_0030-wmtag_memo_调整gitignroe规则" 
#     git switch b15_classroom && git add ../.gitignore && git commit -m "2024_04_08_0030-wmtag_memo_调整gitignroe规则"
#     # NOTE 从b20开始使用
#     git switch b20_classroom && git add ../.gitignore && git commit -m "2024_04_08_0030-wmtag_memo_调整gitignroe规则"
#     git switch b25_classroom && git add ../.gitignore && git commit -m "2024_04_08_0030-wmtag_memo_调整gitignroe规则"
#     git switch b30_classroom && git add ../.gitignore && git commit -m "2024_04_08_0030-wmtag_memo_调整gitignroe规则"
#     git switch b35_classroom && git add ../.gitignore && git commit -m "2024_04_08_0030-wmtag_memo_调整gitignroe规则"
#     git switch b40_classroom && git add ../.gitignore && git commit -m "2024_04_08_0030-wmtag_memo_调整gitignroe规则"
#     git switch b45_classroom && git add ../.gitignore && git commit -m "2024_04_08_0030-wmtag_memo_调整gitignroe规则"
#     git switch b50_classroom && git add ../.gitignore && git commit -m "2024_04_08_0030-wmtag_memo_调整gitignroe规则"
#     git switch b55_classroom && git add ../.gitignore && git commit -m "2024_04_08_0030-wmtag_memo_调整gitignroe规则"
#     git switch b60_classroom && git add ../.gitignore && git commit -m "2024_04_08_0030-wmtag_memo_调整gitignroe规则"
#     git switch b65_classroom && git add ../.gitignore && git commit -m "2024_04_08_0030-wmtag_memo_调整gitignroe规则"
#     git switch b70_classroom && git add ../.gitignore && git commit -m "2024_04_08_0030-wmtag_memo_调整gitignroe规则"
#     git switch b75_classroom && git add ../.gitignore && git commit -m "2024_04_08_0030-wmtag_memo_调整gitignroe规则"
#     # NOTE 保留
#     git switch b80_classroom && git add ../.gitignore && git commit -m "2024_04_08_0030-wmtag_memo_调整gitignroe规则"
#     git switch b85_classroom && git add ../.gitignore && git commit -m "2024_04_08_0030-wmtag_memo_调整gitignroe规则"

#     return 0
# }

# NOTE 在大约16个classroom的git分支中_都加入同样的.gitignore的配置
l44_20_all_classroom_change_gitignore(){

       git switch b10_classroom && cp ../c10_all_classroom_gitignore .gitignore && git add .gitignore && git commit -m "2024_04_08_0030-wmtag_memo_调整gitignroe规则" \
    && git switch b15_classroom && cp ../c10_all_classroom_gitignore .gitignore && git add .gitignore && git commit -m "2024_04_08_0030-wmtag_memo_调整gitignroe规则" \
    && git switch b20_classroom && cp ../c10_all_classroom_gitignore .gitignore && git add .gitignore && git commit -m "2024_04_08_0030-wmtag_memo_调整gitignroe规则" \
    && git switch b25_classroom && cp ../c10_all_classroom_gitignore .gitignore && git add .gitignore && git commit -m "2024_04_08_0030-wmtag_memo_调整gitignroe规则" \
    && git switch b30_classroom && cp ../c10_all_classroom_gitignore .gitignore && git add .gitignore && git commit -m "2024_04_08_0030-wmtag_memo_调整gitignroe规则" \
    && git switch b35_classroom && cp ../c10_all_classroom_gitignore .gitignore && git add .gitignore && git commit -m "2024_04_08_0030-wmtag_memo_调整gitignroe规则" \
    && git switch b40_classroom && cp ../c10_all_classroom_gitignore .gitignore && git add .gitignore && git commit -m "2024_04_08_0030-wmtag_memo_调整gitignroe规则" \
    && git switch b45_classroom && cp ../c10_all_classroom_gitignore .gitignore && git add .gitignore && git commit -m "2024_04_08_0030-wmtag_memo_调整gitignroe规则" \
    && git switch b50_classroom && cp ../c10_all_classroom_gitignore .gitignore && git add .gitignore && git commit -m "2024_04_08_0030-wmtag_memo_调整gitignroe规则" \
    && git switch b55_classroom && cp ../c10_all_classroom_gitignore .gitignore && git add .gitignore && git commit -m "2024_04_08_0030-wmtag_memo_调整gitignroe规则" \
    && git switch b60_classroom && cp ../c10_all_classroom_gitignore .gitignore && git add .gitignore && git commit -m "2024_04_08_0030-wmtag_memo_调整gitignroe规则" \
    && git switch b65_classroom && cp ../c10_all_classroom_gitignore .gitignore && git add .gitignore && git commit -m "2024_04_08_0030-wmtag_memo_调整gitignroe规则" \
    && git switch b70_classroom && cp ../c10_all_classroom_gitignore .gitignore && git add .gitignore && git commit -m "2024_04_08_0030-wmtag_memo_调整gitignroe规则" \
    && git switch b75_classroom && cp ../c10_all_classroom_gitignore .gitignore && git add .gitignore && git commit -m "2024_04_08_0030-wmtag_memo_调整gitignroe规则" \
    && git switch b80_classroom && cp ../c10_all_classroom_gitignore .gitignore && git add .gitignore && git commit -m "2024_04_08_0030-wmtag_memo_调整gitignroe规则" \
    && git switch b85_classroom && cp ../c10_all_classroom_gitignore .gitignore && git add .gitignore && git commit -m "2024_04_08_0030-wmtag_memo_调整gitignroe规则"

    return 0
}



# NOTE 在所有的大约16个classroom_git分支中_统一gitignore规则_并推送到所有远端的_gitcode_github等
l46_git_add_gitignore_and_push_to_all_remote_branch(){
    # NOTE 在大约16个classroom的git分支中_都加入同样的.gitignore的配置
    # l44_20_all_classroom_change_gitignore
    # NOTE 把大约16个_classroom的_git分支_都_推送到_3个类似gitcode_github的远程仓库中
    l36_git_push_all_classroom_branch_to_remote

    return 0
}

# =========================================================================================

# NOTE dealwith_gitignore是_下面函数的_快捷调用方式
# NOTE l46_git_add_gitignore_and_push_to_all_remote_branch()函数
o46_dealwith_gitignore(){

    l46_git_add_gitignore_and_push_to_all_remote_branch

    return 0
}

# =========================================================================================

help(){

    echo "o46_dealwith_gitignore 在所有的大约16个classroom_git分支中_统一gitignore规则_并推送到所有远端的_gitcode_github等"
    return 0
}
# =========================================================================================

# 安装各种软件,设置各种环境.
# 需要运行比较长的时间.
f92_2828_main(){

    help

    return 0
}

# =========================================================================================

c10_wmtask_runonce_set_env(){

    f92_2828_main

    return 0
}

# =========================================================================================


# 下面是_正式_的入口
[ -z "$1" ] && eval c10_wmtask_runonce_set_env || eval $1 $*
