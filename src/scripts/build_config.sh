#!/bin/sh
LINUX_DIR="linux"
SCRIPTS_DIR="scripts"
BUILD_LOG_FILE="build.log"
LOW_COM="/usr/bin/gcc-4.8"
if [ $# != 1 ] ; then
    echo "USAGE: $0 Linux-Version"
    echo " e.g.: $0 high or low"
    exit 1;
fi

VERSION=$1

if [ $VERSION != "high" -a $VERSION != "low" ]; then
    echo "无效参数, e.g.: $0 high or low, but : $0 ${VERSION}"
    exit 1;
fi

gen_read_config (){
    if [ $# != 1 ] ; then
        echo "$0 gen_read_config: 缺少参数"
        echo " e.g.: $0 read-config-dir"
        exit 1;
    fi
    # 检查是否存在目录
    if [ ! -d ${LINUX_DIR} ]; then 
        echo "路径下缺少 ${LINUX_DIR} 目录 : ${PWD}"
        exit 1;
    fi
    read_config=$1
    # in src
    if [ ! -d ${read_config} ];then
        mkdir ${read_config}
    fi
    # in linux
    cd ${LINUX_DIR}

    cp ../${SCRIPTS_DIR}/lkc2sicx.py .
    cp ../${SCRIPTS_DIR}/gen_sourceinsight_list.sh .
    dos2unix lkc2sicx.py  gen_sourceinsight_list.sh

    # 配置宏配置文件
    make defconfig
    ./lkc2sicx.py -s .config -d ../${read_config}/default

    # 生成脚本
    make -j32 > ${BUILD_LOG_FILE}
    ./gen_sourceinsight_list.sh ${BUILD_LOG_FILE} ../${read_config}/file_list.txt
}

clean (){
    # 删除中间生成文件
    rm -f ${BUILD_LOG_FILE}
    # 删除脚本
    rm -f lkc2sicx.py gen_sourceinsight_list.sh
    # 清理编译文件
    # make distclean
}

if [ $VERSION != "low" ]; then
    if [ -f ${LOW_COM} ]; then
        ln -sf ${LOW_COM} /usr/bin/gcc
    else
        echo "无法找到 : ${LOW_COM}"
    fi
fi
gen_read_config "read-config"
clean