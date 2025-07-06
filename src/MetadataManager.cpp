#include "MetadataManager.h"
#include "assert.h" 
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

MetadataManager *GlobalMetadataManagerPtr;

void MetadataManager::load(){
    printf("-----------------------Loading FP-index-----------------------\n");
    printf("Loading index..\n");

    unsigned char* metadata_cache = (unsigned char*)malloc(FILE_CACHE);
    int fd = open(this->metadata_file_path.c_str(), O_RDONLY);
    if(fd < 0)
        printf("MetadataManager::load error\n");
    int n = read(fd, metadata_cache, FILE_CACHE);
    int meta_size = sizeof(SHA1FP) + sizeof(ENTRY_VALUE);
    int entry_count = n/meta_size;
    SHA1FP tmp_fp;
    ENTRY_VALUE tmp_value;

    for(int i=0; i<=entry_count-1; i++){
        memcpy(&tmp_fp, metadata_cache+i*meta_size, sizeof(SHA1FP));
        memcpy(&tmp_value, metadata_cache+i*meta_size + sizeof(SHA1FP), sizeof(ENTRY_VALUE));

        this->fp_table_origin.emplace(tmp_fp, tmp_value);
    }

    close(fd);
    free(metadata_cache);
    printf("metadata table load %d items\n", entry_count);
}

void MetadataManager::save() {
    printf("-----------------------Saving FP-index-----------------------\n");
    
    // 使用 RAII 包装文件描述符
    class FileDescriptor {
        int fd = -1;
    public:
        FileDescriptor(const std::string& path, int flags, mode_t mode) {
            fd = open(path.c_str(), flags, mode);
            if (fd < 0) {
                throw std::runtime_error("open failed: " + std::string(strerror(errno)));
            }
        }
        ~FileDescriptor() { if (fd != -1) close(fd); }
        operator int() const { return fd; }
        
        void writeAll(const void* buf, size_t count) {
            const uint8_t* p = static_cast<const uint8_t*>(buf);
            while (count > 0) {
                ssize_t written = ::write(fd, p, count);
                if (written == -1) {
                    throw std::runtime_error("write failed: " + std::string(strerror(errno)));
                }
                count -= written;
                p += written;
            }
        }
    };

    try {
        FileDescriptor fd(this->metadata_file_path, O_WRONLY | O_CREAT | O_TRUNC, 0777);
        
        // 使用 lseek + ftruncate 清空文件更高效
        if (ftruncate(fd, 0) == -1) {
            throw std::runtime_error("ftruncate failed: " + std::string(strerror(errno)));
        }

        int count = 0;
        
        // 写入新增项
        for (const auto& item : this->fp_table_added) {
            fd.writeAll(&item.first, sizeof(SHA1FP));
            fd.writeAll(&item.second, sizeof(ENTRY_VALUE));
            count++;
        }
        printf("New added items: %d\n", count);

        // 写入原始项
        for (const auto& item : this->fp_table_origin) {
            fd.writeAll(&item.first, sizeof(SHA1FP));
            fd.writeAll(&item.second, sizeof(ENTRY_VALUE));
            count++;
        }
        printf("Total items: %d\n", count);

    } catch (const std::exception& e) {
        fprintf(stderr, "Error saving metadata: %s\n", e.what());
        exit(EXIT_FAILURE);
    }
}


LookupResult MetadataManager::dedupLookup(SHA1FP sha1){
    auto dedupIter = this->fp_table_origin.find(sha1);
    if(dedupIter != this->fp_table_origin.end()){
        return Dedup;
    }

    dedupIter = this->fp_table_added.find(sha1);
    if(dedupIter != this->fp_table_added.end()){
        return Dedup;
    }

    return Unique;
}

int MetadataManager::addNewEntry(SHA1FP sha1, ENTRY_VALUE value){
    this->fp_table_added.emplace(sha1, value);
    return 0;
}

ENTRY_VALUE MetadataManager::getEntry(const SHA1FP sha1){
    return this->fp_table_origin[sha1];
}

ENTRY_VALUE MetadataManager::getAddedEntry(const SHA1FP sha1){
    return this->fp_table_added[sha1];
}