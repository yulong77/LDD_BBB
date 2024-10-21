#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/kdev_t.h>
#include <linux/uaccess.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/mod_devicetable.h>
#include <linux/of.h>
#include <linux/of_device.h>

#include "platform.h"


#undef  pr_fmt
#define pr_fmt(fmt) "%s: " fmt, __func__

#define MAX_DEVICES 10

#define RDONLY 0x01
#define WRONLY 0x10
#define RDWR   0x11


struct device_config {
    int config_item1;
    int config_item2;
};

enum pcdev_names {
    PCDEVA1X,
    PCDEVB1X,
    PCDEVC1X,
    PCDEVD1X,
};

struct device_config pcdev_config[] = {
    [PCDEVA1X] = {.config_item1 = 10,    .config_item2 = 20,},
    [PCDEVB1X] = {.config_item1 = 100,   .config_item2 = 200,},
    [PCDEVC1X] = {.config_item1 = 1000,  .config_item2 = 2000,},
    [PCDEVD1X] = {.config_item1 = 10000, .config_item2 = 20000,},
};

/*Device private data structure */
struct pcdev_private_data {

    struct pcdev_platform_data pdata;
    char *buffer;
    dev_t dev_num;
    struct cdev cdev;
};

/*Driver private data structure */
struct pcdrv_private_data {
    
    int total_devices;
    dev_t device_num_base;
    struct class *class_pcd;
    struct device *device_pcd;
};

struct pcdrv_private_data pcdrv_data;

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
struct file_operations pcd_fops = {
        .open = pcd_open,
        .write = pcd_write,
        .read = pcd_read,
        .llseek = pcd_lseek,
        .release = pcd_release,
        .owner = THIS_MODULE,
};

struct pcdev_platform_data* pcdev_get_platdata_from_dt(struct device *dev) {
    
    struct device_node *dev_node = dev->of_node;
    struct pcdev_platform_data *pdata;

    if (dev_node == NULL) {
        /*This probe did not happen because of device tree node. */
        return NULL;
    }

    pdata = devm_kzalloc(dev, sizeof(*pdata), GFP_KERNEL);
    if (pdata == NULL) {
        dev_info(dev, "Memory can not been allocated.\n");
        return ERR_PTR(-ENOMEM);
    }
    
    if (of_property_read_string(dev_node, "org,device-serial-num", &pdata->serial_number)) {
        dev_info(dev, "Serial number property is missing.\n");
        return ERR_PTR(-EINVAL);
    }

    if(of_property_read_u32(dev_node,"org,size",&pdata->size)) {
		dev_info(dev,"Size property is missing.\n");
		return ERR_PTR(-EINVAL);
	}

	if(of_property_read_u32(dev_node,"org,perm",&pdata->perm)) {
		dev_info(dev,"Permission property is missing.\n");
		return ERR_PTR(-EINVAL);
	}

	return pdata;
}


struct of_device_id org_pcdev_dt_match[];

/* Called when the device is found from the system. */
int pcd_platform_driver_probe(struct platform_device *pdev) {

    int ret;
    int driver_data;

    struct pcdev_private_data *dev_data;
    struct pcdev_platform_data *pdata;

    /*Used to store matched entry of "of_device_id" list of this driver. */
    const struct of_device_id *match;
    
    /*If Linux does not support device tree(CONFIG_OF id off), match will always be NULL. */
    match = of_match_device(of_match_ptr(org_pcdev_dt_match), &pdev->dev);
    
    dev_info(&pdev->dev, "A device is detected.\n");

    if (match) {
        pdata = pcdev_get_platdata_from_dt(&pdev->dev);
        if (IS_ERR(pdata)) 
            return PTR_ERR(pdata);
        
        driver_data = (int)match->data;
    } else {
        pdata = (struct pcdev_platform_data*)dev_get_platdata(&pdev->dev);
        driver_data = pdev->id_entry->driver_data;
    }

    if (pdata == NULL) {
        dev_info(&pdev->dev, "No platform data available.\n");
        return -EINVAL;
    }
   
    /*2. Dynamically allocate memory for the device private data. */
    dev_data = devm_kzalloc(&pdev->dev, sizeof(*dev_data), GFP_KERNEL);
    if (dev_data == NULL) {
        dev_info(&pdev->dev, "Cannot allocate the memory.\n");
        return -ENOMEM;
    }
    
    /*Save the device private data pointer in platform device structure.*/
    dev_set_drvdata(&pdev->dev, dev_data);

    dev_data->pdata.size = pdata->size;
    dev_data->pdata.perm = pdata->perm;
    dev_data->pdata.serial_number = pdata->serial_number;

    dev_info(&pdev->dev, "Device serial number = %s.\n", dev_data->pdata.serial_number);
    dev_info(&pdev->dev, "Device size = %d.\n", dev_data->pdata.size);
    dev_info(&pdev->dev, "Device permission = %d.\n", dev_data->pdata.perm);

    dev_info(&pdev->dev, "Config item 1 = %d\n", pcdev_config[driver_data].config_item1);
    dev_info(&pdev->dev, "Config item 2 = %d\n", pcdev_config[driver_data].config_item2);

    /*3. Dynamically allocate memory for the device buffer using
         size information form the platform data. */
    dev_data->buffer = devm_kzalloc(&pdev->dev, dev_data->pdata.size, GFP_KERNEL);
    if (dev_data->buffer == NULL) {
        dev_info(&pdev->dev, "Cannot allocate the memory.\n");
        return -ENOMEM;
    }

    /*4. Get the device number. */
    dev_data->dev_num = pcdrv_data.device_num_base + pcdrv_data.total_devices;

    /*5. Do cdev init and cdev add. */
    cdev_init(&dev_data->cdev, &pcd_fops);

    dev_data->cdev.owner = THIS_MODULE;
    ret = cdev_add(&dev_data->cdev, dev_data->dev_num, 1);
    if (ret < 0) {
        dev_info(&pdev->dev, "Cdev add failed.\n");
        return ret;
    }

    /*6. Create device file for the detected platform device. */
    pcdrv_data.device_pcd = device_create(pcdrv_data.class_pcd, &pdev->dev, 
              dev_data->dev_num, NULL, "pcdev-%d", pcdrv_data.total_devices);
    if (IS_ERR(pcdrv_data.device_pcd)) {
        pr_err("Device creation failed.\n");
        ret = PTR_ERR(pcdrv_data.device_pcd);
        //cdev_del:
        cdev_del(&dev_data->cdev);
        return ret;   
    }
    
    /*Increment the total device number. */
    pcdrv_data.total_devices++;

    dev_info(&pdev->dev, "The probe succeeded.\n");

    return 0;

}

/* Called when the device is removed from the system. */
int pcd_platform_driver_remove(struct platform_device *pdev) {
   
    struct pcdev_private_data *dev_data = dev_get_drvdata(&pdev->dev);

    /*1. Remove a device that was created with device_create. */
    device_destroy(pcdrv_data.class_pcd, dev_data->dev_num);

    /*2. Remove a cdev entry from the system. */
    cdev_del(&dev_data->cdev);

    /*Decrement the total device number. */
    pcdrv_data.total_devices--;
 
    dev_info(&pdev->dev, "A device has been removed.\n");

    return 0;
}


struct platform_device_id pcdevs_ids[] = {
    {.name = "PCDEV-A1X", .driver_data = PCDEVA1X},
    {.name = "PCDEV-B1X", .driver_data = PCDEVB1X},
    {.name = "PCDEV-C1X", .driver_data = PCDEVC1X},
    {.name = "PCDEV-D1X", .driver_data = PCDEVD1X},
    { },
};

struct of_device_id org_pcdev_dt_match[] = {

    {.compatible = "PCDEV-A1X", .data = (void*)PCDEVA1X},
    {.compatible = "PCDEV-B1X", .data = (void*)PCDEVB1X},
    {.compatible = "PCDEV-C1X", .data = (void*)PCDEVC1X},
    {.compatible = "PCDEV-D1X", .data = (void*)PCDEVD1X},
    { },
};

struct platform_driver pcd_platform_driver = {
    .probe = pcd_platform_driver_probe,
    .remove = pcd_platform_driver_remove,
    .id_table = pcdevs_ids,
    .driver = {
        .name = "pseudo_char_device",
        .of_match_table = of_match_ptr(org_pcdev_dt_match),
    }
};


static int __init pcd_platform_driver_init(void) {
    
    int ret;

    /*1. Dynamically allocate a device number for MAX_DEVICES. */
    ret = alloc_chrdev_region(&pcdrv_data.device_num_base, 0, MAX_DEVICES, "pcdevs");
    if (ret < 0) {
        pr_info("Alloc chrdev failed.\n");
        return ret;
    }

    /*2. Create device class under /sys/class */
    pcdrv_data.class_pcd = class_create(THIS_MODULE, "pcd_class");
    if (IS_ERR(pcdrv_data.class_pcd)) {
        pr_err("Class creation failed.\n");
        unregister_chrdev_region(pcdrv_data.device_num_base, MAX_DEVICES);
        return ret;
    }

    /*3. Register a platform driver */
    platform_driver_register(&pcd_platform_driver);
    pr_info("pcd platform driver has been loaded.\n");
    return 0;

}

static void __exit pcd_platform_driver_cleanup(void) {
    
    /*1. Unregister the platform driver. */
    platform_driver_unregister(&pcd_platform_driver);

    /*2. Class destroy. */
    class_destroy(pcdrv_data.class_pcd);
    
    /*3. Unregister device numbers for MAX_DEVICES. */
    unregister_chrdev_region(pcdrv_data.device_num_base, MAX_DEVICES);

    pr_info("pcd platform driver has been unloaded.\n");
}


module_init(pcd_platform_driver_init);
module_exit(pcd_platform_driver_cleanup);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("RICK");
MODULE_DESCRIPTION("A pseudo character platform driver which handles n platform pcdevs.");
