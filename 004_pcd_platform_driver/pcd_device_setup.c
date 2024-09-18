#include <linux/module.h>
#include <linux/platform_device.h>

#include "platform.h"

#undef  pr_fmt
#define pr_fmt(fmt) "%s: " fmt, __func__


void pcdev_release(struct device *dev) {
    pr_info("Device has been released.\n");
}

/* 1. Creat Two platform data. */

struct pcdev_platform_data pcdev_pdata[2] = {
    [0] = {
        .size = 512,
        .perm = RDWR,
        .serial_number = "PCDEV_ABC_1111",
    },

    [1] = {
        .size = 1024,
        .perm = RDWR,
        .serial_number = "PCDEV_XYZ_2222",
    },

};

/* 2. Create Two platform devices. */

struct platform_device platform_pcdev_1 = {
    .name = "pseudo_char_device",
    .id = 0,
    .dev = {
        .platform_data = &pcdev_pdata[0],
        .release = pcdev_release,
    },
};

struct platform_device platform_pcdev_2 = {
    .name = "pseudo_char_device",
    .id = 1,
    .dev = {
        .platform_data = &pcdev_pdata[1],
        .release = pcdev_release,
    },
};


static int __init pcdev_platform_init(void) {
    /*Register platform device*/
    platform_device_register(&platform_pcdev_1);
    platform_device_register(&platform_pcdev_2);

    pr_info("Device setup module has been loaded.\n");

    return 0;
}

static void __exit pcdev_platform_exit(void) {
    platform_device_unregister(&platform_pcdev_1);
    platform_device_unregister(&platform_pcdev_2);

    pr_info("Device setup module has been unloaded.\n");

}


module_init(pcdev_platform_init);
module_exit(pcdev_platform_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("RICK");
MODULE_DESCRIPTION("Module of registers platform devices.");


