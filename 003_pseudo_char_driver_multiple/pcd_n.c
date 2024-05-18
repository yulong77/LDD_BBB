#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/kdev_t.h>
#include <linux/uaccess.h>

#undef pr_fmt
#define pr_fmt(fmt) "%s: " fmt, __func__

#define NO_OF_DEVICES 4

#define MEM_SIZE_MAX_PCDEV1 1024
#define MEM_SIZE_MAX_PCDEV2 1024
#define MEM_SIZE_MAX_PCDEV3 1024
#define MEM_SIZE_MAX_PCDEV4 1024

/*pseudo device's memory*/
char device_buffer_pcdev1[MEM_SIZE_MAX_PCDEV1];
char device_buffer_pcdev2[MEM_SIZE_MAX_PCDEV2];
char device_buffer_pcdev3[MEM_SIZE_MAX_PCDEV3];
char device_buffer_pcdev4[MEM_SIZE_MAX_PCDEV4];

struct pcdev_private_data
{
    char *buffer;
    unsigned size;
    const char *serial_number;
    int perm;
    struct cdev cdev;
};

/*Driver private data structure*/
struct pcdrv_private_data
{
    int total_devices;

    /*This holds the device number*/
    dev_t device_number;

    struct class *class_pcd;
    struct device *device_pcd;

    struct pcdev_private_data pcdev_data[NO_OF_DEVICES];
};

struct pcdrv_private_data pcdrv_data = {
    .total_devices = NO_OF_DEVICES,
    .pcdev_data = {
        [0] = {
            .buffer = device_buffer_pcdev1,
            .size = MEM_SIZE_MAX_PCDEV1,
            .serial_number = "PCDEV1XYZ123",
            .perm = 0x1, /*Read only*/
        },
        [1] = {
            .buffer = device_buffer_pcdev2, 
            .size = MEM_SIZE_MAX_PCDEV2, 
            .serial_number = "PCDEV2XYZ123", 
            .perm = 0x10, /*Write only*/
        },
        [2] = {
            .buffer = device_buffer_pcdev3, 
            .size = MEM_SIZE_MAX_PCDEV3, 
            .serial_number = "PCDEV3XYZ123", 
            .perm = 0x11, /*Read & Write*/
        },
        [3] = {
            .buffer = device_buffer_pcdev4, 
            .size = MEM_SIZE_MAX_PCDEV4, 
            .serial_number = "PCDEV4XYZ123", 
            .perm = 0x11, /*Read & Write*/
        }}};

loff_t pcd_lseek(struct file *filp, loff_t offset, int whence)
{
#if 0	
    pr_info("lseek requested.\n");
    pr_info("Current value of the file position = %lld\n", filp->f_pos);
    
    loff_t res;

    switch(whence) {
        case SEEK_SET: 
            if ((offset > DEV_MEM_SIZE) || (offset < 0))
                return -EINVAL;
            filp->f_pos = offset;
            break;

        case SEEK_CUR:
            res = filp->f_pos + offset;
            if ((res > DEV_MEM_SIZE) || (res < 0))
                return -EINVAL;
            filp->f_pos = res;
            break;

        case SEEK_END:
            res = DEV_MEM_SIZE + offset;
            if ((res > DEV_MEM_SIZE) || (res < 0))
                return -EINVAL;
            filp->f_pos = res;
            break;

        default:
            return -EINVAL; 
    }
    
    pr_info("New value of the file position = %lld\n", filp->f_pos);

	return filp->f_pos;
#endif
    return 0;
}

ssize_t pcd_read(struct file *filp, char __user *buff, size_t count, loff_t *f_pos)
{
#if 0	
    pr_info("Read requested for %zu bytes.\n", count);
    pr_info("Current file position = %lld\n", *f_pos);

    /*Adjust the 'count'*/
    if ((*f_pos + count) > DEV_MEM_SIZE) 
        count = DEV_MEM_SIZE - *f_pos;

	/*Copy to user*/
    if (copy_to_user(buff, &device_buffer[*f_pos], count)) {
        return -EFAULT;
    }

	/*Update the current file position*/
    *f_pos += count;

    pr_info("Number of bytes successfully read = %zu\n", count);
    pr_info("Updated file position = %lld\n", *f_pos);

    /*Return number of bytes which have been successfully read. */
	return count;
#endif
    return 0;
}

ssize_t pcd_write(struct file *filp, const char __user *buff, size_t count, loff_t *f_pos)
{
#if 0	
    pr_info("write requested for %zu bytes.\n", count);
    pr_info("Current file position = %lld\n", *f_pos);

    /*Adjust the 'count'*/
    if ((*f_pos + count) > DEV_MEM_SIZE) 
        count = DEV_MEM_SIZE - *f_pos;
    
    if (!count) {
        pr_err("No space left on the device.\n");
        return -ENOMEM;
    }

	/*Copy from user*/
    if (copy_from_user(&device_buffer[*f_pos], buff, count)) {
        return -EFAULT;
    }

	/*Update the current file position*/
    *f_pos += count;

    pr_info("Number of bytes successfully written = %zu\n", count);
    pr_info("Updated file position = %lld\n", *f_pos);

    /*Return number of bytes which have been successfully written. */
	return count;
#endif
    return -ENOMEM;
}

int pcd_open(struct inode *inode, struct file *filp)
{

    pr_info("Open was successful.\n");
    return 0;
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

static int __init pcd_driver_init(void)
{
    int i; /*iterate devices.*/
    int retval;

    /*1. Dynamically allocate a device number.*/
    retval = alloc_chrdev_region(&pcdrv_data.device_number, 0, 1, "pcb_devices");
    if (retval < 0)
    {
        pr_info("Alloc chrdev failed.\n");
        goto out;
    }

    /*Create device class under /sys/class/ */
    pcdrv_data.class_pcd = class_create("pcd_class");
    if (IS_ERR(pcdrv_data.class_pcd))
    {
        pr_err("Class creation failed,\n");
        retval = PTR_ERR(pcdrv_data.class_pcd);
        goto unreg_chrdev;
    }

    for (i = 0; i < NO_OF_DEVICES; ++i)
    {
        pr_info("Device number <major>:<minor> = %d:%d\n",
                MAJOR(pcdrv_data.device_number + i),
                MINOR(pcdrv_data.device_number + i));

        /*Initialize the cdev structure with fops.*/
        cdev_init(&pcdrv_data.pcdev_data[i].cdev, &pcd_fops);

        /*Register a device(cdev structure) with VFS.*/
        pcdrv_data.pcdev_data[i].cdev.owner = THIS_MODULE;

        retval = cdev_add(&pcdrv_data.pcdev_data[i].cdev, pcdrv_data.device_number+i, 1);
        if (retval < 0)
        {
            pr_info("Cdev add failed.\n");
            goto cdev_del;
        }


        /*Populate the sysfs with device information.*/
        pcdrv_data.device_pcd = device_create(pcdrv_data.class_pcd, NULL, 
              pcdrv_data.device_number+i, NULL, "pcdev-%d", i+1);
        if (IS_ERR(pcdrv_data.device_pcd))
        {
            pr_err("Device creation failed,\n");
            retval = PTR_ERR(pcdrv_data.device_pcd);
            goto class_del;
        }
    }

    pr_info("Module init was successful\n");

    return 0;
    
    cdev_del:
    class_del:
        for (; i>=0; --i) {
            device_destroy(pcdrv_data.class_pcd, pcdrv_data.device_number+i);
            cdev_del(&pcdrv_data.pcdev_data[i].cdev);
        }
        class_destroy(pcdrv_data.class_pcd);
    
    unreg_chrdev:
        unregister_chrdev_region(pcdrv_data.device_number, NO_OF_DEVICES);

    out:
        pr_info("Module insertion failed.\n");
        return retval;

}

static void __exit pcd_driver_cleanup(void)
{
    for (int i=0; i<NO_OF_DEVICES; ++i) {
            device_destroy(pcdrv_data.class_pcd, pcdrv_data.device_number+i);
            cdev_del(&pcdrv_data.pcdev_data[i].cdev);
        }
    class_destroy(pcdrv_data.class_pcd);

    unregister_chrdev_region(pcdrv_data.device_number, NO_OF_DEVICES);

    pr_info("Module unloaded\n");
}

module_init(pcd_driver_init);
module_exit(pcd_driver_cleanup);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("RICK");
MODULE_DESCRIPTION("A pseudo character driver which handles n devices.");
