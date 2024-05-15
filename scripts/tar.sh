#!/bin/bash

# 遍历当前目录下所有的 tar.gz 文件
for file in *.tar.gz; do
    # 如果文件存在
    if [ -f "$file" ]; then
        # 获取文件名（不含扩展名）
        filename=$(basename "$file" .tar.gz)
        # 创建一个和文件名同名的文件夹
        mkdir "$filename"
        # 解压文件到刚创建的文件夹
        tar -xzf "$file" -C "$filename"
        # 提示解压进度
        echo "解压 $file 完成"
    fi
done

echo "所有 tar.gz 文件解压完成"

