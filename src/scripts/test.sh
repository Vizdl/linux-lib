#!/bin/bash
# author:菜鸟教程
# url:www.runoob.com


HIGH="high"
LOW="low"
VERSION="high"
if [ $VERSION != "high" -a $VERSION != "low" ]; then
    echo "无效参数, e.g.: $0 high or low, but : $0 ${VERSION}"
    exit 1;
fi