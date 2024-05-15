#include "MetadataManager.h"
#include "assert.h" 
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

MetadataManager *GlobalMetadataManagerPtr;

int MetadataManager::load(){
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

int MetadataManager::save(){
    printf("-----------------------Saving FP-index-----------------------\n");
    int fd = open(this->metadata_file_path.c_str(), O_WRONLY | O_CREAT, 0777);
    if(fd < 0){
        perror("Saving fp index error, the reason is ");
        exit(-1);
    }
    lseek(fd, 0, SEEK_SET);
    ftruncate(fd,0);

    int count = 0;
    
    for(auto item : this->fp_table_added){
        write(fd, (uint8_t*)&item.first, sizeof(SHA1FP));
        write(fd, (uint8_t*)&item.second, sizeof(ENTRY_VALUE));
        count++;
    }
    printf("New added item %d\n", count);

    for(auto item : this->fp_table_origin){
        write(fd, (uint8_t*)&item.first, sizeof(SHA1FP));
        write(fd, (uint8_t*)&item.second, sizeof(ENTRY_VALUE));
        count++;
    }
    printf("total item %d\n", count);

    close(fd);
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