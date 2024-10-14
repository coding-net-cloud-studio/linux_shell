#! /bin/bash

# NOTE 本教程是针对完全的初学者
# NOTE 本教程是针对"泛开发者"
# NOTE 泛开发者是指:在本职工作领域是专业人士,但不以编程作为谋生手段
# NOTE 本教程不适合初级程序员_初级程序员应该去看更深的教程了_不需要看本教程
	# NOTE 向已经是程序员的群体,深度推荐:白明,柴树杉,丁尔男,史斌,郝林,徐秋华,陈剑煜,刘丹冰,廖显东,郭宏志,湖南长沙老郭(Nick),王中阳,申专等老师与专家的专业课程
# NOTE 所有曾经编写过300行代码的人士_都不适合看本初级教程了_应该去看其他更深的教程了
# NOTE Makefile中是支持utf-8(unicode)作为目标名称的_我们这样写的目的是给"泛开发者"在初期容易理解实行的
# NOTE 在后继的课程中_我们将取消这些方法_回归到_英文目标名的传统方式
# NOTE 在bash shell script脚本中_针对_泛开发者_我们使用很长的函数名称这样的_非传统方式
# NOTE 在后继的课程中_我们将取消上述_长函数名的_非传统方式_回归常规写法

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
	"go.toolsManagement.autoUpdate": true,
	"redhat.telemetry.enabled": false,
	"bookmarks.saveBookmarksInProject": true,
	"Codegeex.Privacy": true,
	"Codegeex.Survey": false,
	"Codegeex.EnableExtension": false,
	"CS.CodeAssistant.EnableExtension": false,
}
EOF
	) > ${CS_VSCODE_SETTINGS_USER}

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

}

# NOTE wmtag_memo_教学_如何定制自己使用的vscode扩展组合
# wmtag_memo_查看当前有哪些vscode扩展已经被安装上了_在本函数中_各位同学可以_自由的添加_或删除_vscode扩展_定制自己的需要环境
# 在cloudstudio中快速的安装某几个vscode的扩展
f27_38_install_some_vs_ext_quick(){

	# NOTE 列出cloudstudio云上工作室已经安装的vscode扩展列表
	# [[ -f $(which cloudstudio) ]] && cloudstudio --list-extensions

	# NOTE 安装某些vscode的扩展
	echo -e "\n-------在cloudstudio中快速的安装某几个vscode的扩展-------\n"

	[[ -f $(which cloudstudio) ]] && cloudstudio --install-extension  formulahendry.code-runner         --force
	# [[ -f $(which cloudstudio) ]] && cloudstudio --install-extension  AMiner.codegeex                   --force
	[[ -f $(which cloudstudio) ]] && cloudstudio --install-extension  alefragnani.Bookmarks             --force
	[[ -f $(which cloudstudio) ]] && cloudstudio --install-extension  ExodiusStudios.comment-anchors    --force
	# [[ -f $(which cloudstudio) ]] && cloudstudio --install-extension  ritwickdey.LiveServer             --force
	[[ -f $(which cloudstudio) ]] && cloudstudio --install-extension  ms-azuretools.vscode-docker       --force
	# [[ -f $(which cloudstudio) ]] && cloudstudio --install-extension  cweijan.vscode-office             --force
	# [[ -f $(which cloudstudio) ]] && cloudstudio --install-extension  arcanis.vscode-zipfs              --force
	# [[ -f $(which cloudstudio) ]] && cloudstudio --install-extension  mads-hartmann.bash-ide-vscode     --force
	# [[ -f $(which cloudstudio) ]] && cloudstudio --install-extension  vsls-contrib.codetour             --force
	# [[ -f $(which cloudstudio) ]] && cloudstudio --install-extension  ms-vscode.makefile-tools          --force
	[[ -f $(which cloudstudio) ]] && cloudstudio --install-extension  golang.go                         --force
	# [[ -f $(which cloudstudio) ]] && cloudstudio --install-extension  egomobile.vscode-powertools       --force
	# [[ -f $(which cloudstudio) ]] && cloudstudio --install-extension  maelvalais.autoconf               --force
	[[ -f $(which cloudstudio) ]] && cloudstudio --install-extension  Sycl.markdown-command-runner      --force
	[[ -f $(which cloudstudio) ]] && cloudstudio --install-extension  CloudStudio.tutorialkit           --force

	# [[ -f $(which cloudstudio) ]] && cloudstudio --install-extension  Fr43nk.seito-openfile             --force

	# [[ -f $(which cloudstudio) ]] && cloudstudio --install-extension  cweijan.vscode-mysql-client2      --force
	[[ -f $(which cloudstudio) ]] && cloudstudio --install-extension  cweijan.vscode-database-client2   --force

	# [[ -f $(which cloudstudio) ]] && cloudstudio --install-extension  mkhl.direnv                       --force

	# NOTE 下面是强制删除某些vscode的扩展
	[[ -f $(which cloudstudio) ]] && cloudstudio --uninstall-extension muhammad-sammy.csharp             --force

}

#类似imgcat等小工具位于.wmstudy/bin的目录下
f28_20_install_some_wmstudy_bin_tools(){
	# pause_60_second
	# 只有位于cloudstudio工作空间中才执行
	if [[ -f $(which cloudstudio) ]]; then
		# pause_60_second
		# 只有具有如下的目录才执行
		if [[ -d .wmstudy/bin/ ]]; then
			# pause_60_second
			chmod +x .wmstudy/bin/*
			# ls -lah .wmstudy/bin/
			cp -r .wmstudy/bin/* /usr/bin/
			# ls -lah /usr/bin/imgcat
			cp -r .wmstudy/bin/* /bin/
			# ls -alh /bin/imgcat
		fi
	fi
}

f30_install_common_software(){
	apt update
	apt install -y \
		sudo \
		tree \
		curl \
		net-tools \
		lsof \
		htop \
		direnv \
		strace \
		rsync \
		sshfs \
		jq \
		aria2 \
		iputils-ping \
		lynx \
		hugo \
		flex \
		sqlite3 \
		mysql-client \
		redis-tools

	# apt install -y ffmpeg

	# NOTE 这个air极其特殊_是必须安装的_所以再这里单独安装一次_确保_泛开发者_把某些步骤跳过了_而没有安装上
	# air
	# go使用Air实时热加载
	GO111MODULE=auto GOPROXY=https://goproxy.cn,https://mirrors.tencent.com/go/,https://mirrors.aliyun.com/goproxy,https://proxy.golang.com.cn,direct go install github.com/cosmtrek/air@latest

}

f36_install_gcc_10_versioin(){
	apt install -y gcc-10
	apt install -y g++-10

	[ -f /usr/bin/gcc-9 ]  && update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-9 120
	[ -f /usr/bin/gcc-10 ] && update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-10 150
	[ -f /usr/bin/g++-9 ]  && update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-9 120
	[ -f /usr/bin/g++-10 ] && update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-10 150

}

# 安装多个用go语言开发的工具
# 这些工具主要是当做linux pipe 管道的小工具的用途
f68_20_install_go_tools(){

	# goawk
	# go语言对于awk的实现.
	# GO111MODULE=auto GOPROXY=https://goproxy.cn,https://mirrors.tencent.com/go/,https://mirrors.aliyun.com/goproxy,https://proxy.golang.com.cn,direct go install github.com/benhoyt/goawk@latest


	# gojq
	# go处理json文件格式.是jq的go语言实现.
	# GO111MODULE=auto GOPROXY=https://goproxy.cn,https://mirrors.tencent.com/go/,https://mirrors.aliyun.com/goproxy,https://proxy.golang.com.cn,direct go install github.com/itchyny/gojq/cmd/gojq@latest


	# glow
	# glow 在TUI,也就是终端中显示markdown文件.
	# 能否很好的显示各种颜色标示的内容
	# 下载时间比较长一点
	GO111MODULE=auto GOPROXY=https://goproxy.cn,https://mirrors.tencent.com/go/,https://mirrors.aliyun.com/goproxy,https://proxy.golang.com.cn,direct go install github.com/charmbracelet/glow@latest

	# 安装谢孟军以前写的类似curl的bat工具
	# Bat 是使用 Go 实现的 CLI 工具,类似 cURL 的工具,可以说是 Go 语言版本的 cURL .
	# Bat 可以用来测试,调试和 HTTP 服务器进行一般的交互.
	# GO111MODULE=auto GOPROXY=https://goproxy.cn,https://mirrors.tencent.com/go/,https://mirrors.aliyun.com/goproxy,https://proxy.golang.com.cn,direct go install github.com/astaxie/bat@latest

	# csv2md
	# csv数据格式文件转为markdown格式的表格_方便嵌入到docsify中去
	GO111MODULE=auto GOPROXY=https://goproxy.cn,https://mirrors.tencent.com/go/,https://mirrors.aliyun.com/goproxy,https://proxy.golang.com.cn,direct go install github.com/uchiiii/csv2md@latest

	# air
	# go使用Air实时热加载
	GO111MODULE=auto GOPROXY=https://goproxy.cn,https://mirrors.tencent.com/go/,https://mirrors.aliyun.com/goproxy,https://proxy.golang.com.cn,direct go install github.com/cosmtrek/air@latest

	# html2md
	# 把html页面转换为markdown格式
	GO111MODULE=auto GOPROXY=https://goproxy.cn,https://mirrors.tencent.com/go/,https://mirrors.aliyun.com/goproxy,https://proxy.golang.com.cn,direct go install github.com/suntong/html2md@latest
		# NOTE 紧随其后的2条容易失败_原因是访问github的问题_不是html2md出现错误
			# html2md -i https://github.com/suntong/html2md | head -3
			# html2md -i https://github.com/JohannesKaufmann/html-to-markdown -s "div.BorderGrid-row.hide-sm.hide-md > div"
		# echo '<strong>Bold Text</strong>' | html2md -i
		# echo '<strong>Bold Text</strong>' | html2md -i --opt-strong-delimiter="__"
		# cat $GOPATH/src/github.com/JohannesKaufmann/html-to-markdown/testdata/TestPlugins/table/input.html | html2md -i -T | head -6
		# cat $GOPATH/src/github.com/JohannesKaufmann/html-to-markdown/testdata/TestPlugins/table/input.html | html2md -i -T --domain example.com | diff -wU 1 $GOPATH/src/github.com/JohannesKaufmann/html-to-markdown/testdata/TestPlugins/table/output.table.golden -
		# cat $GOPATH/src/github.com/JohannesKaufmann/html-to-markdown/testdata/TestPlugins/table/input.html | html2md -i --plugin-table-compat | head -6
		# cat $GOPATH/src/github.com/JohannesKaufmann/html-to-markdown/testdata/TestPlugins/table/input.html | html2md -i --plugin-table-compat --domain example.com | diff -wU 1 $GOPATH/src/github.com/JohannesKaufmann/html-to-markdown/testdata/TestPlugins/table/output.tablecompat.golden -
		# REVIEW 这个是库 Convert HTML to Markdown. Even works with entire websites and can be extended through rules.
			# https://github.com/JohannesKaufmann/html-to-markdown.git

	echo -e "\n\n"

	ls ${GOPATH}/bin

	echo -e "\n\n"
}

# 删除多个配置文件中的go env涉及的环境变量
f70_remove_go_env(){

	# sed命令删除包含特定字符行: 删除包含"xxx"的行
	# sed -i '/xxx/d' filename

	sed -i '/GO111MODULE/d' ~/.profile
	sed -i '/GO111MODULE/d' ~/.zshrc
	sed -i '/GO111MODULE/d' ~/.bashrc

	sed -i '/GOPROXY/d' ~/.profile
	sed -i '/GOPROXY/d' ~/.zshrc
	sed -i '/GOPROXY/d' ~/.bashrc

}

f71_set_go_env(){

	go env -w GO111MODULE=auto ; \
	go env -w GOPROXY=https://goproxy.cn,https://mirrors.tencent.com/go/,https://mirrors.aliyun.com/goproxy,https://proxy.golang.com.cn,direct ; \
	go env -w GOSUMDB=sum.golang.google.cn ; \
	echo "export GO111MODULE=auto" >> ~/.profile ; \
	echo "export GOSUMDB=sum.golang.google.cn" >> ~/.profile ; \
	echo "export GOPROXY=https://goproxy.cn,https://mirrors.tencent.com/go/,https://mirrors.aliyun.com/goproxy,https://proxy.golang.com.cn,direct" >> ~/.profile ; \
	echo "export GO111MODULE=auto" >> ~/.zshrc ; \
	echo "export GOSUMDB=sum.golang.google.cn" >> ~/.zshrc ; \
	echo "export GOPROXY=https://goproxy.cn,https://mirrors.tencent.com/go/,https://mirrors.aliyun.com/goproxy,https://proxy.golang.com.cn,direct" >> ~/.zshrc ; \
	echo "export GO111MODULE=auto" >> ~/.bashrc ; \
	echo "export GOPROXY=https://goproxy.cn,https://mirrors.tencent.com/go/,https://mirrors.aliyun.com/goproxy,https://proxy.golang.com.cn,direct" >> ~/.bashrc ; \
	echo "export GOSUMDB=sum.golang.google.cn" >> ~/.bashrc

}


# ======================================================================================
# make a1_memos_概览老师主要修改了哪些文件
y01_a1_memos_check_revised_files(){
	# grep -H -r wmtag_memo | grep -v "^.git"
	clear
	[[ -f workspace.yml ]] && grep -r -H wmtag_memo_教学 | grep NOTE

	return 0
}

# --------------------------------------------------------------------------------------
# NOTE 被y36_b2_memos_build_and_start_with_livecode_by_air()函数所调用
# b2_01_先构建_前端_生成前端的目标文件夹
y20_b2_01_npm_build_frontend(){
	[[ -f workspace.yml ]] && cd web && npm install && npm run build && touch ./dist/02_web构建时间_$(date '+%Y-%m-%d日_%H:%M:%S秒').md && ls -lah ./dist && cd ..
	return 0
}

# NOTE 被y36_b2_memos_build_and_start_with_livecode_by_air()函数所调用
# b2_02_把上面构建好的_前端目标文件夹_拷贝一份到_后端的./server/dist目录下
# 03_将覆盖./server/dist中原有内容
y22_copy_frontend_dist_to_backend_directory(){

	[[ -f workspace.yml ]] && [[ -d ./server/dist/ ]] && rm -rf ./server/dist/ && cp -r ./web/dist/ ./server/ && touch ./server/dist/03_从web前端dist拷贝时间_$(date '+%Y-%m-%d日_%H:%M:%S秒').md && ls -lah ./server/dist/
	return 0

}

# NOTE 被y36_b2_memos_build_and_start_with_livecode_by_air()函数所调用
# b2_05_为构建后端的go代码做准备
y24_b2_05_prepare_go_env_for_go_build(){
	[[ -f workspace.yml ]] && go mod download -x
	return 0
}

# NOTE 被y36_b2_memos_build_and_start_with_livecode_by_air()函数所调用
# b2_06_为了放置memos后端服务正在运行_运用下述命令_先把memos尝试停止运行_真实查找的是_"air -c scripts/.air.
y26_b2_06_stop_all_memos_server(){
	[[ -f workspace.yml ]] && kill $(ps -ef | grep "air -c scripts/.air.toml" | grep -v grep | awk '{print $2}')
	return 0
}

# NOTE 被y36_b2_memos_build_and_start_with_livecode_by_air()函数所调用
# b2_07_构建后端的go代码_并且放入到./.air/目录下
y28_b2_07_go_build_backend_and_copy_it_to_target_directory(){

	if [[ -f workspace.yml ]]; then
		# 如果已经存在构建的目标_先删除原有的较老的可执行文件版本
		if [[ -f ./.air/memos ]]; then
			rm -f ./.air/memos
		fi
		# 再构建
		go build -o ./.air/memos ./main.go && touch ./.air/07_go_build_memos_构建时间_$(date '+%Y-%m-%d日_%H:%M:%S秒').md && ls -lah ./.air
	fi

	return 0
}

# NOTE 被y36_b2_memos_build_and_start_with_livecode_by_air()函数所调用
# b2_09_调用刚刚生成的go后端可执行文件_设置用户名称是_root_密码是_a123456
# REVIEW 本步骤可以不执行_为了化简学习难度_老师提供了一个初始memos数据库_已经调用过本步骤了
y30_b2_09_memos_setup(){

	[[ -f workspace.yml ]] && ./.air/memos setup --host-username=root --host-password=a123456 --mode dev
	return 0

}

# NOTE 被y36_b2_memos_build_and_start_with_livecode_by_air()函数所调用
# b2_15_执行如下的命令_以dev_开发模式_启动后台
# b2_11_此时应该位于本git仓库的顶层目录下_类似_绝对目录为_/root/RemoteWorking/22.wmsrc_memos 或 /workspace/22.wmsrc_memos
# b2_16_缺省的后台服务端口是_8081
y32_b2_15_start_memos_with_air_liveload(){
	if [[ -f workspace.yml ]]; then
	 	air -c scripts/.air.toml &
	 fi
	return 0
}

# 被 make b2_memos_构建_并_使用air启动memos 所调用
# make b2_memos_构建_并_使用air启动memos
y36_b2_memos_build_and_start_with_livecode_by_air(){
	# echo "$TODO_啥都不做_就是一个占位_保证构建通过"

	# b2_01_先构建_前端_生成前端的目标文件夹
	# y20_b2_01_npm_build_frontend

	# b2_02_把上面构建好的_前端目标文件夹_拷贝一份到_后端的./server/dist目录下
	# 03_将覆盖./server/dist中原有内容
	y22_copy_frontend_dist_to_backend_directory

	# b2_05_为构建后端的go代码做准备
	y24_b2_05_prepare_go_env_for_go_build

	# b2_06_为了放置memos后端服务正在运行_运用下述命令_先把memos尝试停止运行_真实查找的是_"air -c scripts/.air.
	y26_b2_06_stop_all_memos_server

	# NOTE 加入如下的行_帮助_泛开发者_体验最简单的_暂停运行_打开其他终端_查看中间结果
	# pause_60_second
	# echo -e "\n运行在 $(pwd)/${BASH_SOURCE[${#BASH_SOURCE[@]} - 1]} 脚本的 ${FUNCNAME} 函数中 ${LINENO} 行\n" >> ${WMTAG_LOCK_FILE}

	# b2_07_构建后端的go代码_并且放入到./.air/目录下
	y28_b2_07_go_build_backend_and_copy_it_to_target_directory

	# b2_09_调用刚刚生成的go后端可执行文件_设置用户名称是_root_密码是_a123456
	# REVIEW 本步骤可以不执行_为了化简学习难度_老师提供了一个初始memos数据库_已经调用过本步骤了
	# y30_b2_09_memos_setup

	# b2_15_执行如下的命令_以dev_开发模式_启动后台
	# b2_11_此时应该位于本git仓库的顶层目录下_类似_绝对目录为_/root/RemoteWorking/22.wmsrc_memos 或 /workspace/22.wmsrc_memos
	# b2_16_缺省的后台服务端口是_8081
	y32_b2_15_start_memos_with_air_liveload

	return 0

}

# --------------------------------------------------------------------------------------

# make k9_memos_stop_all
y66_k9_memos_stop_all(){
	# echo "$TODO_啥都不做_就是一个占位_保证构建通过"
	[[ -f workspace.yml ]] && kill $(ps -ef | grep "air -c scripts/.air.toml" | grep -v grep | awk '{print $2}')
	return 0
}

# make p11_开启_cloudstudio自动运行预览
y72_p11_change_vscode_preview_file_content(){
	# echo "$TODO_啥都不做_就是一个占位_保证构建通过"

	# NOTE 方法就是把内容暴力写入.vscode/preview.yml中
	# cloud studio中vscode_browser_preview设置文件
	CS_VSCODE_SETTINGS_BROWSER_PREVIEW=./.vscode/preview.yml
	# -----------------------------------------------------------
	# '嵌入文档涵盖了生成脚本的主体部分.

	(
	cat <<'EOF'
# 这个是生成的_由如下函数_y72_p11_change_vscode_preview_file_content
autoOpen: true
apps:
  - port: 5000
    run: pip3 install -i https://mirrors.tencent.com/pypi/simple/ -r ./.wmstudy/wm28.preview/web/requirements.txt && python3 ./.wmstudy/wm28.preview/web/app.py
    root: .
    name: 我们一起Go_公告板
    # 我们在后继的讲解中_需要用到_机械工业出版社_2022年01月出版的_廖显东老师_这本书中部分内容
    description: cloud_studio_go学习_廖显东_老师_goAdvanced_书籍介绍_启动_公告板
    autoOpen: true
EOF
	) > ${CS_VSCODE_SETTINGS_BROWSER_PREVIEW}

git add .vscode/preview.yml
git commit -m w2011_wmtemp_by_y72_p11_change_vscode_preview_file_content_函数

[[ -f $(which cloudstudio) ]] && [[ -f workspace.yml ]] && cloudstudio .vscode/preview.yml

}

# make p14_关闭_cloudstudio自动运行预览
y74_p14_change_vscode_preview_file_content_as_original(){
	# echo "$TODO_啥都不做_就是一个占位_保证构建通过"

	# NOTE 方法就是把内容暴力写入.vscode/preview.yml中
	# NOTE 把接近原始的内容给写入了
	# cloud studio中vscode_browser_preview设置文件
	CS_VSCODE_SETTINGS_BROWSER_PREVIEW=./.vscode/preview.yml
	# -----------------------------------------------------------
	# '嵌入文档涵盖了生成脚本的主体部分.

	(
	cat <<'EOF'
# 这个是生成的_由如下函数_y74_p11_change_vscode_preview_file_content_as_origina
# autoOpen: true
# apps:
#   - port: 5000
#     run: make help
#     # run: pip3 install -i https://mirrors.tencent.com/pypi/simple/ -r ./.wmstudy/wm28.preview/web/requirements.txt && python3 ./.wmstudy/wm28.preview/web/app.py
#     root: .
#     name: 我们一起Go_公告板
#     # 我们在后继的讲解中_需要用到_机械工业出版社_2022年01月出版的_廖显东老师_这本书中部分内容
#     description: cloud_studio_go学习_廖显东_老师_goAdvanced_书籍介绍_启动_公告板
#     autoOpen: true
EOF
	) > ${CS_VSCODE_SETTINGS_BROWSER_PREVIEW}

git add .vscode/preview.yml
git commit -m w2014_wmtemp_by_y74_p14_change_vscode_preview_file_content_as_original_函数

}

# ======================================================================================


# 本git仓库特有的下拉好多个docker镜像
y77_docker_pull_images(){

	apt update && apt-get install -y mysql-client redis-tools

	docker pull golang:1.19.3-alpine

}

# ======================================================================================
# NOTE 这里是为了wc24_apue_3rd_第3版_构建与编译_的依赖库
wc24_30_install_some_libraries(){
	apt install libbsd-dev
	return 0
}

# ======================================================================================

all(){

	f16_cs_vs_settings_user_update

	f20_linux_git_setting

	f28_20_install_some_wmstudy_bin_tools

	f30_install_common_software

	f36_install_gcc_10_versioin

	f70_remove_go_env

	f71_set_go_env

	wc24_30_install_some_libraries

	# f68_20_install_go_tools

	# y77_docker_pull_images

}

f82_main(){

	# 判断是否位于腾云扣钉的cloudstudio工作空间中
	if [[ -f $(which cloudstudio) ]]; then
		# 已经在cloudstudio工作空间中
		# 可以执行
		all
	else
		echo "本脚本只在cloudstudio工作空间中执行_不满足执行的条件_all_函数_没有_被执行"
	fi

	# 不论是位于cloudstudio都执行下面的操作
	# 由于某些文件会产生变化_我们创造一个新的本地分支
	# 例如: data/config/application.yml
	# git checkout -b wmstudy_wmlocal
	# git switch wmstudy_wmlocal

	return 0

}

# eval f82_main

# 下面是_正式_的入口
[ -z "$1" ] && eval f82_main || eval $1 $*
