#!/bin/bash
cd _install
mkdir etc dev mnt && mkdir -p proc sys tmp mnt && mkdir -p etc/init.d/
mknod dev/console c 5 1 && mknod dev/null c 1 3 && mknod dev/tty1 c 4 1
touch etc/fstab etc/init.d/rcS etc/inittab && chmod 755 etc -R

cat > etc/fstab << "EOF"
proc        /proc           proc         defaults        0        0
tmpfs       /tmp            tmpfs        defaults        0        0
sysfs       /sys            sysfs        defaults        0        0
EOF

cat etc/init.d/rcS << "EOF"
echo -e "Welcome to tinyLinux"
/bin/mount -a
echo -e "Remounting the root filesystem"
mount  -o  remount,rw  /
mkdir -p /dev/pts
mount -t devpts devpts /dev/pts
echo /sbin/mdev > /proc/sys/kernel/hotplug
mdev -s
EOF

cat etc/inittab << "EOF"
::sysinit:/etc/init.d/rcS
::respawn:-/bin/sh
::askfirst:-/bin/sh
::cttlaltdel:/bin/umount -a -r
chmod 755 etc/inittab
cd dev
mknod console c 5 1
mknod null c 1 3
mknod tty1 c 4 1 
EOF
cd ..


rm -rf rootfs.ext3 fs
dd if=/dev/zero of=./rootfs.ext3 bs=1M count=32
mkfs.ext3 rootfs.ext3
mkdir fs
mount -o loop rootfs.ext3 ./fs
cp -rf ./_install/* ./fs
umount ./fs
gzip --best -c rootfs.ext3 > rootfs.img.gz 
