#include <linux/module.h>
#include <linux/platform_device.h>

#include "platform.h"

#undef  pr_fmt
#define pr_fmt(fmt) "%s: " fmt, __func__


void pcdev_release(struct device *dev) {
    pr_info("Device has been released.\n");
}

/* 1. Creat 4 platform data. */

struct pcdev_platform_data pcdev_pdata[4] = {
    [0] = {
        .size = 512,
        .perm = RDWR,
        .serial_number = "PCDEV_ABC_1111",
    },

    [1] = {
        .size = 1024,
        .perm = RDWR,
        .serial_number = "PCDEV_ABC_2222",
    },

    [2] = {
        .size = 128,
        .perm = RDONLY,
        .serial_number = "PCDEV_ABC_3333",
    },

    [3] = {
        .size = 32,
        .perm = WRONLY,
        .serial_number = "PCDEV_ABC_4444",
    },

};

/* 2. Create 4 platform devices. */

struct platform_device platform_pcdev_1 = {
    .name = "PCDEV-A1X",
    .id = 0,
    .dev = {
        .platform_data = &pcdev_pdata[0],
        .release = pcdev_release,
    },
};

struct platform_device platform_pcdev_2 = {
    .name = "PCDEV-B1X",
    .id = 1,
    .dev = {
        .platform_data = &pcdev_pdata[1],
        .release = pcdev_release,
    },
};

struct platform_device platform_pcdev_3 = {
    .name = "PCDEV-C1X",
    .id = 2,
    .dev = {
        .platform_data = &pcdev_pdata[2],
        .release = pcdev_release,
    },
};

struct platform_device platform_pcdev_4 = {
    .name = "PCDEV-D1X",
    .id = 3,
    .dev = {
        .platform_data = &pcdev_pdata[3],
        .release = pcdev_release,
    },
};

struct platform_device *platform_pcdevs[] = {
    &platform_pcdev_1,
    &platform_pcdev_2,
    &platform_pcdev_3,
    &platform_pcdev_4,
};

static int __init pcdev_platform_init(void) {
    /*Register platform device*/
    //platform_device_register(&platform_pcdev_1);
    //platform_device_register(&platform_pcdev_2);

    platform_add_devices(platform_pcdevs, ARRAY_SIZE(platform_pcdevs)),

    pr_info("Device setup module has been loaded.\n");

    return 0;
}

static void __exit pcdev_platform_exit(void) {
    platform_device_unregister(&platform_pcdev_1);
    platform_device_unregister(&platform_pcdev_2);
    platform_device_unregister(&platform_pcdev_3);
    platform_device_unregister(&platform_pcdev_4);

    pr_info("Device setup module has been unloaded.\n");

}


module_init(pcdev_platform_init);
module_exit(pcdev_platform_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("RICK");
MODULE_DESCRIPTION("Module of registers platform devices.");


