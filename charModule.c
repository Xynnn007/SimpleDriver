#include <linux/init.h>
#include <linux/kernel.h>
//module_init
#include <linux/module.h>
//module_license
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <asm/uaccess.h>
#define BASEMINOR 	0
#define COUNT		3
#define NAME		"char_dev_xynnn"


static unsigned char inc = 0;
static unsigned char buffer[256];

dev_t devno;
struct cdev *cdevp;

//open
int open(struct inode *inode, struct file *file){
	printk(KERN_INFO "Open function called.\n");
	if(inc>0) return -ERESTARTSYS;
	inc++;
	return 0;
}

//close
int release(struct inode *inode, struct file *filp){
	inc--;
	printk(KERN_INFO "release function called\n");
	return 0;
}

//read
ssize_t read(struct file * filp, char __user* buf, size_t count , loff_t *f_pos){
	if(copy_to_user(buf, buffer, count)){
		count = -EFAULT;
	}
	return count;
}

//write
ssize_t write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos){
	if(copy_from_user(buffer + *f_pos, buf, count)){
		count = -EFAULT;
	}
	return count;
}


struct file_operations fops = {
	.owner	= 	THIS_MODULE,
	.read 	=	read,
	.write	=	write,
	.open	=	open,
	.release=	release,
};


static int __init driver_init(void){
	//0.alloc dev_t 设备号
	int ret;
	ret = alloc_chrdev_region(&devno, BASEMINOR , COUNT, NAME);
	if(ret < 0 ) {
		printk(KERN_ERR "alloc_chrdev_region failed...\n");
		goto err1;
	}
	printk(KERN_INFO "major = %d, minor = %d.\n",MAJOR(devno),MINOR(devno));
	//1. alloc cdef
	cdevp = cdev_alloc();
	if(!cdevp){
		printk(KERN_ERR "cdevp_alloc failed...\n");
		ret = -ENOMEM;
		goto err2;
	}
	//2.cdev init
	cdev_init(cdevp, &fops);

	//3.cdev_add
	ret = cdev_add(cdevp, devno, COUNT);
	if(ret < 0){
		printk(KERN_ERR "cdev_add failed...\n");
		goto err3;
	}
	
	printk(KERN_INFO "Successfully install device!\n");
	return 0;
err3:
	kzfree(cdevp);
err2:
	unregister_chrdev_region(devno, COUNT);
err1:
	return ret;
}

static void __exit driver_exit(void){
	cdev_del(cdevp);
	unregister_chrdev_region(devno,COUNT);
	printk(KERN_INFO "Successfully uninstall device!\n");
}


module_init(driver_init);
module_exit(driver_exit);
MODULE_LICENSE("GPL");
