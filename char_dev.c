#include <linux/module.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/slab.h>


static ssize_t char_dev_read(struct file *,const char __user *,size_t,loff_t *)
{
	return 0;
}

static ssize_t char_dev_write(struct file *,const char __user *,size_t,loff_t *)
{
	return 0;
}

static int char_dev_open(struct inode *, struct file *)
{
	return 0;
}

static int char_dev_release(struct inode *, struct file *)
{
	return 0;
}

struct file_operations char_dev_ops{
	.read    = char_dev_read;
	.write   = char_dev_write;
	.open    = char_dev_open;
	.release = char_dev_release;
};

static int __init char_dev_init(void)
{
	printk(KERN_INFO"char dev driver init!\n");
	return 0;
}
module_init(char_dev_init);

static void __exit char_dev_exit(void)
{
	printk(KERN_INFO"char dev driver exit!\n");
}
module_exit(char_dev_exit);
