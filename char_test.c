#include <linux/module.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/slab.h>

#define CHAR_DEV_MAJOR  234
#define CHAR_DEV_NAME "my char dev"


MODULE_AUTHOR("Jianjun Wang");
MODULE_LICENSE("GPL v2");

ssize_t char_test_read(struct file *filp, char __user *buf, size_t count,
	loff_t *f_pos)
{
	printk("%s\n",__func__);
	return count;
}
ssize_t char_test_write(struct file *filp, char __user *buf, size_t count,
	loff_t *f_pos)
{
	printk("%s\n",__func__);
	return 0;
}

long char_test_ioctl(struct file *filp, unsigned int cmd, 
	unsigned long count)
{
	return count;
}

int char_test_open(struct inode *inode, struct file *filp)
{
	printk("%s\n",__func__);
	return 0;
}

int char_test_release(struct inode *inode, struct file *filp)
{
	printk("%s\n",__func__);
	return 0;
}



struct file_operations char_test_ops = {
	.owner = THIS_MODULE,
	.read  = char_test_read,
	.write = char_test_write,
	.unlocked_ioctl = char_test_ioctl,
	.open  = char_test_open,
	.release = char_test_release,
};

static int __init char_test_init(void)
{
	int ret;
	printk("%s\n",__func__);
	ret = register_chrdev(CHAR_DEV_MAJOR,CHAR_DEV_NAME,&char_test_ops);
	if(ret){
		printk("Can't register char device %d\n",char_test_major);
		return ret;
	}
	return 0;
}
module_init(char_test_init);

static void __exit char_test_exit(void)
{
	unregister_chrdev(CHAR_DEV_MAJOR,CHAR_DEV_NAME);
	printk("%s\n",__func__);
}
module_exit(char_test_exit);

