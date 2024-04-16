#!/bin/bash
dest=usbdisk.img
mount_dir="/mnt"
## 1. 删除磁盘
rm -rf ${dest}
## 2. 创建多分区磁盘
### 2.1 创建磁盘
dd if=/dev/zero of=${dest} bs=512 count=102400
### 2.2 格式化磁盘
# 执行 expect 脚本,设置分区表创建分区。
expect <<-EOF
set timeout -1
spawn gdisk ${dest}
expect {
    "Command (? for help)" {send "n\n";exp_continue}
    "Partition number (1-128, default 1):" {send "\n";exp_continue}
    "First sector" {send "\n";exp_continue}
    "Last sector" {send "\n";exp_continue}
    "Hex code or GUID" {send "\n"}
}
expect "Command (? for help)" {send "w\n"}
expect "Do you want to proceed" {send "y\n";exp_continue}
EOF

### 2.3 展示磁盘分区
gdisk -l  $dest

### 2.4 添加磁盘到设备
parent_dev=`losetup --show --find usbdisk.img`
kpartx -av ${parent_dev}
subdev=$(echo "$parent_dev" | sed 's|/dev/loop|/dev/mapper/loop|')p1
### 2.5 格式化
mkfs.ext3 ${subdev}
### 2.6 挂载
mount -t ext3 ${subdev} ${mount_dir}
### 2.7 添加一个文件
touch ${mount_dir}/a.txt
chmod 777 ${mount_dir}/a.txt
echo "Hello USB!!!" >> ${mount_dir}/a.txt
### 2.8 同步数据
sync
### 2.9 取消挂载
umount ${mount_dir}
# ### 2.10 取消磁盘与设备的关联
kpartx -dv ${subdev}
losetup -d ${parent_dev}

