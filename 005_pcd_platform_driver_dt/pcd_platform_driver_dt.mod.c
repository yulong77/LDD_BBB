#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;

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
	{ 0x488e1bc, "module_layout" },
	{ 0x494ce098, "class_destroy" },
	{ 0x5bdfea6c, "platform_driver_unregister" },
	{ 0xd90d0db2, "__platform_driver_register" },
	{ 0x6091b333, "unregister_chrdev_region" },
	{ 0x33ab0d7b, "__class_create" },
	{ 0xe3ec2f2b, "alloc_chrdev_region" },
	{ 0xbe697dba, "device_create" },
	{ 0x4de3d163, "cdev_add" },
	{ 0x947317e8, "cdev_init" },
	{ 0x7dfb952d, "of_match_device" },
	{ 0x7a23a5cf, "of_property_read_variable_u32_array" },
	{ 0xea12cca9, "of_property_read_string" },
	{ 0xd440b725, "devm_kmalloc" },
	{ 0x4a6e01de, "_dev_info" },
	{ 0xd3037b38, "cdev_del" },
	{ 0x6024a43b, "device_destroy" },
	{ 0xc5850110, "printk" },
	{ 0xefd6cf06, "__aeabi_unwind_cpp_pr0" },
};

MODULE_INFO(depends, "");

