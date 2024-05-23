touch
===
![](../../../wmimages/sg77_石灰吟_于谦.png)
[//]: # (wmtag_memo_下面是我们修改的_开始)
```bash
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
```
[//]: # (wmtag_memo_下面是我们修改的_结束)

创建新的空文件

## 补充说明

**touch命令** 有两个功能：一是用于把已存在文件的时间标签更新为系统当前的时间（默认方式），它们的数据将原封不动地保留下来；二是用来创建新的空文件。

###  语法

```shell
touch(选项)(参数)
```

###  选项

```shell
-a：或--time=atime或--time=access或--time=use  只更改存取时间；
-c：或--no-create  不建立任何文件；
-d：<时间日期> 使用指定的日期时间，而非现在的时间；
-f：此参数将忽略不予处理，仅负责解决BSD版本touch指令的兼容性问题；
-m：或--time=mtime或--time=modify  只更该变动时间；
-r：<参考文件或目录>  把指定文件或目录的日期时间，统统设成和参考文件或目录的日期时间相同；
-t：<日期时间>  使用指定的日期时间，而非现在的时间；
--help：在线帮助；
--version：显示版本信息。
```

###  参数

文件：指定要设置时间属性的文件列表。

###  实例

```shell
touch ex2
```

在当前目录下建立一个空文件ex2，然后，利用`ls -l`命令可以发现文件ex2的大小为0，表示它是空文件。

批量创建文件

```shell
touch file{1..5}.txt
```

创建 `job1.md` 文件，并写入 `job 1`

```shell
echo "job 1" > job1.md
```
