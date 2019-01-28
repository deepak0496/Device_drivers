#include <linux/types.h>
#include<linux/kdev_t.h>
#include<linux/fs.h>
#include<linux/init.h>
#include<linux/module.h>
#include<linux/moduleparam.h>
#include <linux/kernel.h>
#include<linux/device.h>
#include<linux/cdev.h>

static dev_t first;
static struct cdev c_dev;
static struct class *cls;
//Step -4 driver callback functions
static int my_open(struct inode *i,struct file *f)
{
 printk(KERN_INFO"mychar: open()\n");
 return 0;
}
static int my_close(struct inode *i,struct file *f)
{
 printk(KERN_INFO"mychar: close()\n");
 return 0;
}
static ssize_t my_read(struct file *f, char __user *buf,size_t len, loff_t* off)
{
 printk(KERN_INFO"mychar : read()\n");
 return 0;
}
static ssize_t my_write(struct file *f,const char __user *buf,size_t len, loff_t *off)
{
 printk(KERN_INFO"mychar : write()\n");
 return len; 
}
static struct file_operations fops ={  .owner =THIS_MODULE,
				       .open = my_open,
				       .write = my_write,
				       .release = my_close,
				       .read = my_read
				       };

static int __init mychar_init(void)
{
 printk(KERN_INFO "mychar registered\n");
 if(alloc_chrdev_region(&first, 0,1,"BITS-PILANI") < 0)
 {
   return -1;
 }
 printk(KERN_INFO "<Major,Minor>:<%d, %d>\n",MAJOR(first),MINOR(first));
 // Step 2 creation of device file
if((cls = class_create(THIS_MODULE,"chardev"))==NULL)
{
 unregister_chrdev_region(first,1);
 return -1;
}
if(device_create(cls,NULL,first,NULL,"mydevice")==NULL)
{
 class_destroy(cls);
 unregister_chrdev_region(first,1);
 return -1;
}
//STEP 3 Link fops and cdev to device node
cdev_init(&c_dev,&fops); //cdev structure and initialize the file_operations field in the structure
if(cdev_add(&c_dev,first,1)==-1)
{
 device_destroy(cls,first);
 class_destroy(cls);
 unregister_chrdev_region(first,1);
}
 return 0;
}

static void __exit mychar_exit(void)
{
 cdev_del(&c_dev);
 device_destroy(cls,first);
 class_destroy(cls);
 unregister_chrdev_region(first,1);
 printk(KERN_INFO "mychar unregistered\n");
}

module_init(mychar_init);
module_exit(mychar_exit);

MODULE_DESCRIPTION("My first code");
MODULE_AUTHOR("Perumalla Deepak");
MODULE_LICENSE("GPL");
MODULE_INFO(supportedchips,"chips");
