#ifndef _LINUX_CDEV_H
#define _LINUX_CDEV_H

#include <linux/kobject.h>
#include <linux/kdev_t.h>
#include <linux/list.h>

struct file_operations;
struct inode;
struct module;

struct cdev {
	/**
	 * 内嵌的内核对象.
 	 */
	struct kobject kobj;
	/**
	 * 所属模块
	 */
	struct module *owner;
	/**
	 * 字符设备操作符
	 */
	const struct file_operations *ops;
	/**
	 * 引用此设备的文件结点链表的头
	 */
	struct list_head list;
	/**
	 * 第一个字符设备号 
	 */
	dev_t dev;
	/**
	 * 设备个数 
	 */
	unsigned int count;
};

void cdev_init(struct cdev *, const struct file_operations *);

struct cdev *cdev_alloc(void);

void cdev_put(struct cdev *p);

int cdev_add(struct cdev *, dev_t, unsigned);

void cdev_del(struct cdev *);

int cdev_index(struct inode *inode);

void cd_forget(struct inode *);

extern struct backing_dev_info directly_mappable_cdev_bdi;

#endif
