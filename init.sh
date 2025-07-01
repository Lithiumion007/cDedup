#!/bin/bash
# 这里的配置需要和参数json一致
rm -fr /home/cyf/ssd1/working2/

mkdir /home/cyf/ssd1/working2/
mkdir /home/cyf/ssd1/working2/restoreFolder
mkdir /home/cyf/ssd1/working2/Containers
mkdir /home/cyf/ssd1/working2/FULL_FILE_STORAGE
mkdir /home/cyf/ssd1/working2/metadata
mkdir /home/cyf/ssd1/working2/metadata/FileRecipes

touch /home/cyf/ssd1/working2/metadata/fingerprints.meta
touch /home/cyf/ssd1/working2/metadata/FFFP.meta
touch /home/cyf/ssd1/working2/metadata/L1.meta
touch /home/cyf/ssd1/working2/metadata/L2.meta
touch /home/cyf/ssd1/working2/metadata/L3.meta
touch /home/cyf/ssd1/working2/metadata/L4.meta
touch /home/cyf/ssd1/working2/metadata/L5.meta
touch /home/cyf/ssd1/working2/metadata/L6.meta

source ./scripts/clear_global_stat.sh

