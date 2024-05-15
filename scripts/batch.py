# 由于单独写gcc和linux的每个文件，会导致一个代码文件对应一个容器，性能很差，所以现在把所有的c cpp cc文件合并成一个文件；
# 这样10个版本的代码就是10个大文件；
# 其实leveldb和llma也有这个问题，只不过这两个项目代码相对较少，所以问题不严重；

import os

def find_files(directory, extensions):
    files = []
    for root, _, filenames in os.walk(directory):
        for filename in filenames:
            if filename.endswith(extensions):
                files.append(os.path.join(root, filename))
    return files

def concatenate_files(files, output_file):
    with open(output_file, 'wb') as outfile:  # 打开文件时指定为二进制模式
        for file in files:
            with open(file, 'rb') as infile:  # 打开文件时指定为二进制模式
                outfile.write(infile.read() + b'\n')  # 写入时需要使用字节串

def main():
    directory = '/home/cyf/raid0/cloc/linux/v6.9-rc5'
    output_file = '../codeLinux/linux10.cpp'
    extensions = ('.c', '.cpp', '.cc')

    files = find_files(directory, extensions)
    concatenate_files(files, output_file)

    print("文件合并完成！")

if __name__ == "__main__":
    main()



