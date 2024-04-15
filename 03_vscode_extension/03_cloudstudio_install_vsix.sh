#! /bin/bash


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

# 在cloudstudio环境中_更新_vscode的_用户_settings_文件
# 主要是设置cloud studio为"永不休眠"
f16_cs_vs_settings_user_update(){
	# cloud studio中用户设置文件
	CS_VSCODE_SETTINGS_USER=/root/.local/share/code-server/data/User/settings.json
	# -----------------------------------------------------------
	# '嵌入文档涵盖了生成脚本的主体部分.

	(
	cat <<'EOF'
{
	"cloudstudio.autosleep": "no",
	"extensions.autoCheckUpdates": false,
	"extensions.autoUpdate": false,
	"update.mode": "none",
	"update.showReleaseNotes": false,
	"code-runner.runInTerminal": true
}
EOF
	) > ${CS_VSCODE_SETTINGS_USER}

	# 可能有用的设置_暂时被取消了
	# "Codegeex.Survey": false,
	# "CS.CodeAssistant.EnableExtension": false,
	# "go.toolsManagement.autoUpdate": true,

}

f20_linux_git_setting() {
	#在Linux操作系统环境下
	# git status中文显示乱码解决:
	# https://www.cnblogs.com/v5captain/p/14832597.html
	#相当于在~/.gitconfig 文件中加入一行 file:/root/.gitconfig   core.quotepath=false
	# core.quotepath=false
	git config --global core.quotepath false
	git config --global --add safe.directory $(pwd)

	# git config命令详解
	# http://www.mybatis.cn/archives/2151.html
	# git config --list --show-origin

	# 来自廖雪峰的git教程
	# https://www.liaoxuefeng.com/wiki/896043488029600/898732837407424
	git config --global alias.lg "log --color --graph --pretty=format:'%Cred%h%Creset -%C(yellow)%d%Creset %s %Cgreen(%cr) %C(bold blue)<%an>%Creset' --abbrev-commit"

	git config --global alias.count "rev-list --all --count"

	# REVIEW 这里可能会有问题.立足解决,从本机上用tar.gz传输到cloud studio中直接解开.git用户信息的问题.
	# 相信,git clone模式,不会需要这个语句.
	git config --global --add safe.directory $(pwd)
}


# wmtag_memo_这个是_新_的版本_2023_05_02_0020
# 安装当前目录下vsix作为扩展名的vscode扩展
f3880_a22_vs_ext_install_vscode_extensions(){

	if [[ -f $(which cloudstudio) ]]; then
		# NOTE 位于cloudstudio的工作空间中
		if [[ $(cloudstudio --list-extensions | grep ms-python.python| wc -l ) -gt 0 ]]; then
			# NOTE 如果发现已经安装过python的vscode扩展
			# 首先删除_因为可能是高版本的_例如_2023.14_或_2024.0.1版本
			cloudstudio --uninstall-extension ms-python.python
		fi
	fi

	# NOTE 安装本git仓库自带的特定版本的vscode扩展
	for one_vsix_file in `ls *.vsix `
	do
		echo "准备安装--->" ${one_vsix_file}
		# 判断是否是cloudstudio的环境
		[[ -f $(which cloudstudio) ]] && cloudstudio --install-extension $(pwd)/${one_vsix_file}
	done
}

main(){

	export v33_OLDPWD=$(pwd)

    if [[ -f $(which cloudstudio) ]]; then
        f16_cs_vs_settings_user_update

        f20_linux_git_setting

		if [[ ! -f /root/.local/share/code-server/extensions/extensions.json ]]; then 
			mkdir -p /root/.local/share/code-server/extensions/
			echo "[]" > /root/.local/share/code-server/extensions/extensions.json
		fi

        f3880_a22_vs_ext_install_vscode_extensions
    fi

	cd ${v33_OLDPWD}

    return 0

}

main
