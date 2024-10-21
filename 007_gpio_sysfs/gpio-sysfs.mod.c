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
	{ 0xc5850110, "printk" },
	{ 0x33ab0d7b, "__class_create" },
	{ 0x4d639d1a, "device_create_with_groups" },
	{ 0xc358aaf8, "snprintf" },
	{ 0x69c11859, "_dev_warn" },
	{ 0xd5a2d03e, "devm_fwnode_gpiod_get_index" },
	{ 0xe914e41e, "strcpy" },
	{ 0xea12cca9, "of_property_read_string" },
	{ 0x72f4d33, "of_get_next_available_child" },
	{ 0xd440b725, "devm_kmalloc" },
	{ 0xf31edcd8, "_dev_err" },
	{ 0x6ace8bd3, "of_get_next_child" },
	{ 0xc6a69f7e, "device_unregister" },
	{ 0x4a6e01de, "_dev_info" },
	{ 0x8f678b07, "__stack_chk_guard" },
	{ 0x86332725, "__stack_chk_fail" },
	{ 0xcf65517e, "gpiod_set_value" },
	{ 0xaa19e4aa, "_kstrtol" },
	{ 0x19c0b11f, "gpiod_get_value" },
	{ 0x53946f28, "gpiod_direction_output" },
	{ 0x757c0537, "gpiod_direction_input" },
	{ 0x222e7ce2, "sysfs_streq" },
	{ 0xefd6cf06, "__aeabi_unwind_cpp_pr0" },
	{ 0x3c3ff9fd, "sprintf" },
	{ 0x52f7ed91, "gpiod_get_direction" },
};

MODULE_INFO(depends, "");

