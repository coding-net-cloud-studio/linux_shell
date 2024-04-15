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

# NOTE 卸载所有在_cloudstudio_中已经_安装上的_vscode的扩展
f3770_a22_vs_ext_uninstall_all_vscode_extensions(){

    if [[ -f $(which cloudstudio) ]]; then
		# NOTE 位于cloudstudio的工作空间中
		if [[ $(cloudstudio --list-extensions | grep -v 安装的扩展 | wc -l ) -gt 0 ]]; then

			# NOTE 如果发现_具有_已经安装的_vscode扩展
            echo "f3370_b10_发现_已经_安装的_vscode扩展_需要被_卸载"

            for one_installed_extension in `cloudstudio --list-extensions | grep -v 安装的扩展`
            do
                echo "正在准备_卸载_如下的vscode扩展_${one_installed_extension}"
                cloudstudio --uninstall-extension ${one_installed_extension}
            done
        else
            echo "f3370_b20_没有任何_已经_安装的_vscode扩展_需要被_卸载"
		fi
	fi

    return 0
}

main(){

	export v33_OLDPWD=$(pwd)

    if [[ -f $(which cloudstudio) ]]; then

		if [[ ! -f /root/.local/share/code-server/extensions/extensions.json ]]; then 
			mkdir -p /root/.local/share/code-server/extensions/
			echo "[]" > /root/.local/share/code-server/extensions/extensions.json
		fi

        f3770_a22_vs_ext_uninstall_all_vscode_extensions
    fi

    # NOTE 卸载所有在_cloudstudio_中已经_安装上的_vscode的扩展
	cd ${v33_OLDPWD}

    return 0

}

main