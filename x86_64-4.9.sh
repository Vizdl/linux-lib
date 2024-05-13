#!/bin/sh

TARGET_ARCH=x86_64
LINUX_VERSION=linux-4.9.229

source common.sh

#  gdb 相关
func["gdb-start"]="启动 gdb 服务"
func["gdb-restart"]="重启 gdb 服务"
func["gdb-stop"]="停止 gdb 服务"
func["debug"]="连接 gdb 服务"

#
# 调用主函数
#
main $*