#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/kdev_t.h>
#include <linux/uaccess.h>

#define DEV_MEM_SIZE 512

#undef pr_fmt
#define pr_fmt(fmt) "%s: " fmt, __func__

/* pseudo device's memory */
char device_buffer[DEV_MEM_SIZE];

/* This holds the device number */
dev_t device_number;

struct cdev pcd_cdev;

loff_t pcd_lseek (struct file *filp, loff_t offset, int whence) {
	
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
}

ssize_t pcd_read (struct file *filp, char __user *buff, size_t count, loff_t *f_pos) {
	
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
}

ssize_t pcd_write (struct file *filp, const char __user *buff, size_t count, loff_t *f_pos) {
	
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
}

int pcd_open (struct inode *inode, struct file *filp) {

	pr_info("Open was successful.\n");
	return 0;
}

int pcd_release (struct inode *inode, struct file *filp) {

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

struct class *class_pcd;

struct device *device_pcd;

static int __init pcd_driver_init(void) {
    
    int retval;

    /*1. Dynamically allocate a device number.*/
    retval = alloc_chrdev_region(&device_number, 0, 1, "pcb_devices");
    if (retval < 0) {
        pr_info("Alloc chrdev failed.\n");
        goto out;
    }

    pr_info("Device number <major>:<minor> = %d:%d\n", MAJOR(device_number), MINOR(device_number));

    /*2. Initialize the cdev structure with fops.*/
    cdev_init(&pcd_cdev, &pcd_fops);

    /*3. Register a device(cdev structure) with VFS.*/
    pcd_cdev.owner = THIS_MODULE;
    
    retval = cdev_add(&pcd_cdev, device_number, 1);
    if (retval < 0) {
        pr_info("Cdev add failed.\n");
        goto unreg_chrdev;
    }
    
    /*4. Create device class under /sys/class/ */
    class_pcd = class_create("pcd_class");
    if(IS_ERR(class_pcd)) {
        pr_err("Class creation failed,\n");
        retval = PTR_ERR(class_pcd);
        goto cdev_del;
    }

    /*5. Populate the sysfs with device information.*/
    device_pcd = device_create(class_pcd, NULL, device_number, NULL, "pcd");
    if(IS_ERR(device_pcd)) {
        pr_err("Device creation failed,\n");
        retval = PTR_ERR(device_pcd);
        goto class_del;
    }

    pr_info("Module init was successful\n");

    return 0;

    class_del:
        class_destroy(class_pcd);
    
    cdev_del:
        cdev_del(&pcd_cdev);

    unreg_chrdev:
        unregister_chrdev_region(device_number, 1);

    out:
        pr_info("Module insertion failed.\n");
        return retval;
}


static void __exit pcd_driver_cleanup(void) {
    device_destroy(class_pcd, device_number);
    class_destroy(class_pcd);
    cdev_del(&pcd_cdev);
    unregister_chrdev_region(device_number, 1);
    pr_info("Module unloaded.\n");
}

module_init(pcd_driver_init);
module_exit(pcd_driver_cleanup);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("RICK");
MODULE_DESCRIPTION("A pseudo character");
