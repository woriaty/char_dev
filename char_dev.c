#include <linux/module.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/slab.h>



static int __init char_dev_init(void)
{
	printk(KERN_INFO"char dev driver init!\n");
	return 0;
}
module_init(char_dev_init);

static int __exit char_dev_exit(void)
{
	printk(KERN_INFO"char dev driver exit!\n");
	return 0;
}
module_exit(char_dev_init);
