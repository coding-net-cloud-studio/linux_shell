 # (leading space required for Xenix /bin/sh)

#
# Determine the type of *ix operating system that we're
# running on, and echo an appropriate value.
# This script is intended to be used in Makefiles.
# (This is a kludge.  Gotta be a better way.)
#

#
# 确定我们正在运行的 *ix 操作系统的类型,并回显一个适当的值.
# 该脚本旨在用于 Makefiles 中.
# (这是一种权宜之计.一定有更好的方法.)
#

# 根据不同的操作系统类型设置PLATFORM变量
case `uname -s` in
# 如果操作系统是FreeBSD
"FreeBSD")
	# 将PLATFORM变量设置为freebsd
	PLATFORM="freebsd"
	;;
# 如果操作系统是Linux
"Linux")
	# 将PLATFORM变量设置为linux
	PLATFORM="linux"
	;;
# 如果操作系统是Darwin(即Mac OS X)
"Darwin")
	# 将PLATFORM变量设置为macos
	PLATFORM="macos"
	;;
# 如果操作系统是SunOS(即Solaris)
"SunOS")
	# 将PLATFORM变量设置为solaris
	PLATFORM="solaris"
	;;
# 如果操作系统是其他类型
*)
	# 打印错误信息到标准错误输出
	echo "Unknown platform" >&2
	# 以非零状态码1退出,表示有错误发生
	exit 1
esac
# 打印操作系统类型到标准输出
echo $PLATFORM
# 以零状态码退出,表示程序正常结束
exit 0

