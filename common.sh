#!/bin/sh

# 功能数组
declare -A func
#  docker 镜像相关
func["build-image"]="构建 docker 镜像"
func["run-image"]="运行 docker 镜像"
func["clean-image"]="删除 docker 镜像"
#  linux 编译
func["defconfig"]="构建 linux 默认配置文件"
func["menuconfig"]="以图形化的方式构建 linux 配置文件"
func["image"]="构建 linux 镜像"
#  busybox 编译
func["fs-defconfig"]="构建 busybox 默认配置文件"
func["rootfs"]="构建 busybox rootfs"
# 磁盘生成
func["usbdisk"]="构建 usb 磁盘"
# 运行 linux
func["run"]="在容器内运行 linux 镜像"
func["flush"]="更新 linux 镜像并在容器运行,常用于修改 linux 内核代码后"
func["drun"]="在主机运行 linux 镜像"
func["dflush"]="更新 linux 镜像并在主机运行,常用于修改 linux 内核代码后"

# 清除所有生成结果
func["cleanall"]="清除所有生成结果"

function usage()
{
	echo "功能名作为脚本的第一个参数传入，例如："
	echo "	bash ${0} image"
	echo "以下是该脚本支持的全部功能："
	# 遍历关联数组中的所有键值对
	for key in "${!func[@]}"; do
		echo "	$key : ${func[$key]}"
	done
}

function call_sub_cmd()
{
    sudo TARGET_ARCH=${TARGET_ARCH} LINUX_VERSION=${LINUX_VERSION} make $1
}

function main()
{
	#
	# 带参数运行，看看相应的函数是否存在
	#
	SUB_CMD=$1

	if [ -v func["${SUB_CMD}"] ]; then
		# echo "Value for key2: ${func["${SUB_CMD}"]}"
		call_sub_cmd $SUB_CMD
		exit
	fi
	usage
}

