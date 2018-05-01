#include <linux/init.h>
#include <linux/module.h>
#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/random.h>
#include <linux/slab.h>
#include <asm/uaccess.h>

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("Shwetank Shrey & Rohan Chhokra");

static dev_t dev_no;
static char *name = "Decryption-Device";
static struct cdev c_dev;
static struct class *cl;

static unsigned char *decrypted_text;

static int de_open (struct inode *in, struct file *fl) {
    printk("Opening Decryption Device...\n");
    decrypted_text = kmalloc(sizeof(char)*256, GFP_KERNEL);
    memset(decrypted_text, 0, sizeof(char)*256);
    return 0;
}

static ssize_t de_read (struct file *fl, char __user *usr, size_t sz, loff_t *off) {
    int i;
    printk("Reading from Decryption Device...\n");
    for(i = 0 ; i < 256 ; i++) {
        // printk("%02x", decrypted_text[i]);
        if(decrypted_text[i] == 0x00) {
            break;
        }
    }
    // printk("\n%s\n", decrypted_text);
    if (copy_to_user( usr, (char *) decrypted_text, sz ) != 0) {
        return -EFAULT;
    }
    return sz;
}

static ssize_t de_write (struct file *fl, const char __user *usr, size_t sz, loff_t *off) {
    unsigned char *to_decrypt;
    unsigned char *key;
    char *tmp;
    int i;
    printk("Writing to Decryption Device...\n");
    to_decrypt = kmalloc(sizeof(char)*256, GFP_KERNEL);
    tmp = kmalloc(sizeof(char)*256, GFP_KERNEL);
    memset(to_decrypt, 0, sizeof(char)*256);
    key = kmalloc(sizeof(char)*16, GFP_KERNEL);
    if (copy_from_user(tmp, usr, sz) != 0) {
        return -EFAULT;
    }
    for(i = 0 ; i < 256 ; i++) {
        to_decrypt[i] = (unsigned char) tmp[i];
    }
    for(i = 0 ; i < 16 ; i++) {
        key[i] = to_decrypt[i];
    }
    i = 0;
    while (i < 16) {
        int j = 0;
        while( j < 16 ) {
            decrypted_text[i*16+j] = key[j] ^ to_decrypt[(i+1)*16+j];
            key[j] = to_decrypt[(i+1)*16+j];
            j++;
        }
        i++;
    }
    kfree(to_decrypt);
    kfree(key);
    return sz;
}

static int de_close (struct inode *in, struct file *fl) {
    printk("Closing Decryption Device...\n");
    kfree(decrypted_text);
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
    if((cl = class_create(THIS_MODULE, "decrypto")) == NULL) {
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
    printk("Decryption Device registered. Major number : %i.\n", MAJOR(dev_no));
    printk("Decryption Device available at /dev/decrypto.\n");
    return 0;
}

static void unregister_device(void) {
    cdev_del(&c_dev);
    device_destroy(cl, dev_no);
    class_destroy(cl);
    unregister_chrdev_region(dev_no, 1);
    printk("Decryption Device unregistered.\n");
}

module_init(register_device);
module_exit(unregister_device);