#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/kdev_t.h>
#include <linux/uaccess.h>
#include <linux/platform_device.h>

#include "platform.h"


#undef  pr_fmt
#define pr_fmt(fmt) "%s: " fmt, __func__

#define NO_OF_DEVICES 4

#define MEM_SIZE_MAX_PCDEV1 1024
#define MEM_SIZE_MAX_PCDEV2 1024
#define MEM_SIZE_MAX_PCDEV3 1024
#define MEM_SIZE_MAX_PCDEV4 1024

#define RDONLY 0x01
#define WRONLY 0x10
#define RDWR   0x11

/* pseudo device's memory */
char device_buffer_pcdev1[MEM_SIZE_MAX_PCDEV1];
char device_buffer_pcdev2[MEM_SIZE_MAX_PCDEV2];
char device_buffer_pcdev3[MEM_SIZE_MAX_PCDEV3];
char device_buffer_pcdev4[MEM_SIZE_MAX_PCDEV4];


/*Device private data structure */
struct pcdev_private_data {

    struct pcdev_platform_data pdata;
    char *buffer;
    dev_t dev_num;
    struct cdev cdev;

}

/*Driver private data structure */
struct pcdrv_private_data {
    
    int total_devices;

    /*This holds the device number*/
    dev_t device_number_base;

    struct class *class_pcd;
    struct device *device_pcd;

};

struct pcdrv_private_data pcdrv_data = {
    
};

loff_t pcd_lseek(struct file *filp, loff_t offset, int whence) {
    
    return 0;
}

ssize_t pcd_read(struct file *filp, char __user *buff, size_t count, loff_t *f_pos) {

    return 0;
}

ssize_t pcd_write(struct file *filp, const char __user *buff, size_t count, loff_t *f_pos) {
    
    return -ENOMEM;
}


int pcd_open(struct inode *inode, struct file *filp) {
    
    return 0;
}

int check_permission(int dev_perm, int access_mode)
{
    if(dev_perm == RDWR)
        return 0;
    
    /*Ensure readonly access*/
    if( (dev_perm == RDONLY) && ( (access_mode & FMODE_READ) && !(access_mode & FMODE_WRITE) ) )
        return 0;
    
    /*Ensure writeonly access*/
    if( (dev_perm == WRONLY) && ( (access_mode & FMODE_WRITE) && !(access_mode & FMODE_READ) ) )
        return 0;

    return -EPERM;
}


int pcd_release(struct inode *inode, struct file *filp)
{
    pr_info("Release was successful.\n");
    return 0;
}

/*file operations of the driver*/
struct file_operations pcd_fops =
    {
        .open = pcd_open,
        .write = pcd_write,
        .read = pcd_read,
        .llseek = pcd_lseek,
        .release = pcd_release,
        .owner = THIS_MODULE,
};

/* Called when the device is found from the system. */
int pcd_platform_driver_probe(struct platform_device *pdev) {
    
    pr_info("A device has been detected.\n");
    return 0;
}

/* Called when the device is removed from the system. */
int pcd_platform_driver_remove(struct platform_device *pdev) {
    /*1. Get the platform data. */

    /*2. Dynamically allocate memory for the device private data. */

    /*3. Dynamically allocate memory for the device buffer using
         size information form the platform data. */

    /*4. Get the device number. */

    /*5. Do cdev init and cdev add. */

    /*6. Create device file for the detected platform device. */

    /*7. Error handling. */

    pr_info("A device has been removed.\n");
    return 0;
}


struct platform_driver pcd_platform_driver = {
    .probe = pcd_platform_driver_probe,
    .remove = pcd_platform_driver_remove,
    .driver = {
        .name = "pseudo_char_device",
    }
};


static int __init pcd_platform_driver_init(void) {
    /*1. Dynamically allocate a device number for MAX_DEVICES. */

    /*2. Create device class under /sys/class */

    /*3. Register a platform driver */
    platform_driver_register(&pcd_platform_driver);
    pr_info("pcd platform driver has been loaded.\n");
    return 0;
}

static void __exit pcd_platform_driver_cleanup(void) {
    
    platform_driver_unregister(&pcd_platform_driver);
    pr_info("pcd platform driver has been unloaded.\n");
}


module_init(pcd_platform_driver_init);
module_exit(pcd_platform_driver_cleanup);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("RICK");
MODULE_DESCRIPTION("A pseudo character platform driver which handles n platform pcdevs.");
