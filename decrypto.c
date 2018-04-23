#include <linux/init.h>
#include <linux/module.h>
#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("Shwetank Shrey & Rohan Chhokra");

static dev_t dev_no;
static char *name = "Decryption-Device";
static struct cdev c_dev;
static struct class *cl;

static int de_open (struct inode *in, struct file *fl) {
    printk("Opening Decryption Device...\n");
    return 0;
}

static ssize_t de_read (struct file *fl, char __user *usr, size_t sz, loff_t *off) {
    printk("Reading from Decryption Device...\n");
    return 0;
}

static ssize_t de_write (struct file *fl, const char __user *usr, size_t sz, loff_t *off) {
    printk("Writing to Decryption Device...\n");
    return sz;
}

static int de_close (struct inode *in, struct file *fl) {
    printk("Closing Decryption Device...\n");
    return 0;
}

static struct file_operations driver_fops = {
    .owner = THIS_MODULE,
    .open = de_open,
    .read = de_read,
    .write = de_write,
    .release = de_close,
};

static int register_device(void) {
    printk("Registering Decryption Device...\n");
    if(alloc_chrdev_region(&dev_no ,0, 1, name) < 0) {
        return -1;
    }
    if((cl = class_create(THIS_MODULE, "encrypto")) == NULL) {
        unregister_chrdev_region(dev_no, 1);
        printk("Decryption Device not registered.\n");
        return -1;
    }
    if(device_create(cl, NULL, dev_no, NULL, "decrypto") == NULL) {
        class_destroy(cl);
        unregister_chrdev_region(dev_no, 1);
        printk("Decryption Device not registered.\n");
        return -1;
    }
    cdev_init(&c_dev, &driver_fops);
    if(cdev_add(&c_dev, dev_no, 1) == -1) {
        device_destroy(cl, dev_no);
        class_destroy(cl);
        unregister_chrdev_region(dev_no, 1);
        printk("Decryption Device not registered.\n");
        return -1;
    }
    printk("Decrption Device registered. Major number : %i.\n", MAJOR(dev_no));
    printk("Decryption Device available at /dev/decrypto.\n");
    return 0;
}

static void unregister_device(void) {
    cdev_del(&c_dev);
    device_destroy(cl, dev_no);
    class_destroy(cl);
    unregister_chrdev_region(dev_no, 1);
    printk("Decryption Device unregistered. \n");
}

module_init(register_device);
module_exit(unregister_device);