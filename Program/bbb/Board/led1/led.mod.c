#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0xae4b0ceb, "module_layout" },
	{ 0x2f729c13, "class_destroy" },
	{ 0xb45e1167, "device_destroy" },
	{ 0x37a0cba, "kfree" },
	{ 0x86c8ad2e, "malloc_sizes" },
	{ 0x8758668a, "kmem_cache_alloc_trace" },
	{ 0x6c4fc9f7, "device_create" },
	{ 0x95d52825, "__class_create" },
	{ 0x12c554f0, "cdev_del" },
	{ 0xea01506a, "cdev_add" },
	{ 0x1023a56c, "cdev_init" },
	{ 0x7485e15e, "unregister_chrdev_region" },
	{ 0x6f395f2, "cdev_alloc" },
	{ 0x29537c9e, "alloc_chrdev_region" },
	{ 0x82f776b7, "gpio_export" },
	{ 0x47229b5c, "gpio_request" },
	{ 0xfbc74f64, "__copy_from_user" },
	{ 0x6c8d5ae8, "__gpio_get_value" },
	{ 0xa8f59416, "gpio_direction_output" },
	{ 0xe61a6d2f, "gpio_unexport" },
	{ 0xfe990052, "gpio_free" },
	{ 0x67c2fa54, "__copy_to_user" },
	{ 0x432fd7f6, "__gpio_set_value" },
	{ 0xfa2a45e, "__memzero" },
	{ 0xefd6cf06, "__aeabi_unwind_cpp_pr0" },
	{ 0xf962ac2a, "module_refcount" },
	{ 0x2e5810c6, "__aeabi_unwind_cpp_pr1" },
	{ 0x27e1a049, "printk" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "965B85F596BA438F734E28B");
