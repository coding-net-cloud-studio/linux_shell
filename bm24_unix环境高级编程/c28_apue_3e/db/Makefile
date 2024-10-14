# 定义根目录为上一级目录
ROOT=..

# 使用shell脚本获取系统类型,并赋值给PLATFORM变量
PLATFORM=$(shell $(ROOT)/systype.sh)

# 根据不同的平台包含相应的Makefile定义文件
include $(ROOT)/Make.defines.$(PLATFORM)

# 定义静态库文件名
LIBMISC	= libapue_db.a

# 定义公共目标文件
COMM_OBJ = db.o


ifeq "$(PLATFORM)" "solaris"
  LDCMD=$(LD) -64 -G -Bdynamic -R/lib/64:/usr/ucblib/sparcv9 -o libapue_db.so.1 -L/lib/64 -L/usr/ucblib/sparcv9 -L$(ROOT)/lib -lapue db.o
  EXTRALD=-m64 -R.
else
# wmtag_memo_我修改了这里_开始
#	LDCMD=$(CC) -shared -Wl,-dylib -o libapue_db.so.1 -L$(ROOT)/lib -lapue -lc db.o
	LDCMD=$(CC) -shared -o libapue_db.so.1 -L$(ROOT)/lib -lapue -lc db.o
# wmtag_memo_我修改了这里_结束
endif

ifeq "$(PLATFORM)" "linux"
  EXTRALD=-Wl,-rpath=.
endif
ifeq "$(PLATFORM)" "freebsd"
  EXTRALD=-R.
endif
ifeq "$(PLATFORM)" "macos"
  EXTRALD=-R.
endif

all: libapue_db.so.1 t4 $(LIBMISC)

# 定义如何从COMM_OBJ和LIBAPUE构建libapue_db.a静态库
libapue_db.a:	$(COMM_OBJ) $(LIBAPUE)
		# 使用ar工具将COMM_OBJ归档到LIBMISC
		$(AR) rsv $(LIBMISC) $(COMM_OBJ)
		# 使用ranlib工具更新LIBMISC的符号索引
		$(RANLIB) $(LIBMISC)


# 定义libapue_db.so.1这个目标文件,它依赖于db.c文件和$(LIBAPUE)变量指定的文件
libapue_db.so.1:	db.c $(LIBAPUE)
		# 使用$(CC)编译器,-fPIC选项表示生成位置无关代码,$(CFLAGS)是编译器的其他选项
		# -c选项表示只编译不链接,生成db.o目标文件
		$(CC) -fPIC $(CFLAGS) -c db.c
		# 使用$(LDCMD)变量指定的链接命令来链接生成动态库
		$(LDCMD)
		# 创建一个符号链接,使得libapue_db.so指向libapue_db.so.1
		ln -s libapue_db.so.1 libapue_db.so


# 定义t4目标的规则
t4:	$(LIBAPUE)  # 确保依赖库$(LIBAPUE)已经生成
		$(CC) $(CFLAGS) -c -I. t4.c  # 使用$(CC)编译器,$(CFLAGS)编译选项,-c表示只编译不链接,-I.表示包含当前目录,编译t4.c文件生成t4.o目标文件
		$(CC) $(EXTRALD) -o t4 t4.o -L$(ROOT)/lib -L. -lapue_db -lapue  # 使用$(CC)链接器,$(EXTRALD)额外的链接选项,-o t4指定输出的可执行文件名为t4,-L指定库搜索路径,-l指定链接的库文件


# 清理规则:删除编译生成的文件,包括目标文件,可执行文件,核心转储文件等
clean:
	rm -f *.o a.out core temp.* $(LIBMISC) t4 libapue_db.so.* *.dat *.idx libapue_db.so

# 包含外部库的Makefile规则,$(ROOT)是根目录变量,Make.libapue.inc是包含的外部Makefile片段
include $(ROOT)/Make.libapue.inc

