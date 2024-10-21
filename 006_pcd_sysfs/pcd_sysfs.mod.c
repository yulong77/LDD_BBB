#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/elfnote-lto.h>
#include <linux/export-internal.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

#ifdef CONFIG_UNWINDER_ORC
#include <asm/orc_header.h>
ORC_HEADER;
#endif

BUILD_SALT;
BUILD_LTO_INFO;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif



static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0xc8dcc62a, "krealloc" },
	{ 0xe3ec2f2b, "alloc_chrdev_region" },
	{ 0xe08736b, "devm_kmalloc" },
	{ 0xae0da4b6, "of_property_read_variable_u32_array" },
	{ 0x9deb2665, "platform_driver_unregister" },
	{ 0xa4e5832c, "class_destroy" },
	{ 0x122c3a7e, "_printk" },
	{ 0xf0fdf6cb, "__stack_chk_fail" },
	{ 0x6cbbfc54, "__arch_copy_to_user" },
	{ 0x141e757c, "_dev_info" },
	{ 0x39f012c6, "cdev_add" },
	{ 0xdda94c8e, "of_match_device" },
	{ 0x16736258, "_dev_err" },
	{ 0xfbe72bcf, "device_create" },
	{ 0x3fa498ad, "class_create" },
	{ 0x3233f85d, "of_property_read_string" },
	{ 0x757bcf5e, "sysfs_create_group" },
	{ 0xdcb764ad, "memset" },
	{ 0xcd32f847, "__platform_driver_register" },
	{ 0x3c3ff9fd, "sprintf" },
	{ 0x6091b333, "unregister_chrdev_region" },
	{ 0xb86bb5ac, "device_destroy" },
	{ 0x12a4e128, "__arch_copy_from_user" },
	{ 0x3854774b, "kstrtoll" },
	{ 0xe78e8907, "cdev_init" },
	{ 0xa4afd25e, "cdev_del" },
	{ 0x67a35d9, "module_layout" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "11D5002F8157A527003A301");
