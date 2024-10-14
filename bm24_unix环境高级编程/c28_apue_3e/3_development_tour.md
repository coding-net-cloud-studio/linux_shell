# 在cloudstudio云上工作室中构建与执行memos

## 步骤

### a1_memos_cloudstudio工作空间_初始化

   ```bash
   # TODO a1_01_判断是否处于cloudstudio工作空间中
   [[ -f workspace.yml ]] && make 1_check
   ```

   ```bash
   # TODO a1_02_初始化cloudstudio工作空间
   # 在cloudstudio工作空间中运行_只需要运行1次就可以了
   [[ -f workspace.yml ]] && make 2_init
   ```

   ```bash
   # TODO a1_03_设置自己的vscode扩展自定义组合
   [[ -f workspace.yml ]] && make 3_在cloudstudio中_安装_vscode扩展
   ```

   ```bash
   # TODO a1_04_安装几个go语言编写的小工具
   # 安装几个go语言编写的小工具_包括_处理markdown的工具
   [[ -f workspace.yml ]] && make 4_在cloudstudio中_安装_go语言_小工具
   ```

   ```bash
   # TODO a1_a1_概览老师主要修改了哪些文件
   [[ -f workspace.yml ]] && make a1_memos_概览老师主要修改了哪些文件
   ```

### b2_memos_构建_与_使用air启动memos
- 在修改go语言编写的后端的时候,可以自动重载_liveload

   ```bash
   # TODO b2_01_先构建_前端_生成前端的目标文件夹
   [[ -f workspace.yml ]] && cd web && npm install && npm run build && touch ./dist/02_web构建时间_$(date '+%Y-%m-%d日_%H:%M:%S秒').md && ls -lah ./dist && cd ..
   ```

   ```bash
   # TODO b2_02_把上面构建好的_前端目标文件夹_拷贝一份到_后端的./server/dist目录下
   # 03_上述覆盖了./server/dist中原有内容
   [[ -f workspace.yml ]] && [[ -d ./server/dist/ ]] && rm -rf ./server/dist/ && cp -r ./web/dist/ ./server/ && touch ./server/dist/03_从web前端dist拷贝时间_$(date '+%Y-%m-%d日_%H:%M:%S秒').md && ls -lah ./server/dist/
   ```

   ```bash
   # TODO b2_05_为构建后端的go代码做准备
   [[ -f workspace.yml ]] && go mod download -x
   ```

   ```bash
   # TODO b2_06_为了放置memos后端服务正在运行_运用下述命令_先把memos尝试停止运行_真实查找的是_"air -c scripts/.air.toml"
   [[ -f workspace.yml ]] && kill $(ps -ef | grep "air -c scripts/.air.toml" | grep -v grep | awk '{print $2}') || echo "运行成功_不必关注其他信息\n"
   ```

   ```bash
   # TODO b2_07_构建后端的go代码_并且放入到./.air/目录下
   [[ -f workspace.yml ]] && [[ -f ./.air/memos ]] && rm -f ./.air/memos 
   [[ -f workspace.yml ]] && go build -x -v -o ./.air/memos ./main.go && touch ./.air/07_go_build_memos_构建时间_$(date '+%Y-%m-%d日_%H:%M:%S秒').md && ls -lah ./.air
   ```

   ```bash
   # b2_09_调用刚刚生成的go后端可执行文件_设置用户名称是_root_密码是_a123456
   [[ -f workspace.yml ]] && ./.air/memos setup --host-username=root --host-password=a123456 --mode dev
   ```

   ```bash
   # b2_11_此时应该位于本git仓库的顶层目录下_类似_绝对目录为_/root/RemoteWorking/22.wmsrc_memos
   # TODO b2_15_执行如下的命令_以dev_开发模式_启动后台
   # b2_16_缺省的后台服务端口是_8081
   [[ -f workspace.yml ]] && air -c scripts/.air.toml
   ```

### c3_memos_在浏览器中登录
	- cloudstudio 会自动打开一个链接地址
	- 类似于 https://ohbott-buwkox-8081.preview.myide.io/ [端口号是8081 由air -c scripts/.air.toml 命令启动的]
   - 类似于 https://ohbott-buwkox-8118.preview.myide.io/ [端口号是8118 由docker run命令启动的]
   - 类似于 https://ohbott-buwkox-8228.preview.myide.io/ [端口号是8228 由docker-compose up -d 命令启动的]
	- 每个学生打开的链接都不相同
	- 选择中文语言
	- 用户名称: root
	- 用户密码: a123456

   ```bash
   # TODO c3_16_运用下述命令_尝试把memos尝试停止运行
   # 真实查找的是_"air -c scripts/.air.toml"
   [[ -f workspace.yml ]] && kill $(ps -ef | grep "air -c scripts/.air.toml" | grep -v grep | awk '{print $2}') || echo "运行成功_不必关注其他信息\n"
   ```

### d4_memos_尝试构建docker镜像
   ```bash
   # TODO d4_01_下拉构建过程中需要用的镜像_需要等待比较长一点的时间
   [[ -f workspace.yml ]] && docker pull golang:1.19.3-alpine3.16
   ```

   ```bash
   # TODO d4_03_尝试构建镜像
   [[ -f workspace.yml ]] && docker build -f ./Dockerfile -t neosmemo/memos:latest .
   ```

   ```bash
   # TODO d4_05_尝试在上面新构建的镜像_运行一个临时容器_暴露的端口号是_8118_端口
   [[ -f workspace.yml ]] && docker run -it --rm --name m12_cloudstudio_memos -p 8118:8118 neosmemo/memos:latest
   ```

   ```bash
   # TODO d4_07_尝试停止上面运行起来的m12_cloudstudio_memos容器_执行本步骤_即使是出现错误_也没有问题_可以继续进行
   [[ -f workspace.yml ]] && docker stop m12_cloudstudio_memos
   ```

   ```bash
   # TODO d4_09_尝试使用docker-compose运行起来_这次使用的外露端口是_8228_端口
   [[ -f workspace.yml ]] && docker-compose up -d
   ```

   ```bash
   # TODO d4_11_尝试停止docker-compose启动容器
   [[ -f workspace.yml ]] && docker-compose down
   ```

### p11_风里雨里_我们等你
   ```bash
   # TODO p11_风里雨里_我们等你
   [[ -f workspace.yml ]] && make p11_风里雨里_我们等你
   ```

### p12_开启_cloudstudio自动运行预览
   ```bash
   # TODO p12_开启_cloudstudio自动运行预览
   [[ -f workspace.yml ]] && make p12_开启_cloudstudio自动运行预览
   ```