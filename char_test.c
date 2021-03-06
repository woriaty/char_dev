#include <linux/module.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/io.h>

#define CHAR_DEV_MAJOR  234
#define CHAR_DEV_NAME "my char dev"


MODULE_AUTHOR("Jianjun Wang");
MODULE_LICENSE("GPL v2");

#define PIO_BASE   0x01f02c00
#define PL_CFG1    (PIO_BASE + 0x04)
#define PL_DATA    (PIO_BASE + 0x10)

enum IO_STATE{
	INPUT,
	OUTPUT,
	S_PWM,
	S_PL_EINT = 6,
	IO_DISABLE,
};
enum LIGHT_STATE{LIGHT_OFF,LIGHT_ON};

typedef struct {
	volatile unsigned int PL8_SELECT  : 3;
	volatile unsigned int res0        : 1;
	volatile unsigned int PL9_SELECT  : 3;
	volatile unsigned int res1        : 1;
	volatile unsigned int PL10_SELECT : 3;
	volatile unsigned int res2        : 1;
	volatile unsigned int PL11_SELECT : 3;
	volatile unsigned int res3        : 17;
}pl_cfg1; 

typedef struct {
	volatile unsigned int GPIO_0  : 1;
	volatile unsigned int GPIO_1  : 1;
	volatile unsigned int GPIO_2  : 1;
	volatile unsigned int GPIO_3  : 1;
	volatile unsigned int GPIO_4  : 1;
	volatile unsigned int GPIO_5  : 1;
	volatile unsigned int GPIO_6  : 1;
	volatile unsigned int GPIO_7  : 1;
	volatile unsigned int GPIO_8  : 1;
	volatile unsigned int GPIO_9  : 1;
	volatile unsigned int GPIO_10 : 1;
	volatile unsigned int GPIO_11 : 1;
	volatile unsigned int res     : 20;
}pl_data;

pl_cfg1 *GPIOL_GREEN_CFG;
pl_data *GPIOL_GREEN_DATA;
//pl_cfg1 *GPIOL_GREEN_CFG = (pl_cfg1 *)ioremap(PL_CFG1,4)
//pl_data *GPIOL_GREEN_DATA = (pl_data *)ioremap(PL_DATA,4)

void light_init(void)
{
	GPIOL_GREEN_CFG = (pl_cfg1 *)ioremap(PL_CFG1,4);
	GPIOL_GREEN_DATA = (pl_data *)ioremap(PL_DATA,4);
	GPIOL_GREEN_CFG->PL10_SELECT = OUTPUT;
	GPIOL_GREEN_DATA->GPIO_10 = LIGHT_OFF;
}

void light_on(void)
{
	printk("%s\n",__func__);
	GPIOL_GREEN_DATA->GPIO_10 = LIGHT_ON;
}

void light_off(void)
{
	printk("%s\n",__func__);
	GPIOL_GREEN_DATA->GPIO_10 = LIGHT_OFF;
}

ssize_t char_test_read(struct file *filp, char __user *buf, size_t count,
	loff_t *f_pos)
{
	printk("%s\n",__func__);
	printk("GPIOL_GREEN_CFG = 0x%x\n",*(unsigned int *)GPIOL_GREEN_CFG);
	printk("GPIOL_GREEN_DATA = 0x%x\n",*(unsigned int *)GPIOL_GREEN_DATA);
	return 0;
}
ssize_t char_test_write(struct file *filp, const char __user *buf, size_t count,
	loff_t *f_pos)
{
	char argv;
	printk("%s\n",__func__);
	if(copy_from_user(&argv,buf,count))
		printk(KERN_INFO"Copy from user data ERROR!\n");
	if(argv == '1')
		light_on();
	else light_off();
	return count;
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
	light_init();
	ret = register_chrdev(CHAR_DEV_MAJOR,CHAR_DEV_NAME,&char_test_ops);
	if(ret){
		printk("Can't register char device %d\n",CHAR_DEV_MAJOR);
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

