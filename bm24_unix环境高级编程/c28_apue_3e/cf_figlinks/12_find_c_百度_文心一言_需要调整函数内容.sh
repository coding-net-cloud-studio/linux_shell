#!/bin/bash

# NOTE 提示词
# ubuntu 22.04操作系统上,我有一个名称为cf_figlinks的目录下,有205个c作为文件名后缀的源文件,其中部分c源代码中具有main函数,而其他的c语言源代码中没有main函数,你写一个bash的函数,用grep -r main .等找出所有的具有main函数的源代码的文件名,再写第2个bash函数,找出所有没有main函数的文件名

# NOTE ============================================

# 在Ubuntu 22.04或任何支持bash的环境中,你可以通过编写两个简单的bash函数来实现你的需求.第一个函数用于找出所有包含main函数的.c文件,第二个函数则用于找出所有不包含main函数的.c文件.

# 第一个bash函数:找出包含main函数的.c文件

find_main_files() {
    # 在指定目录下递归查找包含"main"的.c文件
    # 注意:这里假设main函数在文件中至少会完整出现一次,即至少包含"main("
    grep -rlE 'main\(' "$1"/*.c | while read -r file; do
        echo "$file"
    done
}

# 使用方法
# 假设当前目录是你的cf_figlinks目录
# find_main_files "$(pwd)"

# 注意:这里使用了-E选项来启用扩展的正则表达式,使得我们可以直接搜索main(这样的模式.-r或-R选项表示递归搜索,-l选项让grep只输出包含匹配项的文件名.

# 第二个bash函数:找出不包含main函数的.c文件
# 这个稍微复杂一些,因为我们需要首先找出所有.c文件,然后排除那些包含main函数的文件.

# NOTE --------------------------------------------
# 定义一个函数,用于查找指定目录下所有不包含 main 函数的 C 文件
find_non_main_files() {
    # 将传入的参数赋值给变量 dir,即要搜索的目录
    local dir="$1"

    # 使用 find 命令查找指定目录下所有以.c 结尾的文件,并将结果存储在数组 all_c_files 中
    local all_c_files=($(find "$dir" -type f -name '*.c'))

    # 使用 grep 命令查找指定目录下所有包含 main 函数的文件,并将结果存储在数组 main_files 中
    local main_files=($(grep -rlE 'main\(' "$dir" | cut -d'/' -f-$(echo "$dir" | wc -w)))

    # 定义一个数组 non_main_files,用于存储不包含 main 函数的文件
    local non_main_files=()

    # 遍历所有 C 文件,如果文件不在 main_files 数组中,则将其添加到 non_main_files 数组中
    for file in "${all_c_files[@]}"; do
        if [[ ! " ${main_files[*]} " =~ " $file " ]]; then
            non_main_files+=("$file")
        fi
    done

    # 输出不包含 main 函数的文件
    for file in "${non_main_files[@]}"; do
        echo "$file"
    done
}


# 使用方法
# 假设当前目录是你的cf_figlinks目录
find_non_main_files "$(pwd)"

# 注意:

# 这个脚本中,cut命令用于处理grep输出的路径,以匹配find命令生成的路径格式(如果路径中有空格或特殊字符,这可能会出问题).
# 但是,如果你的文件名不包含空格或特殊字符,并且你的目录结构不复杂(即没有深度嵌套的子目录),这个简单的cut方法可能就足够了.
# 另一种更健壮的方法是直接处理find命令的输出,而不是将其拆分为两个步骤(先找所有.c文件,再排除).
# 但这里为了清晰起见,我保持了两个步骤.
# 使用数组来存储文件名,并进行了数组比较来找出不包含main的文件.
# 这种方法在文件名数量很多时可能效率不高,但对于你的用例(205个文件)来说应该是足够的.

# NOTE ============================================