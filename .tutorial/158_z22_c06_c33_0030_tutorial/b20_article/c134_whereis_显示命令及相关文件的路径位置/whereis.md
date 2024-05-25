whereis
===
![](../../../wmimages/wm71_club.png)


[//]: # (wmtag_memo_下面是我们修改的_开始)


```bash
demo_11(){
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
demo_11
```


```bash
demo_31(){
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
#demo_31
```


[//]: # (wmtag_memo_下面是我们修改的_结束)



查找二进制程序、代码等相关文件路径

## 补充说明

**whereis命令** 用来定位指令的二进制程序、源代码文件和man手册页等相关文件的路径。

whereis命令只能用于程序名的搜索，而且只搜索二进制文件（参数-b）、man说明文件（参数-m）和源代码文件（参数-s）。如果省略参数，则返回所有信息。

和find相比，whereis查找的速度非常快，这是因为linux系统会将 系统内的所有文件都记录在一个数据库文件中，当使用whereis和下面即将介绍的locate时，会从数据库中查找数据，而不是像find命令那样，通 过遍历硬盘来查找，效率自然会很高。 但是该数据库文件并不是实时更新，默认情况下时一星期更新一次，因此，我们在用whereis和locate 查找文件时，有时会找到已经被删除的数据，或者刚刚建立文件，却无法查找到，原因就是因为数据库文件没有被更新。

###  语法

```shell
whereis(选项)(参数)
```

###  选项

```shell
-b：只查找二进制文件；
-B<目录>：只在设置的目录下查找二进制文件；
-f：不显示文件名前的路径名称；
-m：只查找说明文件；
-M<目录>：只在设置的目录下查找说明文件；
-s：只查找原始代码文件；
-S<目录>只在设置的目录下查找原始代码文件；
-u：查找不包含指定类型的文件。
```

###  参数

指令名：要查找的二进制程序、源文件和man手册页的指令名。

###  实例

将相关的文件都查找出来

```shell
[root@localhost ~]# whereis tomcat
tomcat:

[root@localhost ~]# whereis svn
svn: /usr/bin/svn /usr/local/svn /usr/share/man/man1/svn.1.gz
```

说明：tomcat没安装，找不出来，svn安装找出了很多相关文件

只将二进制文件查找出来 

```shell
[root@localhost ~]# whereis -b svn
svn: /usr/bin/svn /usr/local/svn

[root@localhost ~]# whereis -m svn
svn: /usr/share/man/man1/svn.1.gz

[root@localhost ~]# whereis -s svn
svn:
```

说明：`whereis -m svn`查出说明文档路径，`whereis -s svn`找source源文件。


