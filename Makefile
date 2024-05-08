CC = gcc
LIB = -lcrypto -lz -lstdc++ -llz4 -lpthread -lstdc++fs \
      -ldoca_sha -ldoca_common -ldoca_argp -L/opt/mellanox/doca/lib/aarch64-linux-gnu/ \
	  -L./utils/lz4-1.9.1/lib

INC = -I/opt/mellanox/doca/include \
	  -I/opt/mellanox/doca/samples \
	  -I/opt/mellanox/doca/applications/common/src \
	  -I./include -I./utils -I./utils/lz4-1.9.1/lib 

SRC = main.cpp ./src/fastcdc.cpp ./src/full_file_deduplicater.cpp ./src/merkle_tree.cpp \
	  ./src/MetadataManager.cpp ./src/ContainerCache.cpp ./src/ChunkCache.cpp \
	  ./src/compressor.cpp \
      ./utils/cJSON.c \
	  ./src/pipeline.cpp ./src/pipeline_read.cpp ./src/pipeline_chunk.cpp ./src/pipeline_hash.cpp ./src/pipeline_dedup.cpp \
	  ./src/sync_queue.cpp ./src/queue.cpp ./src/jcr.cpp \
	  /opt/mellanox/doca/applications/common/src/utils.c \
	  /opt/mellanox/doca/samples/common.c \

EXE_NAME = cDedup

amazing:
	$(CC) $(SRC) $(INC) $(LIB) -o $(EXE_NAME) -g -O0 -std=c++17

clean:
	rm $(EXE_NAME) \
	rm TEST/metadata/FFFP.meta TEST/metadata/fingerprints.meta \
	rm -fr TEST/metadata/FileRecipes TEST/metadata/FULL_FILE_STORAGE TEST/metadata/Containers \
	rm TEST/metadata/L1.meta TEST/metadata/L2.meta TEST/metadata/L3.meta TEST/metadata/L4.meta TEST/metadata/L5.meta TEST/metadata/L6.meta

