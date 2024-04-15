#! /bin/bash

# NOTE 相当于_真的要停止60秒_给操作人员看看_log
# pause_60_second(){
# 	if read -t 60 -p "暂停60秒,按回车继续运行: "
# 	then
# 		# echo "hello $REPLY,welcome to cheng du"
# 		printf "\n";
# 	else
# 		# echo "sorry, Output timeout, please execute the command again !"
# 		printf "\n时间已到,继续运行\n";
# 	fi
# }

# NOTE 相当于_不做调试
pause_60_second(){
	echo "啥都不做"
	return 0
}

# ==============================================================

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
	"code-runner.runInTerminal": true,
	"window.menuBarVisibility": "visible"
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
	git config --global user.email "自学自讲@网上自习室"
	git config --global user.name "自学自讲"

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

# REVIEW 这个函数_是我自己加入的_安装一些函数
f23_install_some_software(){
	if [[ -f $(which cloudstudio) ]]; then
		apt update -y
		DEBIAN_FRONTEND=noninteractive apt install -y \
			iputils-ping \
			make \
			lsof net-tools \
			psmisc \
			file \
			sqlite3 libsqlite3-dev
	fi

	return 0
}

# ==============================================================

# NOTE 把06_样例目录拷贝到/workspace/下面去
# NOTE 这个函数_在本git仓库的_设置_cloudstudio_工作空间中_没有被调用
# 保留一段时间_为了保持一致
l06_copy_06_example(){

	export v06_OLDPWD=$(pwd)

	if [[ ! -d /workspace/06_样例 ]]; then
		if [[ -d ./06_样例 ]]; then
			cp -r ./06_样例 /workspace/
		fi
	fi

	cd ${v06_OLDPWD}
	return 0
}


#  ==============================================================
# NOTE 本cloudstudio教学仓库下面_自带的_安装vscode扩展的_那个目录
# NOTE 本函数被f94_2828_30_main()函数所调用
l07_this_git_call_script_to_install_vscode_ext(){
	export v08_OLDPWD=$(pwd)

	if [[ -d ./03_vscode_extension ]]; then
		cd ./03_vscode_extension

		if [[ -f ./03_cloudstudio_install_vsix.sh ]]; then
			# NOTE 执行安装脚本
			./03_cloudstudio_install_vsix.sh
		fi
	fi

	cd ${v08_OLDPWD}
	return 0
}



# NOTE 调用安装grasspy的脚本
l08_call_script_to_install_vscode_ext(){
	export v08_OLDPWD=$(pwd)

	if [[ -d /root/.pyenv/03_vscode_extension ]]; then
		cd /root/.pyenv/03_vscode_extension

		if [[ -f /root/.pyenv/03_vscode_extension/03_cloudstudio_install_vsix.sh ]]; then
			# NOTE 执行安装脚本
			./03_cloudstudio_install_vsix.sh
		fi
	fi

	cd ${v08_OLDPWD}
	return 0
}

# ==============================================================

# NOTE 下面是老版本p27的_这里不再调用了
l10_install_me(){

	export v10_OLDPWD=$(pwd)

	if [[ -f $(which cloudstudio) ]]; then
		if [[ -d /root/.pyenv ]]; then
			if [[ ! -d /root/.pyenv/03_vscode_extension ]]; then
				# NOTE 此时,还是原始状态_可以进行替换
					# 老的目录修改名称
					echo "正在处置原始的/root/.pyenv目录,时间较长,请耐心等待一小会"

					# NOTE 为了化简逻辑判断_不再区分SaaS版本的cloudstudio.net
					# 还是社区版本的club.cloudstudio.net的工作空间
					# 直接做出凶残的动作_把/root/.pyenv/目录直接地,彻底地,给删除了
					# mv /root/.pyenv /root/38_wmold_pyenv_这里是原始版本的pyenv
					rm -rf /root/.pyenv
					echo "正在把新的pyenv版本拷贝到/root/.pyenv/目录下"
					# 拷贝新的版本的pyenv到/root/.pyenv的位置
					cp -r $(pwd) /root/.pyenv

					# NOTE 把06_样例目录拷贝到/workspace/下面去
					l06_copy_06_example

					# NOTE 调用安装grasspy等vscode扩展的脚本

					if [[ ! -f /root/.local/share/code-server/extensions/lock_08_安装_vscode_扩展_已经被执行过一次_锁文件_不需要再次执行_wmgitignore.txt ]]; then
						# 到这里_没有发现特征_锁文件_也就是_没有安装过我们自己的vscode扩展
						# 这个步骤_主要是cloudstudio社区版只有1G的存储空间
						# 而在"复刻"工作空间的时候/root/.local/share/code-server/extensions/会占用很多的存储空间
						# 只有删除
						# 再安装我们自己的vscode扩展
						rm -rf /root/.local/share/code-server/extensions/
						mkdir -p /root/.local/share/code-server/extensions
						# 删除完毕_开始安装我们自己版本的
						l08_call_script_to_install_vscode_ext

						mkdir -p /root/.local/share/code-server/extensions/

						touch /root/.local/share/code-server/extensions/lock_08_安装_vscode_扩展_已经被执行过一次_锁文件_不需要再次执行_wmgitignore.txt

						echo "新安装我们自己的vscode扩展组合_并且生成了_vscode_扩展_安装_锁文件"

						pause_60_second

						cloudstudio --list-extensions

						pause_60_second

						ls -lah /root/.local/share/code-server/extensions/

						pause_60_second

					else
						echo "/root/.local/share/code-server/extensions/_目录下_已经是我们自己的vscode扩展组合_不需要再次安装了"
						pause_60_second
					fi


					echo "已经把/root/.pyenv更新为新的草莽grasspy3.10.1的版本了"
					echo "欢迎加入 草蟒极速 QQ 交流群:760167264 风里雨里,我们在那里等你 "
					echo "编程语言开发社区网址: https://www.ploc.org.cn/ploc/ "
					echo "我作为一名普通的"编程语言开放社区成员_与_grasspy.cn社区成员",以个人的身份,向吴烜xuan三声,凹语言群体,林纳斯·本纳第克特·托瓦兹(Linus Benedict Torvalds)等致敬!"
			else
				echo "已经执行过/root/.pyenv的替换操作_不需要再次进行"
			fi
		else
			echo "没有找到/root/.pyenv,无法进行后继操作"
		fi
	fi

	cd ${v10_OLDPWD}

	if [[ -f $(which cloudstudio) ]]; then
		if [[ -d /workspace/06_样例 ]]; then
			# 在cloudstudio.net中再打开一个窗口
			cloudstudio -n /workspace/06_样例
		fi
	fi

	return 0
}


# ==============================================================
# NOTE 针对club.cloudstudio.net社区版本
# NOTE 从github.com上获得git仓库建立工作空间以后
# NOTE 需要从gitee上获取新的pyenv环境
l30_git_clone_and_install_new_pyenv(){

	export v10_OLDPWD=$(pwd)

	# NOTE 判断是否_有必要_执行本函数
	# 如果没有_必要_再次执行_就return_0
	if [[ -f $(which cloudstudio) ]]; then
		if [[ -f /workspace/lock_01_run_me_已经被执行过一次_锁文件_不需要再次执行_wmgitignore.txt ]]; then
			# 标志性的锁文件已经存在了
			if [[ -d /root/.pyenv/03_vscode_extension ]]; then
				echo "没有必要再次执行_01_rum_me_的l30()函数"
				return 0
			fi
		fi
	fi



	if [[ -f $(which cloudstudio) ]]; then
		if [[ -d /root/.pyenv ]]; then
			if [[ ! -d /root/.pyenv/03_vscode_extension ]]; then

				# 如果发现_锁文件_把原有的那个文件删除
				# 后面会重新创建的
				[[ -f /workspace/lock_01_run_me_已经被执行过一次_锁文件_不需要再次执行_wmgitignore.txt ]] && rm /workspace/lock_01_run_me_已经被执行过一次_锁文件_不需要再次执行_wmgitignore.txt

				# NOTE 此时,还是原始状态_可以进行替换
					# 老的目录修改名称
					echo "正在处置原始的/root/.pyenv目录,时间较长,请耐心等待一小会"

					# NOTE 为了化简逻辑判断_不再区分SaaS版本的cloudstudio.net
					# 还是社区版本的club.cloudstudio.net的工作空间
					# 直接做出凶残的动作_把/root/.pyenv/目录直接地,彻底地,给删除了
					# mv /root/.pyenv /root/38_wmold_pyenv_这里是原始版本的pyenv

					[[ -d /root/.pyenv ]] && rm -rf /root/.pyenv

					echo "正在把新的pyenv版本_git_clone_到/root/.pyenv/目录下"
					# git_clone_新的版本的pyenv到/root/.pyenv的位置
					git clone --depth=1 https://gitee.com/coding_net_cloud_studio/ploc_grasspy_pyenv.git /root/.pyenv

					# NOTE 把06_样例目录拷贝到/workspace/下面去
					# l06_copy_06_example

					# NOTE 调用安装grasspy等vscode扩展的脚本

					if [[ ! -f /root/.local/share/code-server/extensions/lock_08_安装_vscode_扩展_已经被执行过一次_锁文件_不需要再次执行_wmgitignore.txt ]]; then
						# 到这里_没有发现特征_锁文件_也就是_没有安装过我们自己的vscode扩展
						# 这个步骤_主要是cloudstudio社区版只有1G的存储空间
						# 而在"复刻"工作空间的时候/root/.local/share/code-server/extensions/会占用很多的存储空间
						# 只有删除
						# 再安装我们自己的vscode扩展
						rm -rf /root/.local/share/code-server/extensions/
						mkdir -p /root/.local/share/code-server/extensions
						# NOTE 必须手工建立一个如下的文件_才会正确的安装vscode的扩展
						echo "[]" > /root/.local/share/code-server/extensions/extensions.json

						ls -lah /root/.local/share/code-server/extensions/
						pause_60_second

						# 删除完毕_开始安装我们自己版本的
						l08_call_script_to_install_vscode_ext

						mkdir -p /root/.local/share/code-server/extensions/

						touch /root/.local/share/code-server/extensions/lock_08_安装_vscode_扩展_已经被执行过一次_锁文件_不需要再次执行_wmgitignore.txt

						echo "新安装我们自己的vscode扩展组合_并且生成了_vscode_扩展_安装_锁文件"

						pause_60_second

						cloudstudio --list-extensions

						pause_60_second

						ls -lah /root/.local/share/code-server/extensions/

						pause_60_second

					else
						echo "/root/.local/share/code-server/extensions/_目录下_已经是我们自己的vscode扩展组合_不需要再次安装了"
						pause_60_second
					fi


					echo "已经把/root/.pyenv更新为新的草莽grasspy3.10.1的版本了"
					echo "欢迎加入 草蟒极速 QQ 交流群:760167264 风里雨里,我们在那里等你 "
					echo "编程语言开发社区网址: https://www.ploc.org.cn/ploc/ "
					echo "我作为一名普通的"编程语言开放社区成员_与_grasspy.cn社区成员",以个人的身份,向吴烜xuan三声,凹语言群体,林纳斯·本纳第克特·托瓦兹(Linus Benedict Torvalds)等致敬!"

					touch /workspace/lock_01_run_me_已经被执行过一次_锁文件_不需要再次执行_wmgitignore.txt
			else
				echo "已经执行过/root/.pyenv的替换操作_不需要再次进行"
			fi
		else
			echo "没有找到/root/.pyenv,无法进行后继操作"
		fi
	fi

	cd ${v10_OLDPWD}

	# NOTE 如下的都不要再进行了
	# if [[ -f $(which cloudstudio) ]]; then
	# 	if [[ -d /workspace/06_样例 ]]; then
	# 		# 在cloudstudio.net中再打开一个窗口
	# 		cloudstudio -n /workspace/06_样例
	# 	fi
	# fi

	return 0
}

# ==============================================================
# NOTE 通过git_subtree的形式把子git仓库加入进来_并新增一个cloudstudio的窗口
# NOTE 这个模式太复杂了_相互协调很麻烦_放弃了
l57_30_git_clong_c28_aig_meetup(){

	if [[ ! -d /workspace/w22_c28_aig_meetup ]]; then
		git subtree \
			add \
			-P  w22_c28_aig_meetup \
			https://gitee.com/coding_net_cloud_studio/w22_c28_aig_meetup.git \
			wmstudy \
			--squash
	fi

	if [[ ! -f /tmp/c06_新码农在家背单词_主窗口已经打开_不需要再次打开_锁.sh ]]; then
		# 上述的特殊文件_不存在_就创建
		# 该锁文件_防止_相互_循环拉起
		touch /tmp/c06_新码农在家背单词_主窗口已经打开_不需要再次打开_锁.sh
	fi

	# git clone subtree完毕以后_新增一个cloudstudio窗口去展示_该子git仓库的信息
	# if [[ -d /workspace/w22_c28_aig_meetup ]]; then
	# 	cloudstudio -n /workspace/w22_c28_aig_meetup
	# fi

	return 0
}

# ==============================================================

# NOTE 由于通过git_clone_下载新的pyenv到/root/.pyenv位置以后
# /root/.pyenv/.git目录会占有148M的存储空间
# /root/.pyenv/06_样例_目录_会占用_53M_的存储空间
# 删除上述两个目录_释放一定的空间
# 大约释放200M左右的存储空间
l77_clean_root_pyenv_sub_directories(){

	if [[ -d /root/.pyenv/06_样例 ]]; then
		rm -rf /root/.pyenv/06_样例
	fi

	if [[ -d /root/.pyenv/.git ]]; then
		rm -rf /root/.pyenv/.git
	fi

	return 0
}

# NOTE 做一些设置
l79_final_settings(){

	# NOTE 把当前目录下_很可能位于/workspace目录下_所有的bash_script_都变为可执行文件
	find . -name "*.sh" -exec chmod +x {} \;

	if [[ ! -f /usr/bin/cloudstudio ]]; then
		if [[ -f /.PlnPyKFp4CRfFtgC1/vendor/modules/code-oss-dev/bin/remote-cli/cloudstudio ]]; then
			ln -s /.PlnPyKFp4CRfFtgC1/vendor/modules/code-oss-dev/bin/remote-cli/cloudstudio /usr/bin/cloudstudio
		fi
	fi

	# NOTE 找到bash脚本wmcloudstudio并且变为可执行文件
	find . -name "wmcloudstudio" -exec chmod +x {} \;

	# NOTE 把当前目录下wmcloudstudio拷贝到容易被找到的目录中去
	if [[ ! -f $(which wmcloudstudio) ]]; then
		if [[ -f ./wmcloudstudio ]]; then
			cp ./wmcloudstudio /usr/bin/wmcloudstudio
			cp ./wmcloudstudio /bin/wmcloudstudio
		fi
	fi

	return 0
}

# ==============================================================
# NOTE 定义main()函数的主体
f94_2828_30_main(){

	# 无论何时都先尝试建立这样一个目录
	# mkdir -p /root/.pyenv

	f16_cs_vs_settings_user_update

	f20_linux_git_setting

	# f23_install_some_software

	# NOTE 下面是老版本p27的_这里不再调用了
	# l10_install_me

	# 需要从gitee上获取新的pyenv环境
	# l30_git_clone_and_install_new_pyenv

	# NOTE 通过git_subtree的形式把子git仓库加入进来_并新增一个cloudstudio的窗口
	# NOTE 这个模式太复杂了_相互协调很麻烦_放弃了
	# l57_30_git_clong_c28_aig_meetup

	# NOTE 增加一个clean的函数_释放/root/.pyenv下更多的空间
	# l77_clean_root_pyenv_sub_directories

	# NOTE 如果本cloudstudio社区版本的教学仓库中_具有自带的_03_vscode_extension目录
	# NOTE 该目录下_具有很少数的几个必须安装的vscode扩展
	# NOTE 把这几个扩展给安装上
	l07_this_git_call_script_to_install_vscode_ext

	l79_final_settings

	return 0
}

# ==============================================================


f96_3060_check_environment_and_run_main(){

	if [[ -f $(which cloudstudio) ]]; then
		# 已经位于cloudstudio.net的工作空间以内了
		if [[ $(whoami) == "root" ]]; then
			echo "$(whoami)"
			if [[ -d /workspace ]]; then
				# 这里可以执行本脚本
				echo "可以执行后继的操作"

				# 这里是main函数
				f94_2828_30_main

			fi
		else
			echo "不知道是_啥环境_不能运行本脚本"
			# 不符合运行条件_退出本脚本的运行
			exit 0
		fi
	fi

	return 0
}

# ==============================================================
# NOTE main()是f94_2828_30_main()的便捷名称
main(){

	f94_2828_30_main

	return 0
}

# NOTE 调用main()函数
main

# ==============================================================
# 下面是_正式_的入口
# echo $*
# [ -z "$1" ] && eval f96_3060_check_environment_and_run_main || eval $1 $*
