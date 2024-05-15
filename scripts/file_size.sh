#!/bin/bash

# 检查输入参数是否为空
if [ $# -eq 0 ]; then
    echo "Usage: $0 <directory>"
    exit 1
fi

# 获取目录路径
directory="$1"

# 检查目录是否存在
if [ ! -d "$directory" ]; then
    echo "Error: Directory '$directory' does not exist."
    exit 1
fi

# 统计目录下所有.cc、.cpp和.c文件的大小
total_size=$(find "$directory" -type f \( -name "*.cc" -o -name "*.cpp" -o -name "*.c" \) -exec du -b {} + | awk '{total += $1} END {print total}')
num_files=$(find "$directory" -type f \( -name "*.cc" -o -name "*.cpp" -o -name "*.c" \) | wc -l)
average_size=$((total_size / num_files))

echo "Total size of .cc, .cpp and .c files in '$directory': $total_size bytes"
echo "Number of .cc, .cpp and .c files: $num_files"
echo "Average file size: $average_size bytes"
