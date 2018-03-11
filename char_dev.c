#include <linux/module.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/slab.h>

#define MY_DEV_MAJOR  240

struct my_char_dev{
	struct cdev dev;
	int value;
};
int char_dev_major = MY_DEV_MAJOR;
struct my_char_dev *char_devp;

MODULE_AUTHOR("woriaty");
MODULE_LICENSE("GPL v2");

static ssize_t char_dev_read(struct file *filp,const char __user *buf,
	size_t count,loff_t *f_pos)
{
	struct my_char_dev *dev = filp->private_data;
	if(copy_to_user(buf,&(dev->value),1))
		return -EFAULT;
	return 1;
}

static ssize_t char_dev_write(struct file *filp,const char __user *buf,
	size_t count,loff_t *f_pos)
{
	struct my_char_dev *dev = filp->private_data;
	if(copy_from_user(&(dev->value),buf,1))
		return -EFAULT;
	printk(KERN_INFO"write data = 0x%x\n",dev->value);
	return 1;
}

static int char_dev_open(struct inode *inode, struct file *filp)
{
	filp->private_data = char_devp;
	printk(KERN_INFO"Open char dev\n");
	return 0;
}

static int char_dev_release(struct inode *inode, struct file *filp)
{
	printk(KERN_INFO"Realse char dev\n");
	return 0;
}

struct file_operations char_dev_ops = {
	.owner   = THIS_MODULE,
	.read    = char_dev_read,
	.write   = char_dev_write,
	.open    = char_dev_open,
	.release = char_dev_release,
};

static void char_dev_setup(struct my_char_dev *dev, int index)
{
	int err,devno = MKDEV(char_dev_major,index);
	cdev_init(&dev->dev,&char_dev_ops);
	dev->dev.owner = THIS_MODULE;
	err = cdev_add(&dev->dev,devno,1);
	if(err)
		printk(KERN_INFO"ERROR %d add char_dev %d",err,devno);
}

static int __init char_dev_init(void)
{
	int err;
	dev_t devno = MKDEV(char_dev_major,0);
	if(char_dev_major)
		err = register_chrdev_region(devno,1,"my char dev");
	else{
		err = alloc_chrdev_region(&devno,0,1,"my char dev");
		char_dev_major = MAJOR(devno);
	}
	if(err < 0)
		return err;
	char_devp = kmalloc(sizeof(struct my_char_dev),0);
	if(!char_devp){
		printk(KERN_INFO"No memory for dev!\n");
		goto fail_malloc;
	}
	memset(char_devp,0,sizeof(struct my_char_dev));
	char_dev_setup(char_devp,0);
	printk(KERN_INFO"char dev driver init!\n");
	return 0;
fail_malloc:
	unregister_chrdev_region(devno,1);
	return -ENOMEM;
}
module_init(char_dev_init);

static void __exit char_dev_exit(void)
{
	cdev_del(&char_devp->dev);
	kfree(char_devp);
	unregister_chrdev_region(MKDEV(char_dev_major,0),1);
	printk(KERN_INFO"char dev driver exit!\n");
}
module_exit(char_dev_exit);
