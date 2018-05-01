#include <linux/init.h>
#include <linux/module.h>
#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/slab.h>
#include <linux/random.h>
#include <linux/cdev.h>
#include <asm/uaccess.h>

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("Shwetank Shrey & Rohan Chhokra");

static dev_t dev_no;
static char *name = "Encryption-Device";
static struct cdev c_dev;
static struct class *cl;

static unsigned char *encrypted_text;

static int en_open (struct inode *in, struct file *fl) {
    printk("Opening Encryption Device...\n");
    encrypted_text = kmalloc(sizeof(char)*256, GFP_KERNEL);
    memset(encrypted_text, 0, sizeof(char)*256);
    return 0;
}

static ssize_t en_read (struct file *fl, char __user *usr, size_t sz, loff_t *off) {
    printk("Reading from Encryption Device...\n");
    if (copy_to_user( usr, (char *) encrypted_text, sz ) != 0) {
        return -EFAULT;
    }
    return sz;
}

static ssize_t en_write (struct file *fl, const char __user *usr, size_t sz, loff_t *off) {
    unsigned char *to_encrypt;
    unsigned char *key;
    int i;
    printk("Writing to Encryption Device...\n");
    to_encrypt = kmalloc(sizeof(char)*sz, GFP_KERNEL);
    memset(to_encrypt, 0, sizeof(char)*sz);
    key = kmalloc(sizeof(char)*16, GFP_KERNEL);
    if (copy_from_user(to_encrypt, usr, sz) != 0) {
        return -EFAULT;
    }
    get_random_bytes(key, sizeof(char)*16);
    for(i = 0 ; i < 16 ; i++) {
        encrypted_text[i] = key[i];
    }
    i = 0;
    while (i < sz - 1) {
        int j = 0;
        while( j < 16 ) {
            if( i >= sz-1 ) {
                encrypted_text[i+16] = key[j] ^ 0;
            }
            else {
                encrypted_text[i+16] = key[j] ^ to_encrypt[i];
            }
            key[j] = encrypted_text[i+16];
            j++;
            i++;
        }
    }
    while(i < 240) {
        encrypted_text[i+16] = 0x00;
        i++;
    }
    kfree(to_encrypt);
    kfree(key);
    return sz;
}

static int en_close (struct inode *in, struct file *fl) {
    printk("Closing Encryption Device...\n");
    kfree(encrypted_text);
    return 0;
}

static struct file_operations driver_fops = {
    .owner = THIS_MODULE,
    .open = en_open,
    .read = en_read,
    .write = en_write,
    .release = en_close,
};

static int register_device(void) {
    printk("Registering Encryption Device...\n");
    if(alloc_chrdev_region(&dev_no ,0, 1, name) < 0) {
        return -1;
    }
    if((cl = class_create(THIS_MODULE, "encrypto")) == NULL) {
        unregister_chrdev_region(dev_no, 1);
        printk("Encryption Device not registered.\n");
        return -1;
    }
    if(device_create(cl, NULL, dev_no, NULL, "encrypto") == NULL) {
        class_destroy(cl);
        unregister_chrdev_region(dev_no, 1);
        printk("Encryption Device not registered.\n");
        return -1;
    }
    cdev_init(&c_dev, &driver_fops);
    if(cdev_add(&c_dev, dev_no, 1) == -1) {
        device_destroy(cl, dev_no);
        class_destroy(cl);
        unregister_chrdev_region(dev_no, 1);
        printk("Encryption Device not registered.\n");
        return -1;
    }
    printk("Encryption Device registered. Major number : %i.\n", MAJOR(dev_no));
    printk("Encryption Device available at /dev/encrypto.\n");
    return 0;
}

static void unregister_device(void) {
    cdev_del(&c_dev);
    device_destroy(cl, dev_no);
    class_destroy(cl);
    unregister_chrdev_region(dev_no, 1);
    printk("Encryption Device unregistered.\n");
}

module_init(register_device);
module_exit(unregister_device);
