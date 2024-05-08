#!/bin/bash
# 这里的配置需要和参数json一致
rm -fr /home/cyf/working/

mkdir /home/cyf/working/
mkdir /home/cyf/working/restoreFolder
mkdir /home/cyf/working/Containers
mkdir /home/cyf/working/FULL_FILE_STORAGE
mkdir /home/cyf/working/metadata
mkdir /home/cyf/working/metadata/FileRecipes

touch /home/cyf/working/metadata/fingerprints.meta
touch /home/cyf/working/metadata/FFFP.meta
touch /home/cyf/working/metadata/L1.meta
touch /home/cyf/working/metadata/L2.meta
touch /home/cyf/working/metadata/L3.meta
touch /home/cyf/working/metadata/L4.meta
touch /home/cyf/working/metadata/L5.meta
touch /home/cyf/working/metadata/L6.meta

source ./scripts/clear_global_stat.sh

