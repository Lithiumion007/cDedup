

#!/bin/bash
code_line=0
comment_line=0
blank_line=0

# 指定目录路径
directory="/home/cyf/raid0/cloc/llma"

# 使用 for 循环遍历目录下的所有文件夹
for dir in "$directory"/*/; do
    # 输出文件夹名称（不包括路径）
    cloc ${dir%/} | grep C++| grep -v C/C++ |  awk  '{print $3,$4,$5}' 

done


