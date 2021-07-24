#!/bin/sh
# in src
if [ ! -d "read-config" ];then
    mkdir read-config
fi
# in linux
cd linux

cp ../scripts/lkc2sicx.py .
cp ../scripts/gen_sourceinsight_list.sh .
dos2unix lkc2sicx.py  gen_sourceinsight_list.sh

# 配置宏配置文件
make defconfig
./lkc2sicx.py -s .config -d ../read-config/default

# 生成脚本
make -j32 > build.log
./gen_sourceinsight_list.sh build.log ../read-config/file_list.txt

# 删除中间生成文件
rm -f build.log

# 删除脚本
rm -f lkc2sicx.py gen_sourceinsight_list.sh

# 清理编译文件
# make distclean
