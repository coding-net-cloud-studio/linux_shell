
all: dbm1 dbm2 lock1 lock2 lock3 lock4 lock5 memory1 memory2 memory3 memory4 memory5a memory5b memory6

# https://www.cnblogs.com/yangbajing/archive/2009/11/17/1604765.html
# Ubuntu中默认安装的是gdbm数据库,应该使用gdbm实现来访问数据库.若还是需要以dbm若ndbm方式来访问数据库的话就需要使用gdbm-ndbm.h头文件,编译链接时应使用-lgdbm_compat链接选项
# gcc -lgdbm_compat -o dbm1 dbm1.c

INCLUDE=/usr/include/gdbm
LIBS=-lgdbm
# On some systems, you may have to change the line above to include the
# compatability library, as shown below. Note that the compatability library
# may have to come first.
# LIBS=-lgdbm_compat -lgdbm

# You make also need to edit the files dbm1.c and dbm2.c to change the
# include of  <ndbm.h> to be <gdbm-ndbm.h>


dbm1: dbm1.c
	gcc -I$(INCLUDE) $< -o $@ $(LIBS)

dbm2: dbm2.c
	gcc -I$(INCLUDE) $< -o $@ $(LIBS)


clean:
	rm -f dbm1 dbm2 lock1 lock2 lock3 lock4 lock5 memory1 memory2 memory3 memory4 memory5a memory5b memory6


