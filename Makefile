# 使用 g++ 作为默认编译器（更适合混合 C/C++ 项目）
CC = g++

# 库链接
LIB = -lcrypto -lz -llz4 -lpthread -lstdc++fs

# 源文件（C 和 C++ 混合）
C_SRC = ./utils/cJSON.c
CPP_SRC = main.cpp ./src/fastcdc.cpp \
          ./src/MetadataManager.cpp ./src/ContainerCache.cpp ./src/ChunkCache.cpp \
          ./src/compressor.cpp ./src/jcr.cpp

EXE_NAME = cDedup

# 分别设置 C 和 C++ 的编译标志
CFLAGS = -std=c11 -I./include -I./utils -I./utils/lz4-1.9.1/lib
CXXFLAGS = -std=c++17 -I./include -I./utils -I./utils/lz4-1.9.1/lib

# 链接标志
LDFLAGS = -L./utils/lz4-1.9.1/lib -g -O3

amazing:
	$(CC) $(CXXFLAGS) $(CPP_SRC) $(C_SRC) $(LIB) $(LDFLAGS) -o $(EXE_NAME)